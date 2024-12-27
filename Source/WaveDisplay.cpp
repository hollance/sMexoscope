#include <JuceHeader.h>
#include "WaveDisplay.h"

// Converts linear gain to decibels.
static inline float cf_lin2db(float lin)
{
    if (double(lin) < 9e-51) {
        return -1000.0f;  // prevent invalid operation
    } else {
        return 20.0f * std::log10(lin);
    }
}

// Draws a vertical line that is one pixel wide without anti-aliasing.
static void drawAliasedLine(juce::Graphics& g, int x1, int y1, int y2)
{
    if (y2 > y1) {
        g.fillRect(x1, y1, 1, y2 - y1);
    } else if (y1 > y2) {
        g.fillRect(x1, y2, 1, y1 - y2);
    } else {
        g.fillRect(x1, y1, 1, 1);
    }
}

WaveDisplay::WaveDisplay(Smexoscope& smexoscope, juce::Image heads, juce::Image readout)
    : effect(smexoscope), headsImage(heads), readoutImage(readout)
{
    // This is the magic that selects which smartelectronix head to display
    // in the corner of the wave display.
    juce::Random rng;
    rng.setSeedRandomly();
    headIndex = rng.nextInt(4);

    // Set so crosshairs don't appear by default.
    where.x = -1;
}

void WaveDisplay::mouseDown(const juce::MouseEvent& event)
{
    if (event.mods.isLeftButtonDown() && event.originalComponent == this) {
        where = event.getPosition();
    }
}

void WaveDisplay::mouseDrag(const juce::MouseEvent& event)
{
    if (event.mods.isLeftButtonDown() && event.originalComponent == this) {
        where = event.getPosition();

        // Limit the position to plug-in bounds.
        auto bounds = getLocalBounds();
        if (event.getPosition().x < 0) {
            where.setX(0);
        } else if (event.getPosition().x > bounds.getWidth()) {
            where.setX(bounds.getWidth() - 1);
        }
        if (event.getPosition().y < 0) {
            where.setY(0);
        } else if (event.getPosition().y > bounds.getHeight()) {
            where.setY(bounds.getHeight() - 1);
        }
    }
}

void WaveDisplay::mouseUp(const juce::MouseEvent& event)
{
    // Crosshairs "stick" until you right-click.
    if (event.mods.isRightButtonDown() && event.originalComponent == this) {
        where.x = -1;
    }
}

void WaveDisplay::paint(juce::Graphics& g)
{
    // Note: Offset of the waveform display should be (38, 16) pixels.

    auto bounds = getLocalBounds();

    // Draw a random face because why not.
    g.setOpacity(1.0f);
    g.drawImage(headsImage, 579, 224, 46, 45, 0, headIndex * (headsImage.getHeight() / 4), 46, 45);

    // Draw a grey trigger line when the mode is Rising or Falling.
    int triggerType = int(effect.getParameter(Smexoscope::kTriggerType) * Smexoscope::kNumTriggerTypes + 0.0001f);
    if (triggerType == Smexoscope::kTriggerRising || triggerType == Smexoscope::kTriggerFalling) {
        // The TRIGGER LEVEL is a value between 0.0 and 1.0 where 0.5 is the
        // center. We do `1.0 - level` because a higher level means a smaller
        // y-coordinate.
        int y = 1 + int((1.0f - effect.getParameter(Smexoscope::kTriggerLevel)) * (bounds.getHeight() - 2));
        g.setColour(juce::Colour(229, 229, 229));
        g.drawHorizontalLine(y, 0, bounds.getWidth() - 1);
    }

    // Draw the zero line in orange.
    g.setColour(juce::Colour(179, 111, 56));
    g.drawLine(0, bounds.getHeight()/2 - 1, bounds.getWidth() - 1, bounds.getHeight()/2 - 1);

    // Which array to read from?
    const auto& points = (effect.getParameter(Smexoscope::kSyncDraw) > 0.5f) ? effect.getCopy() : effect.getPeaks();

    // Calculate the number of samples per pixel, which ranges from 0.03162 to
    // 3162. For settings of the TIME knob of 30% or less, there is fewer than
    // one sample per pixel, i.e. we don't have enough readings to fill up the
    // screen. In that case we draw interpolated lines between the sample points.
    double samplesPerPixel = std::pow(10.0, effect.getParameter(Smexoscope::kTimeWindow) * 5.0 - 1.5);
    if (samplesPerPixel < 1.0) {
        g.setColour(juce::Colour(64, 148, 172));  // blue

        double phase = samplesPerPixel;
        double dphase = samplesPerPixel;

        double prevxi = points[0].x;
        double prevyi = points[0].y;

        for (int i = 1; i < bounds.getWidth() - 1; ++i) {
            // Linear interpolation.
            size_t index = (size_t)phase;
            double alpha = phase - (double)index;
            double xi = i;
            double yi = (1.0 - alpha) * points[index * 2].y + alpha * points[(index + 1) * 2].y;

            // This line is rendered with antialiasing, so it looks different
            // (arguably better?) from the original s(M)exoscope.
            g.drawLine(float(prevxi), float(prevyi), float(xi), float(yi), 1.0f);
            prevxi = xi;
            prevyi = yi;

            phase += dphase;
        }
    } else {
        g.setColour(juce::Colour(118, 118, 118));  // grey

        // When we get here, there is one reading for every pixel in the
        // oscilloscope, so draw a vertical line at every location.
        for (size_t i = 0; i < points.size() - 1; ++i) {
            auto p1 = points[i];
            auto p2 = points[i + 1];
            // Don't draw the lines with antialiasing here as that gives nasty
            // semi-transparent blocks instead of solid color when the waveform
            // is dense.
            drawAliasedLine(g, p1.x, p1.y, p2.y);
        }
    }

    // Have a valid mouse location for displaying crosshairs and more info?
    if (where.x != -1) {
        // Draw crosshairs for mouse.
        g.setColour(juce::Colour(10, 10, 10));
        g.drawHorizontalLine(where.y, 0, bounds.getWidth() - 1);
        g.drawVerticalLine(where.x, 0, bounds.getHeight() - 1);

        // Get x and y coordinates scaled for measurement purposes.
        float x = float(where.x) * float(samplesPerPixel);

        // If the gain is 1.0, then the top and bottom of the scope are at 0 dB.
        // If the gain is 10.0, the top/bottom of the scope is 0.1 or -20 dB.
        // If the gain is 100.0, the top/bottom is 0.01 or -40 dB. And so on.
        // Vice versa: If the gain is 0.1, the top/bottom of the scope is 10.0
        // or +10 dB. So we have to divide by the gain to get the y value.
        float gain = std::pow(10.0f, effect.getParameter(Smexoscope::kAmpWindow) * 6.0f - 3.0f);
        float y = (-2.0f * (float(where.y) + 1.0f) / float(OSC_HEIGHT) + 1.0f) / gain;

        char text[64] = { 0 };
        const int lineSize = 10;
        const auto kLeftText = juce::Justification::left;
        const auto sampleRate = effect.getSampleRate();
        juce::Rectangle<int> textRect(512, 2, 105, 10 + lineSize);

        g.drawImageAt(readoutImage, 508, 2);
        g.setColour(juce::Colour(179, 111, 56));
        g.setFont(10.0f);

        snprintf(text, sizeof(text), "y = %.5f", y);
        g.drawText(text, textRect, kLeftText, true);
        textRect.setY(textRect.getY() + lineSize);

        snprintf(text, sizeof(text), "y = %.5f dB", cf_lin2db(std::abs(y)));
        g.drawText(text, textRect, kLeftText, true);
        textRect.setY(textRect.getY() + lineSize*2);

        snprintf(text, sizeof(text), "x = %.2f samples", x);
        g.drawText(text, textRect, kLeftText, true);
        textRect.setY(textRect.getY() + lineSize);

        snprintf(text, sizeof(text), "x = %.5f seconds", x / sampleRate);
        g.drawText(text, textRect, kLeftText, true);
        textRect.setY(textRect.getY() + lineSize);

        snprintf(text, sizeof(text), "x = %.5f ms", 1000.0f * x / sampleRate);
        g.drawText(text, textRect, kLeftText, true);
        textRect.setY(textRect.getY() + lineSize);

        if (x == 0.0f) {
            snprintf(text, sizeof(text), "x = infinite Hz");
        } else {
            snprintf(text, sizeof(text), "x = %.3f Hz", sampleRate / x);
        }
        g.drawText(text, textRect, kLeftText, true);
    }
}
