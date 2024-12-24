#pragma once

#include <JuceHeader.h>
#include "Defines.h"

/*
  This was CSmartelectronixDisplay in the original code, but that class also
  functioned as the main plug-in object.
*/
class Smexoscope
{
public:
    // Parameters
    enum
    {
        kTriggerSpeed,  // internal trigger speed, knob
        kTriggerType,   // trigger type, selection
        kTriggerLevel,  // trigger level, slider
        kTriggerLimit,  // retrigger threshold, knob
        kTimeWindow,    // X-range, knob
        kAmpWindow,     // Y-range, knob
        kSyncDraw,      // sync redraw, on/off
        kChannel,       // channel selection, left/right
        kFreeze,        // freeze display, on/off
        kDCKill,        // kill DC, on/off
        kNumParams
    };

    // Trigger types
    enum
    {
        kTriggerFree = 0,
        //kTriggerTempo,
        kTriggerRising,
        kTriggerFalling,
        kTriggerInternal,
        //kTriggerExternal,
        kNumTriggerTypes
    };

    Smexoscope();

    void prepareToPlay(double sampleRate);
    void reset();

    void process(juce::AudioBuffer<float>& buffer);

    void setParameter(int index, float value);
    float getParameter(int index) const;

    float* getSaveBlock() { return SAVE; }
    constexpr size_t getSaveBlockSize() { return sizeof(float) * kNumParams; }

    double getSampleRate() const { return sampleRate; }

    const std::vector<juce::Point<int>>& getPeaks() const { return peaks; }
    const std::vector<juce::Point<int>>& getCopy() const { return copy; }

protected:
    std::vector<juce::Point<int>> peaks;
    std::vector<juce::Point<int>> copy;

    // index into the peak-array
    size_t index;

    // counter which is used to set the amount of samples / pixel
    double counter;

    // max/min peak in this block
    float max, min, maxR, minR;

    // the last peak we encountered was a maximum!
    bool lastIsMax;

    // the previous sample (for edge-triggers)
    float previousSample;

    // the internal trigger oscillator
    double triggerPhase;

    // This array holds the parameter values.
    float SAVE[kNumParams];

    // trigger limiter!
    long triggerLimitPhase;

    // dc killer
    double dcKill, dcFilterTemp;

//TODO: only used by R so can move into prepareToPlay (see also editor)
    // sample rate
    double sampleRate = 44100.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Smexoscope)
};
