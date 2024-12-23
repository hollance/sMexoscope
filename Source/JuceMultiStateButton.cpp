#include "JuceMultiStateButton.h"

JuceMultiStateButton::JuceMultiStateButton(const juce::Image& raw, bool vertical, int numstates)
    : isVertical(vertical), sequence(raw)
{
    if (isVertical) {
        width = sequence.getWidth();
        numFrames = numstates;
        if (sequence.getHeight() % numFrames != 0) {
            // error, wrong image dimensions.
        } else {
            height = sequence.getHeight()/numFrames;
        }
    } else {
        height = sequence.getHeight();
        numFrames = numstates;
        if (sequence.getWidth() % numFrames != 0) {
            // error, wrong image dimensions.
        } else {
            width = sequence.getWidth()/numFrames;
        }
    }
}

void JuceMultiStateButton::paint(juce::Graphics& g)
{
    if (isVertical) {
        g.drawImage(sequence, 0, 0, width, height, 0, index*height, width, height);
    } else {
        g.drawImage(sequence, 0, 0, width, height, index*width, 0, width, height);
    }
}

void JuceMultiStateButton::mouseDown(const juce::MouseEvent &event)
{
    if (!mouseWasHandled) {
        setValue(++index);
        mouseWasHandled = true;
        repaint();
    }
}

void JuceMultiStateButton::mouseUp(const juce::MouseEvent &event)
{
    if (event.mods.isLeftButtonDown() && event.originalComponent == this) {
        mouseWasHandled = false;
    }
}

void JuceMultiStateButton::setRange(float floor, float ceil, float step)
{
    min = floor;
    max = ceil;
    stepsize = step;
}

void JuceMultiStateButton::setValue(int x)
{
    if (x >= numFrames)
        index = 0;
    else if (x < 0)
        index = 0;
    else
        index = x;
    value = index*stepsize;
}

void JuceMultiStateButton::setValue(float x)
{
    value = x;
    index = x/stepsize;
}
