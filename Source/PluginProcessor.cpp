#include "PluginProcessor.h"
#include "PluginEditor.h"

SmexoscopeAudioProcessor::SmexoscopeAudioProcessor()
    : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
                                      .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
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

void SmexoscopeAudioProcessor::setCurrentProgram(int)
{
}

const juce::String SmexoscopeAudioProcessor::getProgramName(int)
{
    return {};
}

void SmexoscopeAudioProcessor::changeProgramName(int, const juce::String&)
{
}

void SmexoscopeAudioProcessor::prepareToPlay(double sampleRate, int)
{
    smexoscope.prepareToPlay(sampleRate);
}

void SmexoscopeAudioProcessor::releaseResources()
{
}

bool SmexoscopeAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
    &&  layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo()) {
        return false;
    }
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet()) {
        return false;
    }
    return true;
}

void SmexoscopeAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    smexoscope.process(buffer);
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
    destData.setSize(smexoscope.getSaveBlockSize());
    destData.copyFrom(smexoscope.getSaveBlock(), 0, smexoscope.getSaveBlockSize());
}

void SmexoscopeAudioProcessor::setStateInformation(const void* data, int)
{
    std::memcpy(smexoscope.getSaveBlock(), data, smexoscope.getSaveBlockSize());
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SmexoscopeAudioProcessor();
}
