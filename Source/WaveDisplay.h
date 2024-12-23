#pragma once

#include "Defines.h"
#include <JuceHeader.h>
#include "SmartelectronixDisplay.h"

class CWaveDisplay : public juce::Component{
public:
    CWaveDisplay(juce::Rectangle<int> size, CSmartelectronixDisplay* effect, juce::Image back, juce::Image heads, juce::Image readout, double sampleRate);

    virtual ~CWaveDisplay();
    
    virtual void paint(juce::Graphics& g) override;
    
    // custom draw function to draw an aliased line
    void drawAliasedLine(juce::Graphics& g, int x1, int y1, int x2, int y2);
    
    virtual void mouseDown(const juce::MouseEvent &event) override;
    virtual void mouseDrag(const juce::MouseEvent &event) override;
    virtual void mouseUp(const juce::MouseEvent &event) override;
    void setEffectParameter(int index, float value);
    
    int kLeftText = juce::Justification::left;
    
protected:
    juce::Point<int> where;
    juce::Rectangle<int> size;
    
    CSmartelectronixDisplay* effect;
    
    juce::Image back;
    juce::Image heads;
    juce::Image readout;
    
    double sampleRate = 44100;
    
    unsigned char display;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CWaveDisplay)
};
