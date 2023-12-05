#pragma once
#include <JuceHeader.h>
#include "Synth.h"

class ADSRDisplay : public juce::Component
{
public:
    ADSRDisplay(Synth& synth);
    void paint(juce::Graphics& g) override;

private:
    Synth& synth;
};
