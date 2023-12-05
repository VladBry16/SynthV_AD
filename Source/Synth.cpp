#include "Synth.h"

Synth::Synth()
{
	// Инициализация членов класса
}

std::vector<float> Synth::generateSineWaveTable()
{
	constexpr auto WAVETABLE_LENGHT = 64;

	std::vector<float> sineWaveTable(WAVETABLE_LENGHT);

	const auto PI = std::atanf(1.f) * 4;

	for (auto i = 0; i < WAVETABLE_LENGHT; ++i)
	{
		sineWaveTable[i] = std::sinf(2 * PI *
			static_cast<float>(i) / static_cast<float>(WAVETABLE_LENGHT));
	}
	return sineWaveTable;
}

void Synth::initializeOscillators()
{
	constexpr auto OSCILLATORS_COUNT = 128;

	const auto waveTableData = generateSineWaveTable();

	// Преобразование std::vector<float> в juce::AudioSampleBuffer
	juce::AudioSampleBuffer waveTable(1, waveTableData.size());
	std::copy(waveTableData.begin(), waveTableData.end(), waveTable.getWritePointer(0));

	oscillators.clear();
	for (auto i = 0; i < OSCILLATORS_COUNT; ++i) {
		// Создаем новый экземпляр SynthOSC и добавляем его в вектор
		oscillators.push_back(SynthOSC(waveTable, sampleRate));
	}
}


void Synth::prepareToPlay(double sampleRate)
{
	this->sampleRate = sampleRate;

	initializeOscillators();
}

void Synth::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	auto currentSample = 0;

	for (const auto midiMessage : midiMessages)
	{
		const auto midiEvent = midiMessage.getMessage();
		const auto midiEventSample = static_cast<int>(midiEvent.getTimeStamp());

		render(buffer, currentSample, midiEventSample);
		handleMidiEvent(midiEvent);

		currentSample = midiEventSample;

	}

	render(buffer, currentSample, buffer.getNumSamples());
}

void Synth::render(juce::AudioBuffer<float>& buffer,
	int startSample, int endSample)
{
	auto* firstChannel = buffer.getWritePointer(0);

	for (auto& oscillator : oscillators)
	{
		if (oscillator.isPlaying())
		{
			for (auto sample = startSample; sample < endSample; ++sample)
			{
				firstChannel[sample] += oscillator.getNextSample();
			}
		}
	}
	for (auto channel = 1; channel < buffer.getNumChannels(); ++channel)
	{
		std::copy(firstChannel + startSample, firstChannel + endSample,
			buffer.getWritePointer(channel) + startSample);
	}
}

void Synth::handleMidiEvent(const juce::MidiMessage& midiEvent)
{
	if (midiEvent.isNoteOn())
	{
		const auto oscillatorId = midiEvent.getNoteNumber();
		const auto frequency = midiNoteNumberToFrequency(oscillatorId);
		oscillators[oscillatorId].setFrequency(frequency);
		oscillators[oscillatorId].noteOn();
	}
	else if (midiEvent.isNoteOff())
	{
		const auto oscillatorId = midiEvent.getNoteNumber();
		oscillators[oscillatorId].noteOff();
	}
	else if (midiEvent.isAllNotesOff())
	{
		for (auto& oscillator : oscillators)
		{
			oscillator.noteOff();
		}
	}
}

float Synth::midiNoteNumberToFrequency(int midiNoteNumber)
{
	constexpr auto A4_FREQUENCY = 440.f;
	constexpr auto A4_NOTE_NUMBER = 69.f;
	constexpr auto SEMITONES_IN_AN_OCTAVE = 12.f;
	return A4_FREQUENCY * std::powf(2.f, (midiNoteNumber - A4_NOTE_NUMBER) / SEMITONES_IN_AN_OCTAVE);
}

void Synth::setAttack(float attack)
{
	for (auto& oscillator : oscillators)
	{
		oscillator.setAttack(attack);
	}
}

void Synth::setDecay(float decay)
{
	for (auto& oscillator : oscillators)
	{
		oscillator.setDecay(decay);
	}
}

void Synth::setSustain(float sustain)
{
	for (auto& oscillator : oscillators)
	{
		oscillator.setSustain(sustain);
	}
}

void Synth::setRelease(float release)
{
	for (auto& oscillator : oscillators)
	{
		oscillator.setRelease(release);
	}
}

float Synth::getAttack() const
{
	if (oscillators.empty())
		return 0.0f;

	// Вернуть значение атаки для первого осциллятора
	return oscillators[0].getADSR().getParameters().attack;
}

float Synth::getDecay() const
{
	if (oscillators.empty())
		return 0.0f;

	// Вернуть значение атаки для первого осциллятора
	return oscillators[0].getADSR().getParameters().decay;
}

float Synth::getSustain() const
{
	if (oscillators.empty())
		return 0.0f;

	// Вернуть значение атаки для первого осциллятора
	return oscillators[0].getADSR().getParameters().sustain;
}

float Synth::getRelease() const
{
	if (oscillators.empty())
		return 0.0f;

	// Вернуть значение атаки для первого осциллятора
	return oscillators[0].getADSR().getParameters().release;
}