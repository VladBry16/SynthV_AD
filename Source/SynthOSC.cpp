#include "SynthOSC.h"

SynthOSC::SynthOSC(juce::AudioSampleBuffer waveTable, double sampleRate)
    : waveTable(std::move(waveTable)),
    sampleRate(sampleRate)
{}

SynthOSC::SynthOSC()
    : sampleRate(0), frequency(0), phase(0), indexIncrement(0)
{}

void SynthOSC::setFrequency(float frequency)
{
    this->frequency = frequency;
    updateIndexIncrement();
}

void SynthOSC::setPhase(float phase)
{
    // Óñòàíàâëèâàåì ôàçó â äèàïàçîíå [0, 1)
    this->phase = std::fmod(phase, 1.0f);
    if (this->phase < 0.0f) {
        this->phase += 1.0f;
    }
}

void SynthOSC::noteOn()
{
    adsr.noteOn();
    // Íå ñáðàñûâàåì ôàçó ïðè noteOn, 
    // ÷òîáû çâóê íå ïðåðûâàëñÿ ïðè ïîâòîðíûõ íàæàòèÿõ
}

void SynthOSC::noteOff()
{
    adsr.noteOff();
}

float SynthOSC::getNextSample()
{
    // Âû÷èñëÿåì ïîçèöèþ ñýìïëà â òàáëèöå
    float index = phase * waveTable.getNumSamples();

    // Èíòåðïîëèðóåì ìåæäó ñýìïëàìè äëÿ áîëåå ïëàâíîãî çâóêà
    int intIndex = static_cast<int>(index);
    float fracIndex = index - intIndex;

    float currentSample = waveTable.getSample(0, intIndex) * (1.0f - fracIndex) +
        waveTable.getSample(0, (intIndex + 1) % waveTable.getNumSamples()) * fracIndex;

    // Ïðèìåíÿåì îãèáàþùóþ ADSR
    currentSample *= adsr.getNextSample();

    // Îáíîâëÿåì ôàçó
    phase += frequency / sampleRate;
    if (phase >= 1.0f)
        phase -= 1.0f;

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

void SynthOSC::setModulationDepth(float newDepth) { modDepth = newDepth; }

void SynthOSC::setModulationFrequency(float newFrequency) { modFrequency = newFrequency; }

float SynthOSC::getModulationDepth() { return modDepth; }

float SynthOSC::getModulationFrequency() { return modFrequency; }

void SynthOSC::updateIndexIncrement()
{
    indexIncrement = frequency * waveTable.getNumSamples() / sampleRate;
}