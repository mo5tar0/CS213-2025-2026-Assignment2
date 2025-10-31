#include "PlayerAudio.h"

PlayerAudio::PlayerAudio() : resampleSource(&transportSource, false)
{
    formatManager.registerBasicFormats();
}

PlayerAudio::~PlayerAudio()
{
    releaseResources();
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::releaseResources()
{
    resampleSource.releaseResources();
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);
}

bool PlayerAudio::loadFile(const juce::File& file) {
    if (auto* reader = formatManager.createReaderFor(file)) {
        transportSource.stop();
        transportSource.setSource(nullptr);
        readerSource.reset();
        readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
        transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);
        setSpeed(currentspeed);
        setThumbnailSource(file);

        transportSource.stop();
        transportSource.setPosition(0);

        title = file.getFileNameWithoutExtension();
        auto meta = reader->metadataValues;
        if (meta.containsKey("title"))
            title = meta["title"];
        if (meta.containsKey("artist"))
            artist = meta["artist"];
        double lengthInSeconds =
            reader->lengthInSamples / reader->sampleRate;
        int minutes = int(lengthInSeconds) / 60;
        int seconds = int(lengthInSeconds) % 60;
        duration = juce::String(minutes) + ":" + (seconds < 10 ? "0" : "") + juce::String(seconds);
        if (artist.isEmpty())
            artist = "Unknown Artist";
        if (title.isEmpty())
            title = file.getFileNameWithoutExtension();

        return true;
    }
    return false;
}
void PlayerAudio::setThumbnailSource(const juce::File& file) {
    if (!thumbnail) {
        thumbnail = std::make_unique<juce::AudioThumbnail>(512, formatManager, thumbnailCache);
    }
    thumbnail->setSource(new juce::FileInputSource(file));
}

void PlayerAudio::setMuted(bool choice)
{
    if (choice && !muted) {

        gainBeforeMute = transportSource.getGain();
        transportSource.setGain(0.0f);
        muted = true;
    }
    else if (!choice && muted) {

        transportSource.setGain(gainBeforeMute);
        muted = false;
    }
}

bool PlayerAudio::isMuted() const
{
    return muted;
}

void PlayerAudio::play()
{
    transportSource.start();
}

void PlayerAudio::repeat() {
    if (!Repeat) {
        readerSource->setLooping(true);
        Repeat = true;
    }
    else {
        Repeat = false;
        transportSource.setPosition(transportSource.getCurrentPosition());
        readerSource->setLooping(false);
    }
}

void PlayerAudio::setPosition(double posInSeconds)
{
    transportSource.setPosition(posInSeconds);
}

double PlayerAudio::getPosition() const
{
    return transportSource.getCurrentPosition();
}

double PlayerAudio::getLength() const
{
    return transportSource.getLengthInSeconds();
}

void PlayerAudio::setGain(float newGain)
{
    transportSource.setGain(newGain);
}

float PlayerAudio::getGain() const
{
    return transportSource.getGain();
}
void PlayerAudio::setSpeed(double speed)
{
    currentspeed = speed;
    resampleSource.setResamplingRatio(speed);
}

void PlayerAudio::pause()
{
    transportSource.stop();


}

void PlayerAudio::fav() {
    if (favorite.contains(currentFile)) {
        favorite.removeFirstMatchingValue(currentFile);
    }
    else {
        favorite.add(currentFile);
    }

    for (int i = 0; i < favorite.size(); i++)
        Myfav.addItem(favorite[i].getFileName(), i + 1);
}

bool PlayerAudio::isPlaying() const
{
    return transportSource.isPlaying();
}

double PlayerAudio::getSpeed() const
{
    return currentspeed;
}
bool PlayerAudio::hasThumbnail() const {
    return thumbnail && thumbnail->getTotalLength() > 0;
}
