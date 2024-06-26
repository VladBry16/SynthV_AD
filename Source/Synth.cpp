#include "Synth.h"

Synth::Synth()
{
	keyboardState.addListener(this);
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

std::vector<float> Synth::generateSawWaveTable()
{
	constexpr auto WAVETABLE_LENGHT = 64;

	std::vector<float> sawWaveTable(WAVETABLE_LENGHT);

	for (auto i = 0; i < WAVETABLE_LENGHT; ++i)
	{
		sawWaveTable[i] = 1.0 - (2.0 * i / WAVETABLE_LENGHT);
	}

	return sawWaveTable;
}

std::vector<float> Synth::generateSquareWaveTable()
{
	constexpr auto WAVETABLE_LENGHT = 64;

	std::vector<float> squareWaveTable(WAVETABLE_LENGHT);

	for (auto i = 0; i < WAVETABLE_LENGHT; ++i)
	{
		squareWaveTable[i] = i < WAVETABLE_LENGHT / 3 ? 1.0 : -1.0;
	}

	return squareWaveTable;
}

std::vector<float> Synth::generateTriangleWaveTable()
{
	constexpr auto WAVETABLE_LENGHT = 64;

	std::vector<float> triangleWaveTable(WAVETABLE_LENGHT);

	for (auto i = 0; i < WAVETABLE_LENGHT; ++i)
	{
		triangleWaveTable[i] = 2.0 * std::abs(2.0 * ((i / static_cast<double>(WAVETABLE_LENGHT)) - std::round(i / static_cast<double>(WAVETABLE_LENGHT))) + 0.75) - 1.0;
	}

	return triangleWaveTable;
}


void Synth::initializeOscillators()
{
	constexpr auto OSCILLATORS_COUNT = 128;

	const auto waveTableData = generateSineWaveTable();

	juce::AudioSampleBuffer waveTable(1, waveTableData.size());
	std::copy(waveTableData.begin(), waveTableData.end(), waveTable.getWritePointer(0));

	oscillators.clear();
	for (auto i = 0; i < OSCILLATORS_COUNT; ++i) {
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

	if (highPassFilters.size() != buffer.getNumChannels())
	{
		highPassFilters.resize(buffer.getNumChannels());
	}

	if (lowPassFilters.size() != buffer.getNumChannels())
	{
		lowPassFilters.resize(buffer.getNumChannels());
	}

	for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
	{
		auto* channelData = buffer.getWritePointer(channel);
		juce::dsp::AudioBlock<float> block(buffer);
		block = block.getSingleChannelBlock(channel);
		juce::dsp::ProcessContextReplacing<float> context(block);
		highPassFilters[channel].process(context);
		lowPassFilters[channel].process(context);
	}
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
				firstChannel[sample] += volume * oscillator.getNextSample();
			}
		}
	}
	for (auto channel = 1; channel < buffer.getNumChannels(); ++channel)
	{
		std::copy(firstChannel + startSample, firstChannel + endSample,
			buffer.getWritePointer(channel) + startSample);
	}
}

void Synth::handleNoteOn(juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
	const auto oscillatorId = midiNoteNumber;
	const auto frequency = midiNoteNumberToFrequency(oscillatorId);
	oscillators[oscillatorId].setFrequency(frequency);
	oscillators[oscillatorId].noteOn();
}

void Synth::handleNoteOff(juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
	const auto oscillatorId = midiNoteNumber;
	oscillators[oscillatorId].noteOff();
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
	else if (midiEvent.isController())
	{
		const auto controllerNumber = midiEvent.getControllerNumber();
		const auto controllerValue = midiEvent.getControllerValue();

		switch (controllerNumber)
		{
		case 1:
			setAttack(controllerValue / 127.0f);
			break;
		case 2:
			setDecay(controllerValue / 127.0f);
			break;
		case 7: 
			setSustain(controllerValue / 127.0f);
			break;
		case 11:
			setRelease(controllerValue / 127.0f);
			break;
		default:
			break;
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

	return oscillators[0].getADSR().getParameters().attack;
}

float Synth::getDecay() const
{
	if (oscillators.empty())
		return 0.0f;

	return oscillators[0].getADSR().getParameters().decay;
}

float Synth::getSustain() const
{
	if (oscillators.empty())
		return 0.0f;

	return oscillators[0].getADSR().getParameters().sustain;
}

float Synth::getRelease() const
{
	if (oscillators.empty())
		return 0.0f;

	return oscillators[0].getADSR().getParameters().release;
}

void Synth::setWaveTable(Waveform waveform)
{
	std::vector<float> waveTableData;

	switch (waveform)
	{
	case Waveform::Sine:
		waveTableData = generateSineWaveTable();
		break;
	case Waveform::Saw:
		waveTableData = generateSawWaveTable();
		break;
	case Waveform::Square:
		waveTableData = generateSquareWaveTable();
		break;
	case Waveform::Triangle:
		waveTableData = generateTriangleWaveTable();
		break;
	}

	juce::AudioSampleBuffer waveTable(1, waveTableData.size());
	std::copy(waveTableData.begin(), waveTableData.end(), waveTable.getWritePointer(0));

	for (auto& oscillator : oscillators) {
		oscillator.setWaveTable(waveTable);
	}
}

void Synth::setVolume(float newVolume) {
	volume = newVolume;
}

float Synth::getVolume() const {
	return volume;
}

float Synth::getModulationFrequency() {
	if (oscillators.empty())
		return 0.0f;

	return oscillators[0].getModulationFrequency();
}

float Synth::getModulationDepth() {
	if (oscillators.empty())
		return 0.0f;

	return oscillators[0].getModulationDepth();
}

void Synth::setModulationDepth(float newDepth) { 
	for (auto& oscillator : oscillators)
	{
		oscillator.setModulationDepth(newDepth);
	}
}

void Synth::setModulationFrequency(float newFrequency) { 
	for (auto& oscillator : oscillators)
	{
		oscillator.setModulationFrequency(newFrequency);
	}
}

void Synth::setX(int x) {
	X = x;
}

int Synth::getX() const
{
	return X;
}

void Synth::setHighPassFreq(float freq)
{
	highPassFreq = freq;
	auto coeffs = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, freq, 1);
	for (auto& filter : highPassFilters)
	{
		filter.coefficients = *coeffs;
	}
}

void Synth::setLowPassFreq(float freq)
{
	lowPassFreq = freq;
	auto coeffs = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, freq, 1);
	for (auto& filter : lowPassFilters)
	{
		filter.coefficients = *coeffs;
	}
}

double Synth::getSampleRate() const 
{ 
	return sampleRate; 
}

float Synth::getHighPassFreq() const
{ 
	return highPassFreq;
}

float Synth::getLowPassFreq() const
{ 
	return lowPassFreq;
}