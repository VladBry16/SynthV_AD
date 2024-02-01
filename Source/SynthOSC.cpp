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

void SynthOSC::noteOff()
{
    adsr.noteOff();
}

float SynthOSC::getNextSample()
{
    auto modulatedFrequency = generateModulatedSignal(frequency, time);
    auto currentSample = waveTable.getSample(0, static_cast<int>(index));
    index += indexIncrement * modulatedFrequency / frequency;
    if (index >= waveTable.getNumSamples())
        index -= waveTable.getNumSamples();

    currentSample *= adsr.getNextSample();
    time += 1.0 / sampleRate;  // Увеличиваем время

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

const juce::ADSR& SynthOSC::getADSR() const
{
    return adsr;
}

void SynthOSC::setWaveTable(juce::AudioSampleBuffer waveTable)
{
    this->waveTable = std::move(waveTable);
}

float SynthOSC::getFrequency() const { return frequency; }

float SynthOSC::generateModulatedSignal(float carrierFrequency, float time)
{
    if (modDepth == 0.0f)
    {
        return carrierFrequency;
    }
    else
    {
        const auto PI = std::atanf(1.f) * 4;
        return carrierFrequency + modDepth * 100 * std::sinf(2 * PI * modFrequency * time);
    }
}

void SynthOSC::setModulationDepth(float newDepth) { modDepth = newDepth; }

void SynthOSC::setModulationFrequency(float newFrequency) { modFrequency = newFrequency; }

float SynthOSC::getModulationDepth() { return modDepth; }

float SynthOSC::getModulationFrequency() { return modFrequency; }