#pragma once
#include <JuceHeader.h>

class SynthOSC
{
public:
    SynthOSC(juce::AudioSampleBuffer waveTable, double sampleRate);

    void setFrequency(float frequency);
    float getNextSample();
    void noteOn();
    void noteOff();  // Убедитесь, что эта функция объявлена здесь
    bool isPlaying() const;

private:
    juce::AudioSampleBuffer waveTable;
    double sampleRate;
    float frequency = 0.f;
    float index = 0.f;
    float indexIncrement = 0.f;

    juce::ADSR adsr;  // Экземпляр класса juce::ADSR
};
