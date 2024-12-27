#include "CustomSlider.h"

CustomSlider::CustomSlider(const juce::Image& image) : handleImage(image)
{
    setSliderStyle(juce::Slider::LinearBarVertical);
    setRange(0.0, 1.0, 0.0);
    setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    setPopupDisplayEnabled(false, false, this);
    setDoubleClickReturnValue(true, 0.5);
}

void CustomSlider::paint(juce::Graphics& g)
{
    int offset = 266 - int(getValue() * 266);
    int width = handleImage.getWidth();
    int height = handleImage.getHeight();
    g.drawImage(handleImage, 0, offset, width, height, 0, 0, width, height);
}
