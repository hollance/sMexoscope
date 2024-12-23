#include "SmartelectronixDisplay.h"
#include "math.h"

#include <sstream>
#include <string>

#include <JuceHeader.h>

inline std::string CSmartelectronixDisplay::to_string(long t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

inline std::string CSmartelectronixDisplay::to_string(float t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

//-----------------------------------------------------------------------------
CSmartelectronixDisplay::CSmartelectronixDisplay()
{
    long j;
    juce::Point<int> tmp;
    for (j = 0; j < OSC_WIDTH * 2; j += 2) {
        tmp.x = j / 2;
        tmp.y = OSC_HEIGHT * 0.5 - 1;
        peaks.push_back(tmp);
        peaks.push_back(tmp);
        
        tmp.x = j / 2;
        tmp.y = OSC_HEIGHT * 0.5 - 1;
        copy.push_back(tmp);
        copy.push_back(tmp);
    }
    
    setParameter(kTriggerSpeed, 0.5f);
    setParameter(kTriggerType, 0.f);
    setParameter(kTriggerLevel, 0.5f);
    setParameter(kTriggerLimit, 0.5f);
    setParameter(kTimeWindow, 0.75f);
    setParameter(kAmpWindow, 0.5f);
    setParameter(kSyncDraw, 0.f);
    setParameter(kChannel, 0.f);
    setParameter(kFreeze, 0.f);
    setParameter(kDCKill, 0.f);
    
    suspend();
}

//-----------------------------------------------------------------------------------------
// theoretically, this algorithm doesn't allocate heap memory, so no cleanup.
CSmartelectronixDisplay::~CSmartelectronixDisplay() {}

//-----------------------------------------------------------------------------------------
void CSmartelectronixDisplay::linkDisplay(juce::Component *target)
{
    displayScope = target;
}
//-----------------------------------------------------------------------------------------
void CSmartelectronixDisplay::setParameter(int index, float value)
{
    SAVE[index] = value;
}
//-----------------------------------------------------------------------------------------
float CSmartelectronixDisplay::getParameter(int index)
{
    return SAVE[index];
}
//-----------------------------------------------------------------------------------------
void CSmartelectronixDisplay::setSampleRate(double sampleRate)
{
    this->sampleRate = sampleRate;
}
//-----------------------------------------------------------------------------------------
double CSmartelectronixDisplay::getSampleRate()
{
    return this->sampleRate;
}
//-----------------------------------------------------------------------------------------
void CSmartelectronixDisplay::processSub(juce::AudioSampleBuffer* buffer, long sampleFrames)
{
    if (SAVE[kFreeze] > 0.5) {
        suspend();
        return;
    }
    
    // get left/right channel pointer, and number of samples in this bunch
    float* samples;
    if (buffer->getNumChannels() > 1)
        samples = SAVE[kChannel] > 0.5 ? buffer->getWritePointer(1) : buffer->getWritePointer(0);
    else
        samples = buffer->getWritePointer(0);
    sampleFrames = buffer->getNumSamples();
    
    // some simple parameter mappings...
    float gain = powf(10.f, SAVE[kAmpWindow] * 6.f - 3.f);
    float triggerLevel = (SAVE[kTriggerLevel] * 2.f - 1.f);
    long triggerType = (long)(SAVE[kTriggerType] * kNumTriggerTypes + 0.0001);
    long triggerLimit = (long)(pow(10.f, SAVE[kTriggerLimit] * 4.f)); // [0=>1 1=>10000
    double triggerSpeed = pow(10.0, 2.5 * SAVE[kTriggerSpeed] - 5.0);
    double counterSpeed = pow(10.f, -SAVE[kTimeWindow] * 5.f + 1.5); // [0=>10 1=>0.001
    double R = 1.0 - 250.0 / sampleRate;
    bool dcOn = SAVE[kDCKill] > 0.5f;
    
    for (long i = 0; i < sampleFrames; i++) {
        // DC filter...
        dcKill = samples[i] - dcFilterTemp + R * dcKill;
        
        dcFilterTemp = samples[i];
        
        if (fabs(dcKill) < 1e-10)
            dcKill = 0.f;
        
        // Gain
        float sample = dcOn ? (float)dcKill : samples[i];
        sample = clip(sample * gain, 1.f);
        
        // triggers
        
        bool trigger = false;
        
        switch (triggerType) {
            case kTriggerInternal: {
                // internal oscillator, nothing fancy
                triggerPhase += triggerSpeed;
                if (triggerPhase >= 1.0) {
                    triggerPhase -= 1.0;
                    trigger = true;
                }
                break;
            }
            case kTriggerRising: {
                // trigger on a rising edge
                if (sample >= triggerLevel && previousSample < triggerLevel)
                    trigger = true;
                break;
            }
            case kTriggerFalling: {
                // trigger on a falling edge
                if (sample <= triggerLevel && previousSample > triggerLevel)
                    trigger = true;
                break;
            }
            case kTriggerFree: {
                // trigger when we've run out of the screen area :-)
                if (index >= OSC_WIDTH)
                    trigger = true;
                break;
            }
        }
        
        // if there's a retrigger, but too fast, kill it
        triggerLimitPhase++;
        if (trigger && triggerLimitPhase < triggerLimit && triggerType != kTriggerFree && triggerType != kTriggerInternal)
            trigger = false;
        
        // @ trigger
        if (trigger) {
            unsigned long j;
            
            // zero peaks after the last one
            for (j = index * 2; j < OSC_WIDTH * 2; j += 2)
                peaks[j].y = peaks[j + 1].y = OSC_HEIGHT * 0.5 - 1;
            
            // copy to a buffer for drawing!
            for (j = 0; j < OSC_WIDTH * 2; j++)
                copy[j].y = peaks[j].y;
            
            // reset everything
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
        
        counter += counterSpeed;
        
        // @ counter
        // the counter keeps track of how many samples/pixel we have
        if (counter >= 1.0) {
            if (index < OSC_WIDTH) {
                // scale here, better than in the graphics thread :-)
                double max_Y = (OSC_HEIGHT * 0.5 - max * 0.5 * OSC_HEIGHT) - 1.0;
                double min_Y = (OSC_HEIGHT * 0.5 - min * 0.5 * OSC_HEIGHT) - 1.0;
                
                // thanks to David @ Plogue for this interesting hint!
                peaks[(index << 1)].y = lastIsMax ? min_Y : max_Y;
                peaks[(index << 1) + 1].y = lastIsMax ? max_Y : min_Y;
                
                index++;
            }
            
            max = -MAX_FLOAT;
            min = MAX_FLOAT;
            
            // counter = counter - (long)counter;
            counter -= 1.0;
        }
        
        // store for edge-triggers !
        previousSample = sample;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------------------
void CSmartelectronixDisplay::suspend()
{
    index = 0;
    counter = 1.0;
    max = -MAX_FLOAT;
    min = MAX_FLOAT;
    previousSample = 0.f;
    triggerPhase = 0.f;
    triggerLimitPhase = 0;
    dcKill = dcFilterTemp = 0.0;
}

//-----------------------------------------------------------------------------------------
void CSmartelectronixDisplay::resume()
{
    index = 0;
    counter = 1.0;
    max = -MAX_FLOAT;
    min = MAX_FLOAT;
    previousSample = 0.f;
    triggerPhase = 0.f;
    triggerLimitPhase = 0;
    dcKill = dcFilterTemp = 0.0;
}


