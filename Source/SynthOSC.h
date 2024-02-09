#pragma once
#include <JuceHeader.h>

class SynthOSC
{
public:
    SynthOSC(juce::AudioSampleBuffer waveTable, double sampleRate);
    SynthOSC();

    void setFrequency(float frequency);
    float getNextSample();
    void noteOn();
    void noteOff();
    bool isPlaying() const;
    
    void setAttack(float attack);
    void setDecay(float decay);
    void setSustain(float sustain);
    void setRelease(float release);

    void setModulationDepth(float newDepth);
    void setModulationFrequency(float newFrequency);
    float getModulationDepth();
    float getModulationFrequency();

    float generateModulatedSignal(float carrierFrequency, float time);
    
    float getFrequency() const;

    const juce::ADSR& getADSR() const;

    void setWaveTable(juce::AudioSampleBuffer waveTable);

private:
    juce::AudioSampleBuffer waveTable;
    double sampleRate;
    float frequency = 0.f;
    float index = 0.f;
    float indexIncrement = 0.f;

    float modDepth = 0.4f;
    float modFrequency = 0.0f;

    float time = 0.0f;

    juce::ADSR adsr;
};
