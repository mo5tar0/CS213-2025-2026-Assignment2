#pragma once
#include<JuceHeader.h>
#include "MainComponent.h"

MainComponent::MainComponent()
{
    addAndMakeVisible(player1);
    addAndMakeVisible(player2);
    mixerSource.addInputSource(player1.PlayerAudio1.getAudioSource(), false);
    mixerSource.addInputSource(player2.PlayerAudio1.getAudioSource(), false);

    setSize(600, 400);
    setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
	
	mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
	mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.removeAllInputs();
    mixerSource.releaseResources();
   
	
}

void MainComponent::resized()
{
    player1.setBounds(0, 0, getWidth() / 2, getHeight());
    player2.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight());
}
