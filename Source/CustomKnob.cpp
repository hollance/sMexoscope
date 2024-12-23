#include "CustomKnob.h"

CustomKnob::CustomKnob(const juce::Image& raw, bool vertical) : isVertical(vertical), sequence(raw)
{
    if (isVertical) {
        dimensions = sequence.getWidth();
        numFrames = sequence.getHeight() / sequence.getWidth();
        if (sequence.getHeight() % sequence.getWidth() != 0) {
            // error, wrong image dimensions.
        }
    } else {
        dimensions = sequence.getHeight();
        numFrames = sequence.getWidth() / sequence.getHeight();
        if (sequence.getWidth() % sequence.getHeight() != 0) {
            // error, wrong image dimensions.
        }
    }
}

void CustomKnob::paint(juce::Graphics& g)
{
    int index = (getValue()-getMinimum())/(getMaximum()-getMinimum())*(numFrames-1);
    if (isVertical) {
        g.drawImage(sequence, 0,0, dimensions, dimensions, 0, index*dimensions, dimensions, dimensions);
    } else {
        g.drawImage(sequence, 0,0, dimensions, dimensions, index*dimensions, 0, dimensions, dimensions);
    }
}
