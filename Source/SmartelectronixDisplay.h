#pragma once

#include "Defines.h"
#include <JuceHeader.h>
#include <sstream>
#include <vector>

class CSmartelectronixDisplay {
public:
    /*
     trigger type : free / rising / falling / internal / external   (this order)
     channel : left / right
     all else : on / off
     */
    
    // VST parameters
    enum {
        kTriggerSpeed, // internal trigger speed, knob
        kTriggerType, // trigger type, selection
        kTriggerLevel, // trigger level, slider
        kTriggerLimit, // retrigger threshold, knob
        kTimeWindow, // X-range, knob
        kAmpWindow, // Y-range, knob
        kSyncDraw, // sync redraw, on/off
        kChannel, // channel selection, left/right
        kFreeze, // freeze display, on/off
        kDCKill, // kill DC, on/off
        kNumParams
    };
    
    // VST elements
    enum {
        kNumPrograms = 0,
        kNumInputChannels = 2,
        kNumOutputChannels = 2, // VST doesn't like 0 output channels ;-)
    };
    
    // trigger types
    enum {
        kTriggerFree = 0,
        kTriggerRising,
        kTriggerFalling,
        kTriggerInternal,
        kNumTriggerTypes
    };
    
    CSmartelectronixDisplay();
    ~CSmartelectronixDisplay();
    
    // the actual algo :-)
    void processSub(juce::AudioSampleBuffer* buffer, long sampleFrames);
    
    // use to set parameters
    void setParameter(int index, float value);
    // use to get parameters
    virtual float getParameter(int index);
    
    //two simple inline tostring functions
    std::string to_string(long t);
    std::string to_string(float t);
    
    // we use these to freeze the scope
    virtual void suspend();
    virtual void resume();
    
    // links this to a Component UI
    void linkDisplay(juce::Component* target);
    
    // set the sample rate from somewhere else
    void setSampleRate(double sampleRate);
    
    // gets the sample rate for use elsewhere
    double getSampleRate();
    
    // gets the paramaters so that they are saved
    float* getSaveBlock() { return SAVE;};
    
    const std::vector<juce::Point<int>>& getPeaks() const { return peaks; }
    const std::vector<juce::Point<int>>& getCopy() const { return copy; }
    
protected:
    std::vector<juce::Point<int>> peaks;
    std::vector<juce::Point<int>> copy;
    
    // index into the peak-array
    unsigned long index;
    
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
    
    // stupid VST parameter save
    float SAVE[kNumParams];
    
    // trigger limiter!
    long triggerLimitPhase;
    
    // dc killer
    double dcKill, dcFilterTemp;
    
    // sample rate, overwritten by host but 44,100 just in case
    double sampleRate = 44100;
    
    // this is the wave display we're linked to
    juce::Component* displayScope;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CSmartelectronixDisplay)
};
