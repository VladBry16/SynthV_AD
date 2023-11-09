#pragma once
#include <vector>

class SynthOSC
{
public:
	SynthOSC(std::vector<float> waveTable, double sampleRate);

	void setFrequency(float frequency);
	float getSample();

	void stop();
	bool isPlaying();

private:
	float interpolateLinearly();
	float generateSquareWave();
	float generateSawtoothWave();
	float generateTriangleWave();

	std::vector<float> waveTable;
	double sampleRate;
	float index = 0.f;
	float indexIncrement = 0.f;

};

