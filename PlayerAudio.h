#pragma once
#include <JuceHeader.h>

class PlayerAudio {
public:
    PlayerAudio();
    ~PlayerAudio() ;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) ;
    void releaseResources() ;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) ;
   
    void setMuted(bool choice);
    bool loadFile(const juce::File& audioFile);
    void play();
    void stop();
    void setGain(float newGain);
    void setPosition(double posInSeconds);
    double getPosition() const;
    double getLength() const;
   
    float getGain() const;
    bool isMuted() const;


private:
    bool muted = false;
    float gainBeforeMute = 1.0f;
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};