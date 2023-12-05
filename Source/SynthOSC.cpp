#include "SynthOSC.h"

SynthOSC::SynthOSC(juce::AudioSampleBuffer waveTable, double sampleRate)
    : waveTable(std::move(waveTable)),
    sampleRate(sampleRate)
{}

SynthOSC::SynthOSC()
    : sampleRate(0), frequency(0), index(0), indexIncrement(0)
{}

void SynthOSC::setFrequency(float frequency)
{
    this->frequency = frequency;
    indexIncrement = frequency * waveTable.getNumSamples() / sampleRate;
}

void SynthOSC::noteOn()
{
    adsr.noteOn();
}

void SynthOSC::noteOff()  // И эта функция должна быть реализована здесь
{
    adsr.noteOff();
}

float SynthOSC::getNextSample()
{
    auto currentSample = waveTable.getSample(0, static_cast<int>(index));
    index += indexIncrement;
    if (index >= waveTable.getNumSamples())
        index -= waveTable.getNumSamples();

    currentSample *= adsr.getNextSample();
    return currentSample;
}

bool SynthOSC::isPlaying() const
{
    return adsr.isActive();
}

void SynthOSC::setAttack(float attack)
{
    juce::ADSR::Parameters params = adsr.getParameters();
    params.attack = attack;
    adsr.setParameters(params);
}

void SynthOSC::setDecay(float decay)
{
    juce::ADSR::Parameters params = adsr.getParameters();
    params.decay = decay;
    adsr.setParameters(params);
}

void SynthOSC::setSustain(float sustain)
{
    juce::ADSR::Parameters params = adsr.getParameters();
    params.sustain = sustain;
    adsr.setParameters(params);
}

void SynthOSC::setRelease(float release)
{
    juce::ADSR::Parameters params = adsr.getParameters();
    params.release = release;
    adsr.setParameters(params);
}