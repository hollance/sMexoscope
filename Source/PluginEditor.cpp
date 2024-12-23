#include "PluginProcessor.h"
#include "PluginEditor.h"

SmexoscopeAudioProcessorEditor::SmexoscopeAudioProcessorEditor(SmexoscopeAudioProcessor& p)
    : juce::AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(825, 300);

    background = juce::ImageCache::getFromMemory(BinaryData::body_png, BinaryData::body_pngSize);
    heads = juce::ImageCache::getFromMemory(BinaryData::heads_png, BinaryData::heads_pngSize);
    readout = juce::ImageCache::getFromMemory(BinaryData::readout_png, BinaryData::readout_pngSize);

    retrigMode = juce::ImageCache::getFromMemory(BinaryData::free_etc_png, BinaryData::free_etc_pngSize);
    leftRight = juce::ImageCache::getFromMemory(BinaryData::lefr_right_png, BinaryData::lefr_right_pngSize);
    onOff = juce::ImageCache::getFromMemory(BinaryData::off_on_png, BinaryData::off_on_pngSize);

    sliderNew = juce::ImageCache::getFromMemory(BinaryData::slider_new_png, BinaryData::slider_new_pngSize);

    tealKnob = juce::ImageCache::getFromMemory(BinaryData::blue_knob1_4_png, BinaryData::blue_knob1_4_pngSize);

//TODO: suspicious
    this->sampleRate = p.sampleRate;

//TODO: audioProcessor.smexoscopeProcessing
    smexoscopeDisplay = new CWaveDisplay(*(new juce::Rectangle<int>(36, 16, 627, 269)), audioProcessor.smexoscopeProcessing, background, heads, readout, this->sampleRate);
    smexoscopeDisplay->setBounds(36, 16, 627, 269);

    // Setup time knob
    timeKnob = new CustomKnob(tealKnob, true);
    timeKnob->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    timeKnob->setRange(0.0, 1.0, 0);
    timeKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    timeKnob->setPopupDisplayEnabled(false, false, this);
    timeKnob->setValue(audioProcessor.smexoscopeProcessing->getParameter(CSmartelectronixDisplay::kTimeWindow));

    // Setup amp knob
    ampKnob = new CustomKnob(tealKnob, true);
    ampKnob->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    ampKnob->setRange(0.0, 1.0, 0);
    ampKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    ampKnob->setPopupDisplayEnabled(false, false, this);
    ampKnob->setValue(audioProcessor.smexoscopeProcessing->getParameter(CSmartelectronixDisplay::kAmpWindow));

    // Setup trigger speed knob
    intTrigSpeedKnob = new CustomKnob(tealKnob, true);
    intTrigSpeedKnob->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    intTrigSpeedKnob->setRange(0.0, 1.0, 0);
    intTrigSpeedKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    intTrigSpeedKnob->setPopupDisplayEnabled(false, false, this);
    intTrigSpeedKnob->setValue(audioProcessor.smexoscopeProcessing->getParameter(CSmartelectronixDisplay::kTriggerSpeed));

    // Setup retrigger threshold knob
    retrigThreshKnob = new CustomKnob(tealKnob, true);
    retrigThreshKnob->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    retrigThreshKnob->setRange(0.0, 1.0, 0);
    retrigThreshKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    retrigThreshKnob->setPopupDisplayEnabled(false, false, this);
    retrigThreshKnob->setValue(audioProcessor.smexoscopeProcessing->getParameter(CSmartelectronixDisplay::kTriggerLimit));

    // Setup retrigger level slider
    retrigLevelSlider = new CustomSlider(sliderNew);
    retrigLevelSlider->setSliderStyle(juce::Slider::LinearBarVertical);
    retrigLevelSlider->setRange(0.0, 1.0, 0);
    retrigLevelSlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    retrigLevelSlider->setPopupDisplayEnabled(false, false, this);
    retrigLevelSlider->setValue(audioProcessor.smexoscopeProcessing->getParameter(CSmartelectronixDisplay::kTriggerLevel));

    // setup retrigger mode button
    retriggerModeButton = new JuceMultiStateButton(retrigMode, true, 5);
    retriggerModeButton->setRange(0.0, 1.0f, 0.25f);
    retriggerModeButton->setNumModes(4);
    retriggerModeButton->setValue(audioProcessor.smexoscopeProcessing->getParameter(CSmartelectronixDisplay::kTriggerType));

    // setup sync redraw button
    syncRedrawButton = new JuceMultiStateButton(onOff, true, 2);
    syncRedrawButton->setRange(0.0, 1.0, 1);
    syncRedrawButton->setValue(audioProcessor.smexoscopeProcessing->getParameter(CSmartelectronixDisplay::kSyncDraw));

    // setup freeze button
    freezeButton = new JuceMultiStateButton(onOff, true, 2);
    freezeButton->setRange(0.0, 1.0, 1);
    freezeButton->setValue(audioProcessor.smexoscopeProcessing->getParameter(CSmartelectronixDisplay::kFreeze));

    // setup dc kill button
    dcKillButton = new JuceMultiStateButton(onOff, true, 2);
    dcKillButton->setRange(0.0, 1.0, 1);
    dcKillButton->setValue(audioProcessor.smexoscopeProcessing->getParameter(CSmartelectronixDisplay::kDCKill));

    // setup channel selection button
    channelSelectionButton = new JuceMultiStateButton(leftRight, true, 2);
    channelSelectionButton->setRange(0.0, 1.0, 1);
    channelSelectionButton->setValue(audioProcessor.smexoscopeProcessing->getParameter(CSmartelectronixDisplay::kChannel));

    // setup text elements
    timeText = new TextElement();
    ampText = new TextElement();
    speedText = new TextElement();
    threshText = new TextElement();

    // Set locations and draw bounds for UI elements
    timeKnob->setBounds(697, 31, 33,33);
    ampKnob->setBounds(762, 31, 33,33);
    intTrigSpeedKnob->setBounds(700, 134, 33,33);
    retrigThreshKnob->setBounds(765, 134, 33,33);

    retrigLevelSlider->setBounds(11, 13,12,276);

    retriggerModeButton->setBounds(718,94, 60,13);
    syncRedrawButton->setBounds(696,222, 41,17);
    freezeButton->setBounds(754,222, 41,17);
    dcKillButton->setBounds(690,259, 41,17);
    channelSelectionButton->setBounds(748,259, 41,17);

    timeText->setBounds(698, 71, 30, 12);
    ampText->setBounds(762, 71, 30, 12);
    speedText->setBounds(700, 174, 30, 12);
    threshText->setBounds(767, 174, 30, 12);

    // add all elements to view and show
    addAndMakeVisible(timeKnob, 2);
    addAndMakeVisible(ampKnob, 2);
    addAndMakeVisible(intTrigSpeedKnob, 2);
    addAndMakeVisible(retrigThreshKnob, 2);

    addAndMakeVisible(retrigLevelSlider, 2);

    addAndMakeVisible(retriggerModeButton, 2);
    addAndMakeVisible(syncRedrawButton, 2);
    addAndMakeVisible(freezeButton, 2);
    addAndMakeVisible(dcKillButton, 2);
    addAndMakeVisible(channelSelectionButton, 2);

    addAndMakeVisible(timeText, 2);
    addAndMakeVisible(ampText, 2);
    addAndMakeVisible(speedText, 2);
    addAndMakeVisible(threshText, 2);

    addAndMakeVisible(smexoscopeDisplay, 1);

    ExoscopeRedrawTimer* oscRepaint = new ExoscopeRedrawTimer(smexoscopeDisplay, this);
    oscRepaint->startTimerHz(30);
    timepoint = oscRepaint;
}

SmexoscopeAudioProcessorEditor::~SmexoscopeAudioProcessorEditor()
{
    delete timepoint;
    delete timeKnob;
    delete ampKnob;
    delete intTrigSpeedKnob;
    delete retrigThreshKnob;
    delete retriggerModeButton;
    delete syncRedrawButton;
    delete freezeButton;
    delete dcKillButton;
    delete channelSelectionButton;
    delete smexoscopeDisplay;
}

void SmexoscopeAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::white);
    g.drawImageAt(background, 0, 0);
}

void SmexoscopeAudioProcessorEditor::resized()
{
}

void SmexoscopeAudioProcessorEditor::updateParameters()
{
    smexoscopeDisplay->setEffectParameter(CSmartelectronixDisplay::kTimeWindow, (float)timeKnob->getValue());
    smexoscopeDisplay->setEffectParameter(CSmartelectronixDisplay::kAmpWindow, (float)ampKnob->getValue());
    smexoscopeDisplay->setEffectParameter(CSmartelectronixDisplay::kTriggerSpeed, (float)intTrigSpeedKnob->getValue());
    smexoscopeDisplay->setEffectParameter(CSmartelectronixDisplay::kTriggerLimit, (float)retrigThreshKnob->getValue());
    smexoscopeDisplay->setEffectParameter(CSmartelectronixDisplay::kTriggerType, retriggerModeButton->getFloatValue());
    smexoscopeDisplay->setEffectParameter(CSmartelectronixDisplay::kSyncDraw, syncRedrawButton->getFloatValue());
    smexoscopeDisplay->setEffectParameter(CSmartelectronixDisplay::kFreeze, freezeButton->getFloatValue());
    smexoscopeDisplay->setEffectParameter(CSmartelectronixDisplay::kDCKill, dcKillButton->getFloatValue());
    smexoscopeDisplay->setEffectParameter(CSmartelectronixDisplay::kChannel, channelSelectionButton->getFloatValue());
    smexoscopeDisplay->setEffectParameter(CSmartelectronixDisplay::kTriggerLevel, retrigLevelSlider->getValue());

    timeText->setValue(pow(10.f, -timeKnob->getValue()*5.f+1.5));
    ampText->setValue(powf(10.f, ampKnob->getValue()*6.f-3.f));
    speedText->setValue(pow(10.0, 2.5*intTrigSpeedKnob->getValue()-5)*sampleRate);
    threshText->setValue(pow(10.f, retrigThreshKnob->getValue()*4.f));
}

