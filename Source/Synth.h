#pragma once
#include <JuceHeader.h>
#include "SynthOSC.h"

class Synth
{
public:
    Synth();
    void prepareToPlay(double sampleRate);
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    void setAttack(float attack);
    void setDecay(float decay);
    void setSustain(float sustain);
    void setRelease(float release);

    float getAttack() const;
    float getDecay() const;
    float getSustain() const;
    float getRelease() const;

private:
    std::vector<SynthOSC> oscillators;
    double sampleRate;
    void initializeOscillators();
    void render(juce::AudioBuffer<float>& buffer, int startSample, int endSample);
    void handleMidiEvent(const juce::MidiMessage& midiEvent);
    float midiNoteNumberToFrequency(int midiNoteNumber);
    std::vector<float> generateSineWaveTable();
};
