/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ADSRDisplay.h"
#include "CustomLookAndFeel.h"
#include "FilterDisplay.h"

//==============================================================================
/**
*/
class SynthV_ADAudioProcessorEditor  : public juce::AudioProcessorEditor,
    public juce::Slider::Listener, public juce::Button::Listener
{
public:
    SynthV_ADAudioProcessorEditor (SynthV_ADAudioProcessor&);
    ~SynthV_ADAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* button) override;
    void selectWave();

private:

    std::unique_ptr<CustomLookAndFeel> customLookAndFeel;

    SynthV_ADAudioProcessor& audioProcessor;

    juce::Image sin_on = juce::ImageCache::getFromMemory(BinaryData::sin_on_png, BinaryData::sin_on_pngSize);
    juce::Image sin_off = juce::ImageCache::getFromMemory(BinaryData::sin_off_png, BinaryData::saw_off_pngSize);
    juce::Image saw_on = juce::ImageCache::getFromMemory(BinaryData::swa_on_png, BinaryData::swa_on_pngSize);
    juce::Image saw_off = juce::ImageCache::getFromMemory(BinaryData::saw_off_png, BinaryData::saw_off_pngSize);
    juce::Image square_on = juce::ImageCache::getFromMemory(BinaryData::square_on_png, BinaryData::square_on_pngSize);
    juce::Image square_off = juce::ImageCache::getFromMemory(BinaryData::square_off_png, BinaryData::square_off_pngSize);
    juce::Image triangle_on = juce::ImageCache::getFromMemory(BinaryData::triangle_on_png, BinaryData::triangle_on_pngSize);
    juce::Image triangle_off = juce::ImageCache::getFromMemory(BinaryData::triangle_off_png, BinaryData::triangle_off_pngSize);
    juce::Image sin_select = juce::ImageCache::getFromMemory(BinaryData::sin_select_png, BinaryData::sin_select_pngSize);
    juce::Image saw_select = juce::ImageCache::getFromMemory(BinaryData::saw_select_png, BinaryData::saw_select_pngSize);
    juce::Image square_select = juce::ImageCache::getFromMemory(BinaryData::square_select_png, BinaryData::square_select_pngSize);
    juce::Image triangle_select = juce::ImageCache::getFromMemory(BinaryData::triangle_select_png, BinaryData::triangle_select_pngSize);

    juce::ImageButton sineWaveButton;
    juce::ImageButton sawWaveButton;
    juce::ImageButton squareWaveButton;
    juce::ImageButton triangleWaveButton;
    
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;
    ADSRDisplay adsrDisplay;
    FilterDisplay filterDisplay;

    juce::Slider highPassFreqSlider;
    juce::Slider lowPassFreqSlider;

    juce::Slider depthSlider;
    juce::Slider frequencySlider;

    juce::Slider volumeSlider;
    
    juce::Label attackLabel{ "A - Attack", "A - Attack" };
    juce::Label decayLabel{ "D - Decay", "D - Decay" };
    juce::Label sustainLabel{ "S - Sustain", "S - Sustain" };
    juce::Label releaseLabel{ "R - Release", "R - Release" };
    juce::Label volumeLabel{ "Volume", "Volume" };
    juce::Label highPassLabel{ "HighPass", "HighPass" };
    juce::Label lowPassLabel{ "LowPass", "LowPass" };

    juce::MidiKeyboardComponent keyboardComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthV_ADAudioProcessorEditor)
};
