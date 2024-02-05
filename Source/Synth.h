#pragma once
#include <JuceHeader.h>
#include "SynthOSC.h"

enum class Waveform
{
    Sine,
    Saw,
    Square,
    Triangle
};

class Synth : public juce::MidiKeyboardStateListener
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

    void setWaveTable(Waveform waveform);

    void setModulationDepth(float newDepth);
    void setModulationFrequency(float newFrequency);

    void setVolume(float newVolume);
    float getVolume() const;

    void setX(int x);
    int getX() const;

    juce::MidiKeyboardState& getKeyboardState() { return keyboardState; }

    void handleNoteOn(juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;

private:
    std::vector<SynthOSC> oscillators;
    double sampleRate;
    void initializeOscillators();
    void render(juce::AudioBuffer<float>& buffer, int startSample, int endSample);
    void handleMidiEvent(const juce::MidiMessage& midiEvent);
    float midiNoteNumberToFrequency(int midiNoteNumber);
    std::vector<float> generateSineWaveTable();
    std::vector<float> generateSawWaveTable();
    std::vector<float> generateSquareWaveTable();
    std::vector<float> generateTriangleWaveTable();
    juce::MidiKeyboardState keyboardState;
    float volume = 1.0f;

    int X = 1;

    juce::dsp::IIR::Filter<float> lowPassFilter;
    juce::dsp::IIR::Filter<float> highPassFilter;
};
