#include "CustomSlider.h"

CustomSlider::CustomSlider(const juce::Image& raw) : handle(raw)
{
    // do nothing
}

void CustomSlider::paint(juce::Graphics& g)
{
    int offset = 267 - int(getValue() * 267);
    g.drawImage(handle, 0, offset, handle.getWidth(), handle.getHeight(), 0, 0, handle.getWidth(), handle.getHeight());
}
