#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "JuceHeader.h"
#include "CustomLookAndFeel.h"


SynthV_ADAudioProcessorEditor::SynthV_ADAudioProcessorEditor (SynthV_ADAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), adsrDisplay(p.getSynth()),
    attackLabel("A - Attack", "A - Attack"), decayLabel("D - Decay", "D - Decay"),
    sustainLabel("S - Sustain", "S - Sustain"), releaseLabel("R - Release", "R - Release"), 
    keyboardComponent(p.getSynth().getKeyboardState(), juce::MidiKeyboardComponent::horizontalKeyboard)
{
    // Создайте экземпляр CustomLookAndFeel
    customLookAndFeel = std::make_unique<CustomLookAndFeel>();

    // Примените CustomLookAndFeel к слайдерам
    attackSlider.setLookAndFeel(customLookAndFeel.get());
    decaySlider.setLookAndFeel(customLookAndFeel.get());
    sustainSlider.setLookAndFeel(customLookAndFeel.get());
    releaseSlider.setLookAndFeel(customLookAndFeel.get());

    // Установите диапазон, интервал и начальное значение для каждого слайдера
    attackSlider.setRange(0.0, 1.0);
    attackSlider.setValue(audioProcessor.getSynth().getAttack());
    decaySlider.setRange(0.1, 1.0);
    decaySlider.setValue(audioProcessor.getSynth().getDecay());
    sustainSlider.setRange(0.0, 1.0);
    sustainSlider.setValue(audioProcessor.getSynth().getSustain());
    releaseSlider.setRange(0.1, 1.0);
    releaseSlider.setValue(audioProcessor.getSynth().getRelease());

    volumeSlider.setRange(0.0, 1.0); // Установите диапазон для слайдера громкости
    volumeSlider.setValue(audioProcessor.getSynth().getVolume()); // Установите начальное значение слайдера громкости
    volumeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0); // Уберите поле с цифрами около слайдера громкости
    addAndMakeVisible(volumeSlider); // Добавьте слайдер громкости на ваш компонент
    volumeSlider.addListener(this); // Добавьте слушателя для слайдера громкости

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

    addAndMakeVisible(sineWaveButton);
    addAndMakeVisible(sawWaveButton);
    addAndMakeVisible(squareWaveButton);
    addAndMakeVisible(triangleWaveButton);

    sineWaveButton.setButtonText("Sine Wave");
    sawWaveButton.setButtonText("Saw Wave");
    squareWaveButton.setButtonText("Square Wave");
    triangleWaveButton.setButtonText("Triangle Wave");

    sineWaveButton.addListener(this);
    sawWaveButton.addListener(this);
    squareWaveButton.addListener(this);
    triangleWaveButton.addListener(this);

    addAndMakeVisible(keyboardComponent);

    // Добавьте ADSRDisplay на ваш компонент
    addAndMakeVisible(adsrDisplay);

    selectWave(x);

    setSize(1000, 500);
}

SynthV_ADAudioProcessorEditor::~SynthV_ADAudioProcessorEditor()
{
}

void SynthV_ADAudioProcessorEditor::paint(juce::Graphics& g)
{
    juce::Image background = juce::ImageCache::getFromMemory(BinaryData::Gradient_png, BinaryData::Gradient_pngSize);
    g.drawImageAt(background, 0, 0);
    juce::Image neon = juce::ImageCache::getFromMemory(BinaryData::Piano_png, BinaryData::Piano_pngSize);
    int imageY = getHeight() - neon.getHeight(); // Расчет координаты Y для размещения изображения внизу
    g.drawImageAt(neon, 0, imageY);
    juce::Image point_1 = juce::ImageCache::getFromMemory(BinaryData::Points_png, BinaryData::Points_pngSize);
    g.drawImageAt(point_1, 532, 246);
    juce::Image point_2 = juce::ImageCache::getFromMemory(BinaryData::Points_png, BinaryData::Points_pngSize);
    g.drawImageAt(point_2, 635, 246);
    juce::Image point_3 = juce::ImageCache::getFromMemory(BinaryData::Points_png, BinaryData::Points_pngSize);
    g.drawImageAt(point_3, 738, 246);
    juce::Image point_4 = juce::ImageCache::getFromMemory(BinaryData::Points_png, BinaryData::Points_pngSize);
    g.drawImageAt(point_4, 841, 246);
}

void SynthV_ADAudioProcessorEditor::resized()
{
    // Здесь вы можете установить положение и размер слайдеров
    // Например:
    int sliderSize = 80; // Увеличиваем размер слайдера
    int padding = 10; // Отступ между слайдерами
    int labelHeight = 20; // Высота метки

    // Устанавливаем положение и размер меток и слайдеров
    attackLabel.setBounds(532, 326, sliderSize, labelHeight);
    attackSlider.setSliderStyle(juce::Slider::Rotary); // Устанавливаем стиль слайдера как Rotary
    attackSlider.setBounds(532, 246, sliderSize, sliderSize);

    decayLabel.setBounds(635, 326, sliderSize, labelHeight);
    decaySlider.setSliderStyle(juce::Slider::Rotary); // Устанавливаем стиль слайдера как Rotary
    decaySlider.setBounds(635, 246, sliderSize, sliderSize);

    sustainLabel.setBounds(738, 326, sliderSize, labelHeight);
    sustainSlider.setSliderStyle(juce::Slider::Rotary); // Устанавливаем стиль слайдера как Rotary
    sustainSlider.setBounds(738, 246, sliderSize, sliderSize);

    releaseLabel.setBounds(841, 326, sliderSize, labelHeight);
    releaseSlider.setSliderStyle(juce::Slider::Rotary); // Устанавливаем стиль слайдера как Rotary
    releaseSlider.setBounds(841, 246, sliderSize, sliderSize);

    sineWaveButton.setBounds(350, 10, 100, 30);
    sawWaveButton.setBounds(350, 100, 100, 30);
    squareWaveButton.setBounds(350, 200, 100, 30);
    triangleWaveButton.setBounds(350, 300, 100, 30);

    volumeSlider.setBounds(300, 100, 72, 72);

    keyboardComponent.setBounds(getLocalBounds().removeFromBottom(80).reduced(100, 0));;

    // Установите положение и размер ADSRDisplay
    adsrDisplay.setBounds(564, 46, 330, 194);
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
    if (slider == &volumeSlider)
    {
        audioProcessor.getSynth().setVolume(volumeSlider.getValue());
    }
    // Перерисуйте ADSRDisplay
    adsrDisplay.repaint();
}

void SynthV_ADAudioProcessorEditor::buttonClicked(juce::Button* button)
{

    if (button == &sineWaveButton)
    {
        x = 1;
    }
    else if (button == &sawWaveButton)
    {
        x = 2;
    }
    else if (button == &squareWaveButton)
    {
        x = 3;
    }
    else if (button == &triangleWaveButton)
    {
        x = 4;
    }
    selectWave(x);
}

void SynthV_ADAudioProcessorEditor::selectWave(int x)
{
    if (x == 1)
    {
        audioProcessor.getSynth().setWaveTable(Waveform::Sine);
        sineWaveButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
        sawWaveButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
        squareWaveButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
        triangleWaveButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
    }
    else if (x == 2)
    {
        audioProcessor.getSynth().setWaveTable(Waveform::Saw);
        sineWaveButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
        sawWaveButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
        squareWaveButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
        triangleWaveButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
    }
    else if (x == 3)
    {
        audioProcessor.getSynth().setWaveTable(Waveform::Square);
        sineWaveButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
        sawWaveButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
        squareWaveButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
        triangleWaveButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
    }
    else if (x == 4)
    {
        audioProcessor.getSynth().setWaveTable(Waveform::Triangle);
        sineWaveButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
        sawWaveButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
        squareWaveButton.setColour(juce::TextButton::buttonColourId, juce::Colours::white);
        triangleWaveButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    }
}