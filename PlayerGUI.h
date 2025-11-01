#pragma once

#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer,
    public juce::ComboBox::Listener
{
public:
    PlayerGUI();
    ~PlayerGUI() override;
    void resized() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    void paint(juce::Graphics& g) override;

    void updateLabels() {
        updateMetadata();
    };

    juce::Slider positionSlider;
    

    void resetCurrentPosition() {
        currentPosition = 0.0;
        if (waveformComponent)
            waveformComponent->repaint();
    }

    void updateWaveformThumbnail() {
        if (waveformComponent) {
            auto* newThumbnail = PlayerAudio1.getThumbnail();
            waveformComponent->setThumbnail(newThumbnail);
        }
    }

    void startPlaybackTimer() { startTimer(50); }
    PlayerAudio PlayerAudio1;
private:
    

    class WaveformDisplay : public juce::Component {
    public:
        WaveformDisplay(juce::AudioThumbnail* t, double& pos) : thumbnail(t), position(pos) {}


        void setThumbnail(juce::AudioThumbnail* newThumbnail) {
            thumbnail = newThumbnail;
            repaint();
        }

        void paint(juce::Graphics& g) override {
            auto bounds = getLocalBounds();
            g.fillAll(juce::Colours::darkgrey);

            if (thumbnail && thumbnail->getTotalLength() > 0) {

                g.setColour(juce::Colour(0xFFFF4081));
                thumbnail->drawChannels(g, bounds.reduced(2), 0.0, thumbnail->getTotalLength(), 1.0f);

                g.setColour(juce::Colours::red);
                double ratio = position / thumbnail->getTotalLength();
                int x = static_cast<int>(ratio * bounds.getWidth());
                g.drawLine(x, 0, x, bounds.getHeight(), 3.0f);
            }
            else {
                g.setColour(juce::Colours::grey);
                g.setFont(14.0f);
                g.drawText("No waveform - Load a valid file", bounds, juce::Justification::centred);
            }
        }

    private:
        juce::AudioThumbnail* thumbnail;
        double& position;
    };

    juce::TextButton loadButton{ "Load" };
    juce::TextButton playButton{ "play" };
    juce::TextButton muteButton{ "Mute" };

    juce::Slider volumeSlider;
    juce::TextButton goToStartButton{ "Go to Start" };
    juce::TextButton goToEndButton{ "Go to End" };
    juce::TextButton repeatButton{ "Repeat" };
    juce::TextButton set_AButton{ "Set A" };
    juce::TextButton set_BButton{ "Set B" };
    juce::TextButton favoriteButton{ "My Favorite" };
    double A = 0;
    double B = 0;
    bool repeat = false;
    void  comboBoxChanged(juce::ComboBox* comboBox);

    juce::Label titleLabel;
    juce::Label artistLabel;
    juce::Label durationLabel;
    juce::TextButton addToPlaylistButton{ "Add to Playlist" };
    juce::ListBox playlistBox;
    std::vector<juce::File> playlistFiles;
    std::unique_ptr<PlaylistModel>playlistModel;
    bool isPlaying = false;
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void updateMetadata()
    {
        titleLabel.setText("Title: " + PlayerAudio1.getTitle(), juce::dontSendNotification);
        artistLabel.setText("Artist: " + PlayerAudio1.getArtist(), juce::dontSendNotification);
        durationLabel.setText("Duration: " + PlayerAudio1.getDuration(), juce::dontSendNotification);
        playButton.setButtonText("Pause");
    }

    juce::TextButton sleepTimerButton{ "Sleep Timer" };
    juce::Slider speedSlider;

    juce::TextButton forward10sButton{ "+ 10s" };
    juce::TextButton backward10sButton{ "- 10s" };

    std::unique_ptr<juce::FileChooser> fileChooser;

    juce::Label timeLabel;
    bool useProgressBar = true;
    juce::Label volumeLabel;

    juce::Label speedLabel;

    bool sleepTimerEnabled = false;
    int sleepTimerCounter = 0;
    const int sleepTimerDuration = 10 * 60 * 20;

    std::unique_ptr<juce::AudioThumbnail> thumbnail;
    juce::AudioThumbnailCache thumbnailCache{ 5 };
    double currentPosition = 0.0;
    std::unique_ptr<WaveformDisplay> waveformComponent;
    void timerCallback() override;

    juce::String secondsToTime(double seconds);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};

class PlaylistModel : public juce::ListBoxModel
{
public:
    PlaylistModel(std::vector<juce::File>& files, PlayerAudio& playerRef, PlayerGUI& guiRef)
        : playlistFiles(files), player(playerRef), gui(guiRef) {
    }

    int getNumRows() override { return (int)playlistFiles.size(); }

    void paintListBoxItem(int rowNumber, juce::Graphics& g,
        int width, int height, bool rowIsSelected) override
    {

        if (rowIsSelected)
            g.fillAll(juce::Colour(0xFFFF2D8B));
        else
            g.fillAll(juce::Colour(0xFF3A2F45));


        g.setColour(juce::Colours::white);

        if (rowNumber < (int)playlistFiles.size())
            g.drawText(playlistFiles[rowNumber].getFileNameWithoutExtension(),
                10, 0, width, height, juce::Justification::centredLeft);
    }

    void listBoxItemClicked(int row, const juce::MouseEvent&) override
    {
        if (row >= 0 && row < (int)playlistFiles.size())
        {
            gui.PlayerAudio1.currentFile = playlistFiles[row];
            if (player.loadFile(playlistFiles[row]))
            {
                player.play();
                gui.updateLabels();

                gui.positionSlider.setRange(0.0, player.getLength());
                gui.positionSlider.setValue(0.0);


                gui.updateWaveformThumbnail();
                gui.startPlaybackTimer();
                gui.resetCurrentPosition();
            }
        }
    }

private:
    std::vector<juce::File>& playlistFiles;
    PlayerAudio& player;
    PlayerGUI& gui;
};