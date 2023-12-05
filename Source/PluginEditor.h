/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SynthV_ADAudioProcessorEditor  : public juce::AudioProcessorEditor,
    public juce::Slider::Listener
{
public:
    SynthV_ADAudioProcessorEditor (SynthV_ADAudioProcessor&);
    ~SynthV_ADAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SynthV_ADAudioProcessor& audioProcessor;

    // Слайдеры для каждого параметра ADSR
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;
    SynthOSC oscillator; // Ваш осциллятор

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthV_ADAudioProcessorEditor)
};
