#include"JuceHeader.h"
#include "PlayerGUI.h"

PlayerGUI::PlayerGUI() {

    for (auto* btn : { &loadButton, &playButton,  &muteButton, &goToStartButton, &goToEndButton ,&repeatButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
        playButton.setClickingTogglesState(true);


    }

    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

    muteButton.addListener(this);
    addAndMakeVisible(muteButton);
    addAndMakeVisible(titleLabel);
    addAndMakeVisible(artistLabel);
    addAndMakeVisible(durationLabel);
    addAndMakeVisible(addToPlaylistButton);
    addAndMakeVisible(playlistBox);
    addToPlaylistButton.addListener(this);
    playlistModel = std::make_unique<PlaylistModel>(playlistFiles, PlayerAudio1,*this);
    playlistBox.setModel(playlistModel.get());

}
PlayerGUI::~PlayerGUI() {}


void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    PlayerAudio1.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    PlayerAudio1.getNextAudioBlock(bufferToFill);
}
void PlayerGUI::releaseResources()
{
    PlayerAudio1.releaseResources();
}
void PlayerGUI::resized()
{
    int y = 20;
    loadButton.setBounds(20, y, 100, 40);
    playButton.setBounds(140, y, 80, 40);

    muteButton.setBounds(240, y, 80, 40);
    goToStartButton.setBounds(340, y, 100, 40);
    goToEndButton.setBounds(460, y, 100, 40);
    repeatButton.setBounds(580, y, 80, 40);

    volumeSlider.setBounds(20, 100, getWidth() - 40, 30);
    titleLabel.setBounds(20, 150, getWidth() - 40, 30);
    artistLabel.setBounds(20, 180, getWidth() - 40, 30);
    durationLabel.setBounds(20, 210, getWidth() - 40, 30);
	addToPlaylistButton.setBounds(20, 250, 150, 30);
	playlistBox.setBounds(20, 290, getWidth() - 40, getHeight() - 310);




}
void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        juce::FileChooser chooser("Select audio files...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile()) {
                    PlayerAudio1.loadFile(file);
                    titleLabel.setText("Title: " + PlayerAudio1.getTitle(), juce::dontSendNotification);
                    artistLabel.setText("Artist: " + PlayerAudio1.getArtist(), juce::dontSendNotification);
                    durationLabel.setText("Duration: " + PlayerAudio1.getDuration(), juce::dontSendNotification);
                    PlayerAudio1.play();
                    playButton.setButtonText("pause");
                    isPlaying = true;
                }
            })
            ;
    }

    else if (button == &playButton)
    {

        if (PlayerAudio1.isPlaying())
        {
            PlayerAudio1.pause();
            playButton.setButtonText("Play");


        }
        else
        {
            PlayerAudio1.play();
            playButton.setButtonText("Pause");

        }
    }

    else if (button == &repeatButton)
    {
        PlayerAudio1.repeat();
        if (repeatButton.getButtonText() == "Repeat") {

            repeatButton.setButtonText("UnRepeat");
        }
        else {
            repeatButton.setButtonText("Repeat");
        }
    }


    else if (button == &muteButton)
    {
        bool shouldMute = !PlayerAudio1.isMuted();
        PlayerAudio1.setMuted(shouldMute);

        if (shouldMute)
            muteButton.setButtonText("Unmute");
        else
            muteButton.setButtonText("Mute");
    }
    else if (button == &goToStartButton)
    {
        PlayerAudio1.setPosition(0.0);
        PlayerAudio1.play();
        playButton.setButtonText("Pause");

    }
    else if (button == &goToEndButton)
    {
        double length = PlayerAudio1.getLength();
        PlayerAudio1.setPosition(length);
        playButton.setToggleState(false, juce::dontSendNotification);
        playButton.setButtonText("Play");

    }
    else if (button == &addToPlaylistButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select audio files to add to playlist...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile()) {
                    playlistFiles.push_back(file);
                    playlistBox.updateContent();
                    repaint();
                    
                    
                    
                    
                }
            });
    }
            
            
                
               
            
	
    }

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        PlayerAudio1.setGain((float)slider->getValue());

}


