#pragma once

#include <JuceHeader.h>

class CustomSlider : public juce::Slider
{
public:
    CustomSlider(const juce::Image& raw);

    void paint(juce::Graphics& g) override;

private:
    juce::Image handle;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomSlider)
};
