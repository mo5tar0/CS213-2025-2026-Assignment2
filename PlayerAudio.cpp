#include "PlayerAudio.h"

PlayerAudio::PlayerAudio(): resampleSource(&transportSource, false) 
{
    formatManager.registerBasicFormats();
}

PlayerAudio::~PlayerAudio()
{
    releaseResources() ;
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

bool PlayerAudio::loadFile(const juce::File& file)
{
        if (auto* reader = formatManager.createReaderFor(file))
        {
           
            transportSource.stop();
            transportSource.setSource(nullptr);
            readerSource.reset();

           
            readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

          
            transportSource.setSource(readerSource.get(),
                0,
                nullptr,
                reader->sampleRate);
            setSpeed(currentspeed);
            return true;
        }
        return false;
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

void PlayerAudio::stop()
{
    transportSource.stop();
    transportSource.setPosition(0.0);
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

double PlayerAudio::getSpeed() const
{
    return currentspeed;
}

