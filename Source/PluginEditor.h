#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "WaveDisplay.h"
#include "CustomKnob.h"
#include "TextElement.h"
#include "CustomSlider.h"
#include "JuceMultiStateButton.h"

class SmexoscopeAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    SmexoscopeAudioProcessorEditor(SmexoscopeAudioProcessor&);
    ~SmexoscopeAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

//TODO: suspicious
    double sampleRate = 44100;

private:
    void timerCallback() override;
    void updateParameters();

    SmexoscopeAudioProcessor& audioProcessor;

    juce::Image backgroundImage { juce::ImageCache::getFromMemory(BinaryData::body_png, BinaryData::body_pngSize) };
    juce::Image headsImage { juce::ImageCache::getFromMemory(BinaryData::heads_png, BinaryData::heads_pngSize) };
    juce::Image readoutImage { juce::ImageCache::getFromMemory(BinaryData::readout_png, BinaryData::readout_pngSize) };
    juce::Image knobImage { juce::ImageCache::getFromMemory(BinaryData::blue_knob1_4_png, BinaryData::blue_knob1_4_pngSize) };
    juce::Image retrigModeImage { juce::ImageCache::getFromMemory(BinaryData::free_etc_png, BinaryData::free_etc_pngSize) };
    juce::Image leftRightImage { juce::ImageCache::getFromMemory(BinaryData::lefr_right_png, BinaryData::lefr_right_pngSize) };
    juce::Image onOffImage { juce::ImageCache::getFromMemory(BinaryData::off_on_png, BinaryData::off_on_pngSize) };
    juce::Image sliderImage { juce::ImageCache::getFromMemory(BinaryData::slider_new_png, BinaryData::slider_new_pngSize) };

    CustomKnob timeKnob { knobImage, true };
    CustomKnob ampKnob { knobImage, true };
    CustomKnob intTrigSpeedKnob { knobImage, true };
    CustomKnob retrigThreshKnob { knobImage, true };

    CustomSlider retrigLevelSlider { sliderImage };

//TODO rename to MultiStateButton
    JuceMultiStateButton retriggerModeButton { retrigModeImage, true, 5 };
    JuceMultiStateButton syncRedrawButton { onOffImage, true, 2 };
    JuceMultiStateButton freezeButton { onOffImage, true, 2 };
    JuceMultiStateButton dcKillButton { onOffImage, true, 2 };
    JuceMultiStateButton channelSelectionButton { leftRightImage, true, 2 };

    TextElement timeText;
    TextElement ampText;
    TextElement speedText;
    TextElement threshText;

    CWaveDisplay waveDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SmexoscopeAudioProcessorEditor)
};
