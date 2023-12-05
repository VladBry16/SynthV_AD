#include "PluginProcessor.h"
#include "PluginEditor.h"

SynthV_ADAudioProcessorEditor::SynthV_ADAudioProcessorEditor (SynthV_ADAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), adsrDisplay(p.getSynth()),
    attackLabel("A - Attack", "A - Attack"), decayLabel("D - Decay", "D - Decay"),
    sustainLabel("S - Sustain", "S - Sustain"), releaseLabel("R - Release", "R - Release")
{
    // Установите диапазон, интервал и начальное значение для каждого слайдера
    attackSlider.setRange(0.1, 1.0);
    attackSlider.setValue(0.0);
    decaySlider.setRange(0.1, 1.0);
    decaySlider.setValue(0.0);
    sustainSlider.setRange(0.1, 1.0);
    sustainSlider.setValue(0.1);
    releaseSlider.setRange(0.0, 1.0);
    releaseSlider.setValue(0.1);

    // Уберите поле с цифрами около слайдеров
    attackSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    decaySlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    sustainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    releaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

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

    // Добавьте метки на ваш компонент
    addAndMakeVisible(attackLabel);
    addAndMakeVisible(decayLabel);
    addAndMakeVisible(sustainLabel);
    addAndMakeVisible(releaseLabel);

    // Добавьте ADSRDisplay на ваш компонент
    addAndMakeVisible(adsrDisplay);

    setSize(1000, 500);
}

SynthV_ADAudioProcessorEditor::~SynthV_ADAudioProcessorEditor()
{
}

void SynthV_ADAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void SynthV_ADAudioProcessorEditor::resized()
{
    // Здесь вы можете установить положение и размер слайдеров
    // Например:
    int sliderSize = 150; // Увеличиваем размер слайдера
    int padding = 10; // Отступ между слайдерами
    int labelHeight = 20; // Высота метки

    // Устанавливаем положение и размер меток и слайдеров
    attackLabel.setBounds(padding, padding, sliderSize, labelHeight);
    attackSlider.setSliderStyle(juce::Slider::Rotary); // Устанавливаем стиль слайдера как Rotary
    attackSlider.setBounds(padding, padding + labelHeight, sliderSize, sliderSize);

    decayLabel.setBounds(padding + sliderSize + padding, padding, sliderSize, labelHeight);
    decaySlider.setSliderStyle(juce::Slider::Rotary); // Устанавливаем стиль слайдера как Rotary
    decaySlider.setBounds(padding + sliderSize + padding, padding + labelHeight, sliderSize, sliderSize);

    sustainLabel.setBounds(padding, padding + sliderSize + padding + labelHeight, sliderSize, labelHeight);
    sustainSlider.setSliderStyle(juce::Slider::Rotary); // Устанавливаем стиль слайдера как Rotary
    sustainSlider.setBounds(padding, padding + sliderSize + padding + 2 * labelHeight, sliderSize, sliderSize);

    releaseLabel.setBounds(padding + sliderSize + padding, padding + sliderSize + padding + labelHeight, sliderSize, labelHeight);
    releaseSlider.setSliderStyle(juce::Slider::Rotary); // Устанавливаем стиль слайдера как Rotary
    releaseSlider.setBounds(padding + sliderSize + padding, padding + sliderSize + padding + 2 * labelHeight, sliderSize, sliderSize);

    // Установите положение и размер ADSRDisplay
    adsrDisplay.setBounds(padding, 2 * padding + 2 * sliderSize + 2 * labelHeight, getWidth() - 2 * padding, getHeight() - 3 * padding - 2 * sliderSize - 2 * labelHeight);
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
    // Перерисуйте ADSRDisplay
    adsrDisplay.repaint();
}