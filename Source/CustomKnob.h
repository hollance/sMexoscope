#pragma once

#include <JuceHeader.h>

/*
  This knob class assumes square knob images, dynamically determining other
  properties about the filmstrip.
 */
class CustomKnob : public juce::Slider
{
public:
    CustomKnob(const juce::Image& raw, bool vertical);

    void paint(juce::Graphics& g) override;

private:
    int dimensions;
    int numFrames;
    bool isVertical;
    juce::Image sequence;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomKnob)
};
