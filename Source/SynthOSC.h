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

	// �������� ����� ������� ��� ��������� ���������� FM-�������
	void setModulatorFrequency(float frequency);
	void setModulationIndex(float index);

private:
	float interpolateLinearly();
	float generateSquareWave();
	float generateSawtoothWave();
	float generateTriangleWave();

	std::vector<float> waveTable;
	double sampleRate;
	float index = 0.f;
	float indexIncrement = 0.f;

	// �������� ����� ���������� ��� FM-�������
	float modulatorFrequency = 0.0f;
	float modulationIndex = 0.0f;
	float frequency = 0.0f;  // �������� ��� ������
};
