#pragma once
#include <JuceHeader.h>
#include <functional>
#include <vector>

class PlayerAudio {
public:
    PlayerAudio();
    ~PlayerAudio();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void releaseResources();
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);

    void setMuted(bool choice);
    bool loadFile(const juce::File& audioFile);
    void play();

    void setGain(float newGain);
    void setPosition(double posInSeconds);
    double getPosition() const;
    double getLength() const;
    void pause();
    float getGain() const;
    bool isMuted() const;
    bool isPlaying() const { return transportSource.isPlaying(); }
    void repeat();

    juce::String getTitle() const { return title; }
    juce::String getArtist() const { return artist; }
    juce::String getDuration() const { return duration; }

    juce::Component* getParentComponent() const { return parentComponent; }
    void setParentComponent(juce::Component* parent) { parentComponent = parent; }

    void fav();
    juce::Array<juce::File>favorite;
    juce::ComboBox Myfav;
    juce::File currentFile;

private:
    bool muted = false;
    float gainBeforeMute = 1.0f;
    bool Repeat = false;
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource;
    juce::String title;
    juce::String artist;
    juce::String duration;
    juce::Component* parentComponent = nullptr;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};

class PlaylistModel;