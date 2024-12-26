#pragma once

#include <JuceHeader.h>
#include "Defines.h"
#include "Smexoscope.h"

class WaveDisplay : public juce::Component
{
public:
    WaveDisplay(Smexoscope& effect, juce::Image heads, juce::Image readout);

    void paint(juce::Graphics& g) override;

    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;

private:
    Smexoscope& effect;

    juce::Image headsImage;
    juce::Image readoutImage;

    // Position of the crosshairs.
    juce::Point<int> where;

    // Which head image to draw (randomly chosen).
    int headIndex = 0;

//TODO was passed in through ctor, use better method
    double sampleRate = 44100.0;

    char text[256] = { 0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveDisplay)
};
