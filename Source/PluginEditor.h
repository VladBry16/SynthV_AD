/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ADSRDisplay.h"

//==============================================================================
/**
*/
class SynthV_ADAudioProcessorEditor  : public juce::AudioProcessorEditor,
    public juce::Slider::Listener
{
public:
    SynthV_ADAudioProcessorEditor (SynthV_ADAudioProcessor&);
    ~SynthV_ADAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;

private:
    
    SynthV_ADAudioProcessor& audioProcessor;

    
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;
    ADSRDisplay adsrDisplay;

    
    juce::Label attackLabel{ "A - Attack", "A - Attack" };
    juce::Label decayLabel{ "D - Decay", "D - Decay" };
    juce::Label sustainLabel{ "S - Sustain", "S - Sustain" };
    juce::Label releaseLabel{ "R - Release", "R - Release" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthV_ADAudioProcessorEditor)
};
