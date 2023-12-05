#include "PluginProcessor.h"
#include "PluginEditor.h"

SynthV_ADAudioProcessorEditor::SynthV_ADAudioProcessorEditor (SynthV_ADAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Установите диапазон, интервал и начальное значение для каждого слайдера
    attackSlider.setRange(0.1, 5.0);
    attackSlider.setValue(0.0);
    decaySlider.setRange(0.1, 5.0);
    decaySlider.setValue(0.0);
    sustainSlider.setRange(0.0, 5.0);
    sustainSlider.setValue(0.1);
    releaseSlider.setRange(0.0, 5.0);
    releaseSlider.setValue(0.1);

    // Добавьте слайдеры на ваш компонент
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(decaySlider);
    addAndMakeVisible(sustainSlider);
    addAndMakeVisible(releaseSlider);

    // Добавьте слушателей для слайдеров
    attackSlider.addListener(this);
    decaySlider.addListener(this);
    sustainSlider.addListener(this);
    releaseSlider.addListener(this);

    setSize (400, 300);
}

SynthV_ADAudioProcessorEditor::~SynthV_ADAudioProcessorEditor()
{
}

void SynthV_ADAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::royalblue);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void SynthV_ADAudioProcessorEditor::resized()
{
    // Здесь вы можете установить положение и размер слайдеров
    // Например:
    attackSlider.setBounds(10, 10, getWidth() - 20, 20);
    decaySlider.setBounds(10, 40, getWidth() - 20, 20);
    sustainSlider.setBounds(10, 70, getWidth() - 20, 20);
    releaseSlider.setBounds(10, 100, getWidth() - 20, 20);
}

void SynthV_ADAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &attackSlider)
    {
        audioProcessor.getSynth().setAttack(attackSlider.getValue());
    }
    if (slider == &decaySlider)
    {
        audioProcessor.getSynth().setDecay(decaySlider.getValue());
    }
    if (slider == &sustainSlider)
    {
        audioProcessor.getSynth().setSustain(sustainSlider.getValue());
    }
    if (slider == &releaseSlider)
    {
        audioProcessor.getSynth().setRelease(releaseSlider.getValue());
    }
}
