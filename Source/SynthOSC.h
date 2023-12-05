#pragma once
#include <JuceHeader.h>

class SynthOSC
{
public:
    SynthOSC(juce::AudioSampleBuffer waveTable, double sampleRate);
    SynthOSC(); // Конструктор по умолчанию

    void setFrequency(float frequency);
    float getNextSample();
    void noteOn();
    void noteOff();  // Убедитесь, что эта функция объявлена здесь
    bool isPlaying() const;
    void setAttack(float attack);
    void setDecay(float decay);
    void setSustain(float sustain);
    void setRelease(float release);

private:
    juce::AudioSampleBuffer waveTable;
    double sampleRate;
    float frequency = 0.f;
    float index = 0.f;
    float indexIncrement = 0.f;

    juce::ADSR adsr;  // Экземпляр класса juce::ADSR
};
