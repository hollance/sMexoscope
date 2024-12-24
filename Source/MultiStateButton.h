#pragma once

#include <JuceHeader.h>

class MultiStateButton : public juce::Component
{
public:
    MultiStateButton(const juce::Image& filmstrip, bool vertical, int numStates);

    void paint(juce::Graphics& g) override;

    void mouseDown(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;

    void setNumStates(int count);
    void setRange(float floor, float ceil, float step);

    void setIndex(int index);
    int getIndex() const;

    void setValue(float value);
    float getValue() const;

private:
    juce::Image filmstrip;

    int width;        // dimensions of a single frame
    int height;

    int index = 0;    // index of current frame
    float value;      // normalized value (between 0.0 and 1.0)
    float stepSize;   // for converting index to float value

    int numFrames;    // how many images in the filmstrip
    bool isVertical;  // orientation of images in filmstrip

    bool mouseWasHandled = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MultiStateButton)
};
