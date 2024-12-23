#include "TextElement.h"

void TextElement::paint(juce::Graphics& g)
{
    g.setColour(juce::Colour(255, 255, 255));
    g.setFont(12.0f);

    juce::Rectangle<int> textRect(0, 0, 30, 12);

    sprintf(text, "%.5f", value);
    g.drawText(text, textRect, juce::Justification::centredTop, false);
}

void TextElement::setValue(float x)
{
    value = x;
    repaint();
}
