#pragma once
#include <JuceHeader.h>

class SynthOSC
{
public:
    SynthOSC(juce::AudioSampleBuffer waveTable, double sampleRate);
    SynthOSC();

    void setFrequency(float frequency);
    void setPhase(float phase); // Äîáàâëÿåì ìåòîä äëÿ óñòàíîâêè ôàçû

    float getNextSample();
    void noteOn();
    void noteOff();
    bool isPlaying() const;

    void setAttack(float attack);
    void setDecay(float decay);
    void setSustain(float sustain);
    void setRelease(float release);

    // Ìåòîäû äëÿ ìîäóëÿöèè ïîêà îñòàâëÿåì áåç èçìåíåíèé
    void setModulationDepth(float newDepth);
    void setModulationFrequency(float newFrequency);
    float getModulationDepth();
    float getModulationFrequency();

    float getFrequency() const;
    float getPhase() const; // Äîáàâëÿåì ìåòîä äëÿ ïîëó÷åíèÿ òåêóùåé ôàçû

    const juce::ADSR& getADSR() const;

    void setWaveTable(juce::AudioSampleBuffer waveTable);

private:
    void updateIndexIncrement();

    juce::AudioSampleBuffer waveTable;
    double sampleRate = 44100.f;
    float frequency = 0.f;
    float phase = 0.f;
    float indexIncrement = 0.f;

    float modDepth = 0.4f;
    float modFrequency = 0.0f;

    float time = 0.0f;

    juce::ADSR adsr;
};