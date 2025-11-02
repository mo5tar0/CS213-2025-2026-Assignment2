#pragma once
#include <JuceHeader.h>


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
    bool isPlaying() const;
    void repeat();

    void setSpeed(double speed);
    double getSpeed() const;

    juce::String getTitle() const { return title; }
    juce::String getArtist() const { return artist; }
    juce::String getDuration() const { return duration; }

    juce::Component* getParentComponent() const { return parentComponent; }
    void setParentComponent(juce::Component* parent) { parentComponent = parent; }

    void fav();
    juce::Array<juce::File>favorite;
    juce::ComboBox Myfav;
    juce::File currentFile;

    juce::AudioFormatManager& getFormatManager() { return formatManager; }
    void setThumbnailSource(const juce::File& file);
    juce::AudioThumbnail* getThumbnail() const { return thumbnail ? thumbnail.get() : nullptr; }
    bool hasThumbnail() const;
    juce::AudioSource* getAudioSource() { return &resampleSource; }
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
    std::unique_ptr<juce::AudioThumbnail> thumbnail;
    juce::AudioThumbnailCache thumbnailCache{ 5 };
	
    double currentspeed = 1.0f;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};
class PlaylistModel;