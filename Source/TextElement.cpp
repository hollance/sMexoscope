#include "TextElement.h"

void TextElement::paint(juce::Graphics& g)
{
    g.setColour(juce::Colour(255, 255, 255));
    g.setFont(12.0f);

    juce::Rectangle<int> textRect(0, 0, 34, 12);
    juce::String text;
    if (value < 1000.0f) {
        text = juce::String(value, 3);
    } else {
        text = juce::String(int(value));
    }
    g.drawText(text, textRect, juce::Justification::centredTop, false);
}

void TextElement::setValue(float x)
{
    value = x;
    repaint();
}
