#pragma once

#include <JuceHeader.h>

class TextElement : public juce::Component
{
public:
    TextElement() {}
    void paint(juce::Graphics& g) override;
    void setValue(float x);

private:
    char text[256];
    float value = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TextElement)
};
