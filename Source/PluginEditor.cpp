#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "JuceHeader.h"
#include "CustomLookAndFeel.h"


SynthV_ADAudioProcessorEditor::SynthV_ADAudioProcessorEditor (SynthV_ADAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), adsrDisplay(p.getSynth()),
    attackLabel("A - Attack", "A - Attack"), decayLabel("D - Decay", "D - Decay"),
    sustainLabel("S - Sustain", "S - Sustain"), releaseLabel("R - Release", "R - Release"), 
    volumeLabel("Volume", "Volume"), keyboardComponent(p.getSynth().getKeyboardState(), juce::MidiKeyboardComponent::horizontalKeyboard)
{
    // Создайте экземпляр CustomLookAndFeel
    customLookAndFeel = std::make_unique<CustomLookAndFeel>();

    // Примените CustomLookAndFeel к слайдерам
    attackSlider.setLookAndFeel(customLookAndFeel.get());
    decaySlider.setLookAndFeel(customLookAndFeel.get());
    sustainSlider.setLookAndFeel(customLookAndFeel.get());
    releaseSlider.setLookAndFeel(customLookAndFeel.get());
    volumeSlider.setLookAndFeel(customLookAndFeel.get());

    depthSlider.setLookAndFeel(customLookAndFeel.get());
    frequencySlider.setLookAndFeel(customLookAndFeel.get());
    addAndMakeVisible(depthSlider);
    addAndMakeVisible(frequencySlider);
    depthSlider.setRange(0.0, 1.0);
    frequencySlider.setRange(0.1, 10.0);
    depthSlider.addListener(this);
    frequencySlider.addListener(this);
    depthSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    frequencySlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);


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
    addAndMakeVisible(volumeLabel);

    addAndMakeVisible(sineWaveButton);
    addAndMakeVisible(sawWaveButton);
    addAndMakeVisible(squareWaveButton);
    addAndMakeVisible(triangleWaveButton);

    selectWave();

    // Установите идентификатор группы для каждой кнопки
    sineWaveButton.setRadioGroupId(1);
    sawWaveButton.setRadioGroupId(1);
    squareWaveButton.setRadioGroupId(1);
    triangleWaveButton.setRadioGroupId(1);

    sineWaveButton.addListener(this);
    sawWaveButton.addListener(this);
    squareWaveButton.addListener(this);
    triangleWaveButton.addListener(this);

    addAndMakeVisible(keyboardComponent);

    // Добавьте ADSRDisplay на ваш компонент
    addAndMakeVisible(adsrDisplay);

    setSize(1000, 500);
}

SynthV_ADAudioProcessorEditor::~SynthV_ADAudioProcessorEditor()
{
}

void SynthV_ADAudioProcessorEditor::paint(juce::Graphics& g)
{
    juce::Image background = juce::ImageCache::getFromMemory(BinaryData::Gradient_png, BinaryData::Gradient_pngSize);
    g.drawImageAt(background, 0, 0);
    juce::Image group = juce::ImageCache::getFromMemory(BinaryData::Group_png, BinaryData::Group_pngSize);
    g.drawImageAt(group, 0, 0);
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
    juce::Image point_5 = juce::ImageCache::getFromMemory(BinaryData::Points_png, BinaryData::Points_pngSize);
    g.drawImageAt(point_5, 15, 400);
}

void SynthV_ADAudioProcessorEditor::resized()
{
    // Здесь вы можете установить положение и размер слайдеров
    // Например:
    int sliderSize = 80; // Увеличиваем размер слайдера
    int padding = 10; // Отступ между слайдерами
    int labelHeight = 20; // Высота метки
    int buttonSize = 90;

    // Устанавливаем положение и размер меток и слайдеров
    attackLabel.setBounds(532, 326, sliderSize, labelHeight);
    attackSlider.setSliderStyle(juce::Slider::Rotary); // Устанавливаем стиль слайдера как Rotary
    attackSlider.setBounds(532, 246, sliderSize, sliderSize);

    decayLabel.setBounds(635, 326, sliderSize, labelHeight);
    decaySlider.setSliderStyle(juce::Slider::Rotary); // Устанавливаем стиль слайдера как Rotary
    decaySlider.setBounds(635, 246, sliderSize, sliderSize);

    depthSlider.setBounds(100, 200, sliderSize, sliderSize);
    frequencySlider.setBounds(100, 300, sliderSize, sliderSize);

    sustainLabel.setBounds(738, 326, sliderSize, labelHeight);
    sustainSlider.setSliderStyle(juce::Slider::Rotary); // Устанавливаем стиль слайдера как Rotary
    sustainSlider.setBounds(738, 246, sliderSize, sliderSize);

    releaseLabel.setBounds(841, 326, sliderSize, labelHeight);
    releaseSlider.setSliderStyle(juce::Slider::Rotary); // Устанавливаем стиль слайдера как Rotary
    releaseSlider.setBounds(841, 246, sliderSize, sliderSize);

    sineWaveButton.setBounds(59, 52, buttonSize, buttonSize);
    sawWaveButton.setBounds(158, 52, buttonSize, buttonSize);
    squareWaveButton.setBounds(257, 52, buttonSize, buttonSize);
    triangleWaveButton.setBounds(356, 52, buttonSize, buttonSize);

    volumeSlider.setSliderStyle(juce::Slider::Rotary);
    volumeSlider.setBounds(15, 400, sliderSize, sliderSize);

    volumeLabel.setBounds(15, 475, sliderSize, labelHeight);

    auto bounds = getLocalBounds().removeFromBottom(84).reduced(100, 0);
    bounds.setX(150);

    keyboardComponent.setBounds(bounds);

    // Установите положение и размер ADSRDisplay
    adsrDisplay.setBounds(564, 46, 330, 194);
}

void SynthV_ADAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &attackSlider)
    {
        audioProcessor.getSynth().setAttack(attackSlider.getValue());
        adsrDisplay.repaint();
    }
    if (slider == &decaySlider)
    {
        audioProcessor.getSynth().setDecay(decaySlider.getValue());
        adsrDisplay.repaint();
    }
    if (slider == &sustainSlider)
    {
        audioProcessor.getSynth().setSustain(sustainSlider.getValue());
        adsrDisplay.repaint();
    }
    if (slider == &releaseSlider)
    {
        audioProcessor.getSynth().setRelease(releaseSlider.getValue());
        adsrDisplay.repaint();
    }
    if (slider == &volumeSlider)
    {
        audioProcessor.getSynth().setVolume(volumeSlider.getValue());
    }
    if (slider == &depthSlider) {
        audioProcessor.getSynth().setModulationDepth(depthSlider.getValue());
    }
    if (slider == &frequencySlider) {
        audioProcessor.getSynth().setModulationFrequency(frequencySlider.getValue());

    }
}

void SynthV_ADAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &sineWaveButton)
    {
        audioProcessor.getSynth().setX(1);
    }
    else if (button == &sawWaveButton)
    {
        audioProcessor.getSynth().setX(2);
    }
    else if (button == &squareWaveButton)
    {
        audioProcessor.getSynth().setX(3);
    }
    else if (button == &triangleWaveButton)
    {
        audioProcessor.getSynth().setX(4);
    }
    selectWave();
}

void SynthV_ADAudioProcessorEditor::selectWave()
{
    sineWaveButton.setImages(true, true, true,
        sin_off, 1.0f, juce::Colours::transparentBlack,
        sin_on, 1.0f, juce::Colours::transparentBlack,
        sin_off, 1.0f, juce::Colours::transparentBlack);
    sawWaveButton.setImages(false, true, true,
        saw_off, 1.0f, juce::Colours::transparentBlack,
        saw_on, 1.0f, juce::Colours::transparentBlack,
        saw_off, 1.0f, juce::Colours::transparentBlack);
    squareWaveButton.setImages(false, true, true,
        square_off, 1.0f, juce::Colours::transparentBlack,
        square_on, 1.0f, juce::Colours::transparentBlack,
        square_off, 1.0f, juce::Colours::transparentBlack);
    triangleWaveButton.setImages(false, true, true,
        triangle_off, 1.0f, juce::Colours::transparentBlack,
        triangle_on, 1.0f, juce::Colours::transparentBlack,
        triangle_off, 1.0f, juce::Colours::transparentBlack);
    if (audioProcessor.getSynth().getX() == 1) {
        audioProcessor.getSynth().setWaveTable(Waveform::Sine);
        sineWaveButton.setImages(true, true, true,
            sin_select, 1.0f, juce::Colours::transparentBlack,
            sin_select, 1.0f, juce::Colours::transparentBlack,
            sin_select, 1.0f, juce::Colours::transparentBlack);
    }
    else if (audioProcessor.getSynth().getX() == 2)
    {
        audioProcessor.getSynth().setWaveTable(Waveform::Saw);
        sawWaveButton.setImages(false, true, true,
            saw_select, 1.0f, juce::Colours::transparentBlack,
            saw_select, 1.0f, juce::Colours::transparentBlack,
            saw_select, 1.0f, juce::Colours::transparentBlack);
    }
    else if (audioProcessor.getSynth().getX() == 3) {
        audioProcessor.getSynth().setWaveTable(Waveform::Square);
        squareWaveButton.setImages(false, true, true,
            square_select, 1.0f, juce::Colours::transparentBlack,
            square_select, 1.0f, juce::Colours::transparentBlack,
            square_select, 1.0f, juce::Colours::transparentBlack);
    }
    else if (audioProcessor.getSynth().getX() == 4)
    {
        audioProcessor.getSynth().setWaveTable(Waveform::Triangle);
        triangleWaveButton.setImages(false, true, true,
            triangle_select, 1.0f, juce::Colours::transparentBlack,
            triangle_select, 1.0f, juce::Colours::transparentBlack,
            triangle_select, 1.0f, juce::Colours::transparentBlack);
    }
}