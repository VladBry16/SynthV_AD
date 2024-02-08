#pragma once
#include <JuceHeader.h>
#include "Synth.h"

class FilterDisplay : public juce::Component
{
public:
    FilterDisplay(Synth& synth);
    void paint(juce::Graphics& g) override;

private:
    Synth& synth;
};