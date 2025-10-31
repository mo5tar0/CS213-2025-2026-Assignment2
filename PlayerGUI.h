#pragma once

#include <JuceHeader.h>
#include "PlayerAudio.h"
#include<vector>

class PlaylistModel;


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

    void updateLabels() {
        updateMetadata();
    };
 
    void timerCallback();

    juce::Slider positionSlider;



private:
    PlayerAudio PlayerAudio1;

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
    std::unique_ptr<juce::FileChooser> fileChooser;
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
            g.fillAll(juce::Colours::lightblue);

        if (rowNumber < (int)playlistFiles.size())
            g.drawText(playlistFiles[rowNumber].getFileNameWithoutExtension(),
                10, 0, width, height, juce::Justification::centredLeft);
    }

    void listBoxItemClicked(int row, const juce::MouseEvent&) override
    {
        if (row >= 0 && row < (int)playlistFiles.size())
        {
            if (player.loadFile(playlistFiles[row]))
            {
                player.play();
                gui.updateLabels();
                gui.positionSlider.setRange(0.0, player.getLength());
                gui.positionSlider.setValue(0.0);
            }
        }
    }

private:
    std::vector<juce::File>& playlistFiles;
    PlayerAudio& player;
    PlayerGUI& gui;
};