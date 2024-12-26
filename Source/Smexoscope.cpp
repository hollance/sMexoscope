#include "Smexoscope.h"
#include <cmath>

Smexoscope::Smexoscope()
{
    // Zero out the readings.
    for (size_t j = 0; j < peaks.size(); ++j) {
        juce::Point<int> tmp;
        tmp.x = int(j / 2);
        tmp.y = OSC_CENTER - 1;
        peaks[j] = tmp;
        copy[j] = tmp;
    }

    // Default parameter values.
    setParameter(kTriggerSpeed, 0.5f);
    setParameter(kTriggerType, 0.0f);
    setParameter(kTriggerLevel, 0.5f);
    setParameter(kTriggerLimit, 0.5f);
    setParameter(kTimeWindow, 0.75f);
    setParameter(kAmpWindow, 0.5f);
    setParameter(kSyncDraw, 0.0f);
    setParameter(kChannel, 0.0f);
    setParameter(kFreeze, 0.0f);
    setParameter(kDCKill, 0.0f);
}

void Smexoscope::setParameter(int paramIndex, float value)
{
    SAVE[paramIndex] = value;
}

float Smexoscope::getParameter(int paramIndex) const
{
    return SAVE[paramIndex];
}

void Smexoscope::prepareToPlay(double newSampleRate)
{
    sampleRate = newSampleRate;
    reset();
}

void Smexoscope::reset()
{
    index = 0;
    counter = 1.0;
    max = -MAX_FLOAT;
    min = MAX_FLOAT;
    previousSample = 0.0f;
    triggerPhase = 0.0f;
    triggerLimitPhase = 0;
    dcKill = dcFilterTemp = 0.0;
}

void Smexoscope::process(juce::AudioBuffer<float>& buffer)
{
    // In freeze mode, don't process any incoming data.
    if (SAVE[kFreeze] > 0.5) {
        reset();
        return;
    }

    // Read from left or right channel?
    float* samples;
    if (buffer.getNumChannels() > 1) {
        samples = (SAVE[kChannel] > 0.5) ? buffer.getWritePointer(1) : buffer.getWritePointer(0);
    } else {
        samples = buffer.getWritePointer(0);
    }

    const int sampleFrames = buffer.getNumSamples();

    // Linear amplification factor between 0.001 (= -60 dB) and 1000 (+60 dB).
    // Default value is 1.0 or 0 dB gain. Same formula as for the AMP knob text
    // in the editor window.
    const float gain = std::pow(10.0f, SAVE[kAmpWindow] * 6.0f - 3.0f);

    // Linear level value between -1.0f and 1.0f;
    const float triggerLevel = SAVE[kTriggerLevel] * 2.0f - 1.0f;

    // Convert the 0-1 float into one of the kTriggerXXX enum values.
    const int triggerType = int(SAVE[kTriggerType] * kNumTriggerTypes + 0.0001f);

    // This is a number of samples between 1 and 10000.
    const int triggerLimit = int(std::pow(10.0, SAVE[kTriggerLimit] * 4.0));

    // Increment for the phase of the oscillator for the Internal trigger mode.
    // Normally the increment is freq/sample rate. This is why the TRIG SPEED
    // knob multiplies this same value by the sample rate to show the frequency.
    const double triggerSpeed = std::pow(10.0, SAVE[kTriggerSpeed] * 2.5 - 5.0);

    // Number of pixels per sample. Same formula as for the TIME knob text.
    // If the TIME knob is at 30% or higher, counterSpeed will be less than 1.0
    // and a single pixel describes multiple samples. In that case, we do not
    // store individual sample readings but the max/min over that range.
    const double counterSpeed = std::pow(10.0, 1.5 - SAVE[kTimeWindow] * 5.0);

    const double R = 1.0 - 250.0 / sampleRate;
    const bool dcOn = SAVE[kDCKill] > 0.5f;
    
    for (int i = 0; i < sampleFrames; i++) {
        // DC filter. This is a simple high pass filter.
        dcKill = samples[i] - dcFilterTemp + R * dcKill;
        dcFilterTemp = samples[i];

        // Handle denormals. We don't actually need to do this manually here
        // because juce::ScopedNoDenormals will do it automatically.
        if (std::abs(dcKill) < 1e-10f) {
            dcKill = 0.0f;
        }

        // Use the raw input sample if DC killer is turned off.
        float sample = dcOn ? float(dcKill) : samples[i];

        // Apply gain from the AMP knob. Clip to [-1, 1].
        sample = clip(sample * gain, 1.0f);

        // Was the trigger hit?
        bool trigger = false;
        switch (triggerType) {
            case kTriggerFree:
                // Trigger when we've run out of the screen area :-)
                if (index >= OSC_WIDTH) {
                    trigger = true;
                }
                break;
            case kTriggerRising:
                // Trigger on a rising edge.
                if (sample >= triggerLevel && previousSample < triggerLevel) {
                    trigger = true;
                }
                break;
            case kTriggerFalling:
                // Trigger on a falling edge.
                if (sample <= triggerLevel && previousSample > triggerLevel) {
                    trigger = true;
                }
                break;
            case kTriggerInternal:
                // Internal oscillator, nothing fancy.
                triggerPhase += triggerSpeed;
                if (triggerPhase >= 1.0) {
                    triggerPhase -= 1.0;
                    trigger = true;
                }
                break;
        }
        
        // If there's a retrigger, but too fast, kill it.
        // The trigger limit value is determined by the RETRIGGER THRES knob
        // and is expressed as a number of samples.
        triggerLimitPhase++;
        if (trigger && triggerLimitPhase < triggerLimit && triggerType != kTriggerFree && triggerType != kTriggerInternal) {
            trigger = false;
        }

        // @ trigger
        if (trigger) {
            // Zero out the remainder of the peaks array.
            for (size_t j = index * 2; j < OSC_WIDTH * 2; j += 2) {
                peaks[j].y = peaks[j + 1].y = OSC_CENTER - 1;
            }

            // Copy to a buffer for drawing!
            for (size_t j = 0; j < OSC_WIDTH * 2; ++j) {
                copy[j].y = peaks[j].y;
            }

            // Reset everything.
            index = 0;
            counter = 1.0;
            max = -MAX_FLOAT;
            min = MAX_FLOAT;
            triggerLimitPhase = 0;
        }

        // @ sample
        if (sample > max) {
            max = sample;
            lastIsMax = true;
        }
        if (sample < min) {
            min = sample;
            lastIsMax = false;
        }

        // The counter keeps track of how many pixel/sample we have.
        // Essentially we're sampling the signal at a lower rate.
        // The speed is determined by the TIME knob. 
        counter += counterSpeed;
        
        // @ counter
        if (counter >= 1.0) {
            if (index < OSC_WIDTH) {
                // scale here, better than in the graphics thread :-)
                int max_Y = int(OSC_CENTER - max * OSC_CENTER) - 1;
                int min_Y = int(OSC_CENTER - min * OSC_CENTER) - 1;

                // thanks to David @ Plogue for this interesting hint!
                // We store both the min and max sample value that we've seen
                // over the last N samples and will draw a vertical line between
                // these two values. That's why we store 2 points per sample.
                peaks[(index << 1)    ].y = lastIsMax ? min_Y : max_Y;
                peaks[(index << 1) + 1].y = lastIsMax ? max_Y : min_Y;

                index++;
            }
            
            max = -MAX_FLOAT;
            min = MAX_FLOAT;
            counter -= 1.0;
        }

        // Store the previous sample for edge triggers.
        previousSample = sample;
    }
}
