#pragma once

#include <JuceHeader.h>

/*
  This knob class assumes square knob images, dynamically determining other
  properties about the filmstrip.
 */
class CustomKnob : public juce::Slider
{
public:
    CustomKnob(const juce::Image& filmstrip, bool vertical, double defaultValue);

    void paint(juce::Graphics& g) override;

private:
    juce::Image filmstrip;

    int dimensions;   // width/height of a single frame
    int numFrames;    // how many images in the filmstrip
    bool isVertical;  // orientation of images in filmstrip

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomKnob)
};
