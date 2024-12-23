#include "PluginProcessor.h"
#include "PluginEditor.h"

SmexoscopeAudioProcessor::SmexoscopeAudioProcessor()
    : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
                                      .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
    smexoscopeProcessing = new CSmartelectronixDisplay();
}

SmexoscopeAudioProcessor::~SmexoscopeAudioProcessor()
{
}

const juce::String SmexoscopeAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SmexoscopeAudioProcessor::acceptsMidi() const
{
    return false;
}

bool SmexoscopeAudioProcessor::producesMidi() const
{
    return false;
}

bool SmexoscopeAudioProcessor::isMidiEffect() const
{
    return false;
}

double SmexoscopeAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SmexoscopeAudioProcessor::getNumPrograms()
{
    return 1;
}

int SmexoscopeAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SmexoscopeAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String SmexoscopeAudioProcessor::getProgramName(int index)
{
    return {};
}

void SmexoscopeAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

void SmexoscopeAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;
    smexoscopeProcessing->setSampleRate(this->sampleRate);
}

void SmexoscopeAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SmexoscopeAudioProcessor::setPreferredBusArrangement (bool isInput, int bus, const juce::AudioChannelSet& preferredSet)
{
    // Reject any bus arrangements that are not compatible with your plugin

    const int numChannels = preferredSet.size();

#if JucePlugin_IsMidiEffect
    if (numChannels != 0)
        return false;
#elif JucePlugin_IsSynth
    if (isInput || (numChannels != 1 && numChannels != 2))
        return false;
#else
    if (numChannels != 1 && numChannels != 2)
        return false;

    if (! setPreferredBusArrangement (! isInput, bus, preferredSet))
        return false;
#endif

    return setPreferredBusArrangement (isInput, bus, preferredSet);
}
#endif

void SmexoscopeAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    smexoscopeProcessing->processSub(&buffer,1);
}

bool SmexoscopeAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* SmexoscopeAudioProcessor::createEditor()
{
    return new SmexoscopeAudioProcessorEditor(*this);
}

void SmexoscopeAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    destData.setSize(sizeof(float)*CSmartelectronixDisplay::kNumParams);
    destData.copyFrom(smexoscopeProcessing->getSaveBlock(), 0, sizeof(float)*CSmartelectronixDisplay::kNumParams);
}

void SmexoscopeAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::memcpy(smexoscopeProcessing->getSaveBlock(), data, sizeof(float)*CSmartelectronixDisplay::kNumParams);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SmexoscopeAudioProcessor();
}
