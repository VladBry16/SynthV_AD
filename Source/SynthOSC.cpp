#include "SynthOSC.h"
#include <cmath>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

SynthOSC::SynthOSC(std::vector<float> waveTable,
	double sampleRate)
	: waveTable{ std::move(waveTable) },
	sampleRate{ sampleRate }
{}

void SynthOSC::setFrequency(float frequency)
{
	this->frequency = frequency;  // добавьте эту строку
	indexIncrement = frequency * static_cast<float>(waveTable.size())
		/ static_cast<float>(sampleRate);
}

void SynthOSC::setModulatorFrequency(float frequency) {
	modulatorFrequency = frequency;
}

void SynthOSC::setModulationIndex(float index) {
	modulationIndex = index;
}

float SynthOSC::getSample() {
	const auto modulatorSample = sin(2.0f * M_PI * modulatorFrequency / sampleRate);
	const auto carrierFrequency = frequency + modulationIndex * modulatorSample;
	const auto sample = sin(2.0f * M_PI * carrierFrequency / sampleRate);
	index += indexIncrement;
	index = std::fmod(index, static_cast<float>(waveTable.size()));
	return sample;
}

float SynthOSC::interpolateLinearly()
{
	const auto truncatedIndex = static_cast<int>(index);
	const auto nextIndex = (truncatedIndex + 1) %
		static_cast<int>(waveTable.size());
	const auto nextIndexWeight = index - static_cast<float>(truncatedIndex);
	const auto truncatedIndexWeight = 1.f - nextIndexWeight;

	return truncatedIndexWeight * waveTable[truncatedIndex] +
		nextIndexWeight * waveTable[nextIndex];
}

void SynthOSC::stop()
{
	index = 0.f;
	indexIncrement = 0.f;
}

bool SynthOSC::isPlaying()
{
	return indexIncrement != 0.f;
}

float SynthOSC::generateSawtoothWave()
{
	return 2.0f * (index / static_cast<float>(waveTable.size()) -
		floor(0.5f + index / static_cast<float>(waveTable.size())));
}

float SynthOSC::generateSquareWave()
{
	return (index < waveTable.size() / 2) ? 1.0f : -1.0f;
}

float SynthOSC::generateTriangleWave()
{
	return 2.0f * fabs(2.0f * (index /
		static_cast<float>(waveTable.size()) - floor(0.5f + index /
			static_cast<float>(waveTable.size())))) - 1.0f;
}