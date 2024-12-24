#include "PluginProcessor.h"
#include "PluginEditor.h"

SmexoscopeAudioProcessorEditor::SmexoscopeAudioProcessorEditor(SmexoscopeAudioProcessor& p) :
    juce::AudioProcessorEditor(&p),
    audioProcessor(p),
    waveDisplay(audioProcessor.smexoscope, headsImage, readoutImage)
{
//TODO: handle this better
    timeKnob.setValue(audioProcessor.smexoscope.getParameter(Smexoscope::kTimeWindow));
    ampKnob.setValue(audioProcessor.smexoscope.getParameter(Smexoscope::kAmpWindow));
    intTrigSpeedKnob.setValue(audioProcessor.smexoscope.getParameter(Smexoscope::kTriggerSpeed));
    retrigThreshKnob.setValue(audioProcessor.smexoscope.getParameter(Smexoscope::kTriggerLimit));
    retrigLevelSlider.setValue(audioProcessor.smexoscope.getParameter(Smexoscope::kTriggerLevel));
    retriggerModeButton.setValue(audioProcessor.smexoscope.getParameter(Smexoscope::kTriggerType));
    syncRedrawButton.setValue(audioProcessor.smexoscope.getParameter(Smexoscope::kSyncDraw));
    freezeButton.setValue(audioProcessor.smexoscope.getParameter(Smexoscope::kFreeze));
    dcKillButton.setValue(audioProcessor.smexoscope.getParameter(Smexoscope::kDCKill));
    channelSelectionButton.setValue(audioProcessor.smexoscope.getParameter(Smexoscope::kChannel));

    // Filmstrip has 5 images but External triggering mode is not implemented.
    retriggerModeButton.setNumStates(4);

    addAndMakeVisible(timeKnob, 2);
    addAndMakeVisible(timeText, 2);
    addAndMakeVisible(ampKnob, 2);
    addAndMakeVisible(ampText, 2);
    addAndMakeVisible(intTrigSpeedKnob, 2);
    addAndMakeVisible(speedText, 2);
    addAndMakeVisible(retrigThreshKnob, 2);
    addAndMakeVisible(threshText, 2);
    addAndMakeVisible(retrigLevelSlider, 2);
    addAndMakeVisible(retriggerModeButton, 2);
    addAndMakeVisible(syncRedrawButton, 2);
    addAndMakeVisible(freezeButton, 2);
    addAndMakeVisible(dcKillButton, 2);
    addAndMakeVisible(channelSelectionButton, 2);
    addAndMakeVisible(waveDisplay, 1);

    setSize(825, 300);
    startTimerHz(30);
}

void SmexoscopeAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::white);
    g.drawImageAt(backgroundImage, 0, 0);
}

void SmexoscopeAudioProcessorEditor::resized()
{
    waveDisplay.setBounds(36, 16, 627, 269);

    timeKnob.setBounds(697, 31, 33, 33);
    timeText.setBounds(698, 71, 30, 12);

    ampKnob.setBounds(762, 31, 33, 33);
    ampText.setBounds(762, 71, 30, 12);

    retriggerModeButton.setBounds(718, 94, 60, 13);
    retrigLevelSlider.setBounds(11, 13, 12, 276);

    intTrigSpeedKnob.setBounds(700, 134, 33, 33);
    speedText.setBounds(700, 174, 30, 12);

    retrigThreshKnob.setBounds(765, 134, 33, 33);
    threshText.setBounds(767, 174, 30, 12);

    syncRedrawButton.setBounds(696, 222, 41, 17);
    freezeButton.setBounds(754, 222, 41, 17);
    dcKillButton.setBounds(690, 259, 41, 17);
    channelSelectionButton.setBounds(748, 259, 41, 17);
}

void SmexoscopeAudioProcessorEditor::timerCallback()
{
    waveDisplay.repaint();
    updateParameters();
}

void SmexoscopeAudioProcessorEditor::updateParameters()
{
    waveDisplay.setEffectParameter(Smexoscope::kTimeWindow, float(timeKnob.getValue()));
    waveDisplay.setEffectParameter(Smexoscope::kAmpWindow, float(ampKnob.getValue()));
    waveDisplay.setEffectParameter(Smexoscope::kTriggerSpeed, float(intTrigSpeedKnob.getValue()));
    waveDisplay.setEffectParameter(Smexoscope::kTriggerLimit, float(retrigThreshKnob.getValue()));
    waveDisplay.setEffectParameter(Smexoscope::kTriggerType, retriggerModeButton.getValue());
    waveDisplay.setEffectParameter(Smexoscope::kSyncDraw, syncRedrawButton.getValue());
    waveDisplay.setEffectParameter(Smexoscope::kFreeze, freezeButton.getValue());
    waveDisplay.setEffectParameter(Smexoscope::kDCKill, dcKillButton.getValue());
    waveDisplay.setEffectParameter(Smexoscope::kChannel, channelSelectionButton.getValue());
    waveDisplay.setEffectParameter(Smexoscope::kTriggerLevel, float(retrigLevelSlider.getValue()));

//TODO: not thread safe
    double sampleRate = audioProcessor.smexoscope.getSampleRate();

//TODO: fix warnings
    timeText.setValue(pow(10.f, -timeKnob.getValue()*5.f+1.5));
    ampText.setValue(powf(10.f, ampKnob.getValue()*6.f-3.f));
    speedText.setValue(pow(10.0, 2.5*intTrigSpeedKnob.getValue()-5)*sampleRate);
    threshText.setValue(pow(10.f, retrigThreshKnob.getValue()*4.f));
}
