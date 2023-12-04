#include "SynthOSC.h"

SynthOSC::SynthOSC(juce::AudioSampleBuffer waveTable, double sampleRate)
    : waveTable(std::move(waveTable)),
    sampleRate(sampleRate)
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
