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

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();



private:
    PlayerAudio PlayerAudio1;

    juce::TextButton loadButton{ "Load" };
    juce::TextButton playButton{ "play" };
    juce::TextButton muteButton{ "Mute" };
    juce::Slider volumeSlider;
    juce::TextButton goToStartButton{ "Go to Start" };
    juce::TextButton goToEndButton{ "Go to End" };
    std::unique_ptr<juce::FileChooser> fileChooser;
    bool isPlaying = false;
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)

};
