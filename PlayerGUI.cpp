#include"JuceHeader.h"
#include "PlayerGUI.h"

PlayerGUI::PlayerGUI() {

    for (auto* btn : { &loadButton, &playButton, &stopButton, &muteButton, &goToStartButton, &goToEndButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
		playButton.setClickingTogglesState(true);
        playButton.onClick = [this]() {
            if (playButton.getToggleState()) {
                PlayerAudio1.play();
				playButton.setButtonText("Pause");
            }
            else {
                PlayerAudio1.pause();
				playButton.setButtonText("Play");
            }
			};
    }


    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

    muteButton.addListener(this);
    addAndMakeVisible(muteButton);


}
PlayerGUI::~PlayerGUI() {}


void PlayerGUI:: prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    PlayerAudio1.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void PlayerGUI ::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    PlayerAudio1.getNextAudioBlock(bufferToFill);
}
void PlayerGUI::releaseResources()
{
    PlayerAudio1.releaseResources();
}
void PlayerGUI ::resized()
{
    int y = 20;
    loadButton.setBounds(20, y, 100, 40);
    playButton.setBounds(140, y, 80, 40);
    stopButton.setBounds(240, y, 80, 40);
    muteButton.setBounds(340, y, 80, 40);
	goToStartButton.setBounds(440, y, 100, 40);
	goToEndButton.setBounds(560, y, 100, 40);

    volumeSlider.setBounds(20, 100, getWidth() - 40, 30);

    
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
                }
            })
            ;
    }

    else if (button == &playButton)
    {
        if (isPlaying)
        {
            PlayerAudio1.pause();
            playButton.setButtonText("Play");
            isPlaying = false;
        }
        else
        {
            PlayerAudio1.play();
            playButton.setButtonText("Pause");
            isPlaying = true;
        }
    }
    
  
    

    else if (button == &stopButton)
    {
        PlayerAudio1.stop();
        PlayerAudio1.setPosition(0.0);
		playButton.setToggleState(false, juce::dontSendNotification);
		playButton.setButtonText("Play");
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
    else if(button == &goToStartButton)
    {
        PlayerAudio1.setPosition(0.0);
        PlayerAudio1.play();
        playButton.setButtonText("Pause");
        isPlaying = true;
	}
    else if (button == &goToEndButton)
    {
        double length = PlayerAudio1.getLength();
        PlayerAudio1.setPosition(length);
        playButton.setToggleState(false, juce::dontSendNotification);
        playButton.setButtonText("Play");
    
	}
}
void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        PlayerAudio1.setGain((float)slider->getValue());

}

