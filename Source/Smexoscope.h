#pragma once

#include <JuceHeader.h>
#include "Defines.h"

/*
  This was CSmartelectronixDisplay in the original code, but there the class
  also functioned as the main plug-in object. Here it only captures the audio.
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

    // We store two readings for every pixel position in the oscilloscope,
    // which is why there are `OSC_WIDTH * 2` elements in the array.
    using PeaksArray = std::array<juce::Point<int>, OSC_WIDTH * 2>;

    const PeaksArray& getPeaks() const { return peaks; }
    const PeaksArray& getCopy() const { return copy; }

protected:
    // Array containing the waveform readings. The `copy` array is used
    // for Sync Redraw mode and is only updated when the trigger is hit.
    PeaksArray peaks;
    PeaksArray copy;

    // Current write position into the peaks array.
    size_t index;

    // How often we take a reading, i.e. the number of samples-per-pixel.
    double counter;

    // Largest and smallest samples seen in the most recent block.
    float max, min;

    // Whether the last peak we encountered was a maximum or minimum.
    bool lastIsMax;

    // The previous sample, for edge triggers.
    float previousSample;

    // Oscillator used for Internal trigger mode.
    double triggerPhase;

    // Counter that limits how soon the trigger may happen again.
    int triggerLimitPhase;

    // DC killer filter state.
    double dcKill, dcFilterTemp;

    // This array holds the parameter values. They're stored in an array so
    // they can be loaded & saved easily by copying (into) the whole array.
    float SAVE[kNumParams];

    // Sample rate that was passed into `prepareToPlay`.
    double sampleRate = 44100.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Smexoscope)
};
