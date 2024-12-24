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

    void setEffectParameter(int index, float value);

private:
    // custom draw function to draw an aliased line
    void drawAliasedLine(juce::Graphics& g, int x1, int y1, int x2, int y2);

    Smexoscope& effect;

    juce::Point<int> where;
    juce::Rectangle<int> size;

    juce::Image heads;
    juce::Image readout;

//TODO was passed in through ctor, use better method
    double sampleRate = 44100.0;

    unsigned char display;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveDisplay)
};
