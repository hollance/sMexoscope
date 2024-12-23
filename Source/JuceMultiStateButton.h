#pragma once

#include <JuceHeader.h>

class JuceMultiStateButton : public juce::Component
{
public:
    JuceMultiStateButton(const juce::Image& raw, bool vertical, int numstates);

    void paint(juce::Graphics& g) override;

    void mouseDown(const juce::MouseEvent &event) override;
    void mouseUp(const juce::MouseEvent &event) override;

    void setNumModes(int i) { numFrames = i; }

    void setRange(float floor, float ceil, float step);
    void setValue(int x);
    void setValue(float x);

    int getValue() const { return index; }
    float getFloatValue() const { return value; }

private:
    // dimensions of our knob slide
    int width;
    int height;
    int index;
    float min;
    float max;
    float stepsize;
    float value;
    int numFrames;
    bool isVertical;
    bool mouseWasHandled = false;
    juce::Image sequence;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JuceMultiStateButton)
};
