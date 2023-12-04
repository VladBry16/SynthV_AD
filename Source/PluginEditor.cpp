#include "PluginProcessor.h"
#include "PluginEditor.h"

SynthV_ADAudioProcessorEditor::SynthV_ADAudioProcessorEditor (SynthV_ADAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (400, 300);
}

SynthV_ADAudioProcessorEditor::~SynthV_ADAudioProcessorEditor()
{
}

void SynthV_ADAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void SynthV_ADAudioProcessorEditor::resized()
{
}
