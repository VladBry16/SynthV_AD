#pragma once
#include <JuceHeader.h>
#include "SynthOSC.h"

class Synth
{
public:
    Synth();
    void prepareToPlay(double sampleRate);
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);

private:
    std::vector<SynthOSC> oscillators;
    double sampleRate;
    void initializeOscillators();
    void render(juce::AudioBuffer<float>& buffer, int startSample, int endSample);
    void handleMidiEvent(const juce::MidiMessage& midiEvent);
    float midiNoteNumberToFrequency(int midiNoteNumber);
    std::vector<float> generateSineWaveTable();
};
