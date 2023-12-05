#include "PluginProcessor.h"
#include "PluginEditor.h"

SynthV_ADAudioProcessor::SynthV_ADAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

SynthV_ADAudioProcessor::~SynthV_ADAudioProcessor()
{
}

//==============================================================================
const juce::String SynthV_ADAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SynthV_ADAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SynthV_ADAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SynthV_ADAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SynthV_ADAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SynthV_ADAudioProcessor::getNumPrograms()
{
    return 1;
}

int SynthV_ADAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SynthV_ADAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SynthV_ADAudioProcessor::getProgramName (int index)
{
    return {};
}

void SynthV_ADAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SynthV_ADAudioProcessor::prepareToPlay (double sampleRate, int)
{
    synth.prepareToPlay(sampleRate);
}

void SynthV_ADAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynthV_ADAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SynthV_ADAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    buffer.clear();
    
    synth.processBlock(buffer, midiMessages);
}

bool SynthV_ADAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* SynthV_ADAudioProcessor::createEditor()
{
    return new SynthV_ADAudioProcessorEditor (*this);
}

//==============================================================================
void SynthV_ADAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
}

void SynthV_ADAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

Synth& SynthV_ADAudioProcessor::getSynth()
{
    return synth;
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynthV_ADAudioProcessor();
}
