#include "MultiStateButton.h"

MultiStateButton::MultiStateButton(const juce::Image& image, bool vertical, int numStates)
    : filmstrip(image), isVertical(vertical)
{
    setNumStates(numStates);

    if (isVertical) {
        width = filmstrip.getWidth();
        height = filmstrip.getHeight() / numFrames;
        jassert(filmstrip.getHeight() % numFrames == 0);
    } else {
        height = filmstrip.getHeight();
        width = filmstrip.getWidth() / numFrames;
        jassert(filmstrip.getWidth() % numFrames == 0);
    }
}

void MultiStateButton::paint(juce::Graphics& g)
{
    if (isVertical) {
        g.drawImage(filmstrip, 0, 0, width, height, 0, index*height, width, height);
    } else {
        g.drawImage(filmstrip, 0, 0, width, height, index*width, 0, width, height);
    }
}

void MultiStateButton::mouseDown(const juce::MouseEvent&)
{
    if (!mouseWasHandled) {
        setIndex(++index);
        mouseWasHandled = true;
        repaint();
    }
}

void MultiStateButton::mouseUp(const juce::MouseEvent& event)
{
    if (event.mods.isLeftButtonDown() && event.originalComponent == this) {
        mouseWasHandled = false;
    }
}

void MultiStateButton::setNumStates(int count)
{
    numFrames = count;
    if (numFrames == 2) {
        stepSize = 1.0f;
    } else {
        stepSize = 1.0f / float(numFrames);
    }
}

void MultiStateButton::setIndex(int x)
{
    if (x >= numFrames) {
        index = 0;
    } else if (x < 0) {
        index = 0;
    } else {
        index = x;
    }
    value = index * stepSize;
}

int MultiStateButton::getIndex() const
{
    return index;
}

void MultiStateButton::setValue(float x)
{
    value = x;
    index = int(x / stepSize);
}

float MultiStateButton::getValue() const
{
    return value;
}
