#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "WaveDisplay.h"
#include "CustomKnob.h"
#include "TextElement.h"
#include "CustomSlider.h"
#include "JuceMultiStateButton.h"

class SmexoscopeAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SmexoscopeAudioProcessorEditor(SmexoscopeAudioProcessor&);
    ~SmexoscopeAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void updateParameters();

//TODO: suspicious
    double sampleRate = 44100;

private:
    SmexoscopeAudioProcessor& audioProcessor;

    juce::Image background;
    juce::Image heads;
    juce::Image readout;
    juce::Image tealKnob;
    juce::Image retrigMode;
    juce::Image leftRight;
    juce::Image onOff;
    juce::Image sliderNew;

//TODO: why are these pointers?
    CustomKnob* timeKnob;
    CustomKnob* ampKnob;
    CustomKnob* intTrigSpeedKnob;
    CustomKnob* retrigThreshKnob;

    CustomSlider* retrigLevelSlider;

    JuceMultiStateButton* retriggerModeButton;
    JuceMultiStateButton* syncRedrawButton;
    JuceMultiStateButton* freezeButton;
    JuceMultiStateButton* dcKillButton;
    JuceMultiStateButton* channelSelectionButton;

    TextElement* timeText;
    TextElement* ampText;
    TextElement* speedText;
    TextElement* threshText;

    juce::Timer* timepoint;

    CWaveDisplay* smexoscopeDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SmexoscopeAudioProcessorEditor)
};

// special timer used to update gui on a clock
class ExoscopeRedrawTimer : public juce::Timer
{
public:
    ExoscopeRedrawTimer(CWaveDisplay* target, SmexoscopeAudioProcessorEditor* editor)
    {
        smexoscopeDisplay = target;
        this->editor = editor;
    }

    void timerCallback() override
    {
        smexoscopeDisplay->repaint();
        editor->updateParameters();
    }

private:
    CWaveDisplay* smexoscopeDisplay;
    SmexoscopeAudioProcessorEditor* editor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ExoscopeRedrawTimer)
};
