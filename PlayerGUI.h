#pragma once

#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener
{
public:
    PlayerGUI();
    ~PlayerGUI() override;
    void resized() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) ;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) ;
    void releaseResources() ;
 


private:
    PlayerAudio PlayerAudio1;

    juce::TextButton loadButton{ "Load" };
    juce::TextButton playButton{ "play" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton muteButton{ "Mute" };
    juce::Slider volumeSlider;
    juce::Slider speedSlider;
    //void loadTrack(const juce::File& file);
    std::unique_ptr<juce::FileChooser> fileChooser;
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};