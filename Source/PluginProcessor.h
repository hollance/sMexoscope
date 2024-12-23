#pragma once

#include <JuceHeader.h>
#include "SmartelectronixDisplay.h"

class SmexoscopeAudioProcessor  : public juce::AudioProcessor
{
public:
    SmexoscopeAudioProcessor();
    ~SmexoscopeAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

//TODO: change this
#ifndef JucePlugin_PreferredChannelConfigurations
    bool setPreferredBusArrangement(bool isInput, int bus, const juce::AudioChannelSet& preferredSet);
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

//TODO: private?
    CSmartelectronixDisplay* smexoscopeProcessing;

//TODO: suspicious
    double sampleRate = 44100;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SmexoscopeAudioProcessor)
};
