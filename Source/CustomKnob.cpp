#include "CustomKnob.h"

CustomKnob::CustomKnob(const juce::Image& image, bool vertical, double defaultValue) : filmstrip(image), isVertical(vertical)
{
    setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    setRange(0.0, 1.0, 0.0);
    setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    setPopupDisplayEnabled(false, false, this);
    setDoubleClickReturnValue(true, defaultValue);

    if (vertical) {
        dimensions = filmstrip.getWidth();
        numFrames = filmstrip.getHeight() / filmstrip.getWidth();
        jassert(filmstrip.getHeight() % filmstrip.getWidth() == 0);
    } else {
        dimensions = filmstrip.getHeight();
        numFrames = filmstrip.getWidth() / filmstrip.getHeight();
        jassert(filmstrip.getWidth() % filmstrip.getHeight() == 0);
    }
}

void CustomKnob::paint(juce::Graphics& g)
{
    int index = int((getValue() - getMinimum())/(getMaximum() - getMinimum()) * (numFrames - 1));
    if (isVertical) {
        g.drawImage(filmstrip, 0, 0, dimensions, dimensions, 0, index*dimensions, dimensions, dimensions);
    } else {
        g.drawImage(filmstrip, 0, 0, dimensions, dimensions, index*dimensions, 0, dimensions, dimensions);
    }
}
