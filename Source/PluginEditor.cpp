#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "JuceHeader.h"
#include "CustomLookAndFeel.h"


SynthV_ADAudioProcessorEditor::SynthV_ADAudioProcessorEditor (SynthV_ADAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), adsrDisplay(p.getSynth()),
    attackLabel("A - Attack", "A - Attack"), decayLabel("D - Decay", "D - Decay"),
    sustainLabel("S - Sustain", "S - Sustain"), releaseLabel("R - Release", "R - Release"), 
    volumeLabel("Volume", "Volume"), keyboardComponent(p.getSynth().getKeyboardState(), juce::MidiKeyboardComponent::horizontalKeyboard), filterDisplay(p.getSynth()),
    lowPassLabel("LowPass", "LowPass"), highPassLabel("HighPass", "HighPass")
{
    customLookAndFeel = std::make_unique<CustomLookAndFeel>();

    attackSlider.setLookAndFeel(customLookAndFeel.get());
    decaySlider.setLookAndFeel(customLookAndFeel.get());
    sustainSlider.setLookAndFeel(customLookAndFeel.get());
    releaseSlider.setLookAndFeel(customLookAndFeel.get());
    volumeSlider.setLookAndFeel(customLookAndFeel.get());
    highPassFreqSlider.setLookAndFeel(customLookAndFeel.get());
    lowPassFreqSlider.setLookAndFeel(customLookAndFeel.get());

    depthSlider.setLookAndFeel(customLookAndFeel.get());
    frequencySlider.setLookAndFeel(customLookAndFeel.get());
    addAndMakeVisible(depthSlider);
    addAndMakeVisible(frequencySlider);
    depthSlider.setRange(0.0, 0.4);
    frequencySlider.setRange(0.1, 5.0);
    depthSlider.addListener(this);
    frequencySlider.addListener(this);
    depthSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    frequencySlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    highPassFreqSlider.setSliderStyle(juce::Slider::Rotary);
    highPassFreqSlider.addListener(this);
    addAndMakeVisible(&highPassFreqSlider);

    highPassFreqSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lowPassFreqSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    lowPassFreqSlider.setSliderStyle(juce::Slider::Rotary);
    lowPassFreqSlider.addListener(this);
    addAndMakeVisible(&lowPassFreqSlider);

    highPassFreqSlider.setRange(20.0, 20000.0, 0.1);
    highPassFreqSlider.setSkewFactorFromMidPoint(1000.0);
    lowPassFreqSlider.setRange(20.0, 20000.0, 0.1);
    lowPassFreqSlider.setSkewFactorFromMidPoint(1000.0);

    highPassFreqSlider.setValue(audioProcessor.getSynth().getHighPassFreq());
    lowPassFreqSlider.setValue(audioProcessor.getSynth().getLowPassFreq());

    attackSlider.setRange(0.0, 1.0);
    attackSlider.setValue(audioProcessor.getSynth().getAttack());
    decaySlider.setRange(0.1, 1.0);
    decaySlider.setValue(audioProcessor.getSynth().getDecay());
    sustainSlider.setRange(0.0, 1.0);
    sustainSlider.setValue(audioProcessor.getSynth().getSustain());
    releaseSlider.setRange(0.1, 1.0);
    releaseSlider.setValue(audioProcessor.getSynth().getRelease());

    volumeSlider.setRange(0.0, 1.0);
    volumeSlider.setValue(audioProcessor.getSynth().getVolume());
    volumeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(volumeSlider);
    volumeSlider.addListener(this);

    attackSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    decaySlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    sustainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    releaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    addAndMakeVisible(attackSlider);
    addAndMakeVisible(decaySlider);
    addAndMakeVisible(sustainSlider);
    addAndMakeVisible(releaseSlider);

    attackSlider.addListener(this);
    decaySlider.addListener(this);
    sustainSlider.addListener(this);
    releaseSlider.addListener(this);

    addAndMakeVisible(attackLabel);
    addAndMakeVisible(decayLabel);
    addAndMakeVisible(sustainLabel);
    addAndMakeVisible(releaseLabel);
    addAndMakeVisible(volumeLabel);
    addAndMakeVisible(lowPassLabel);
    addAndMakeVisible(highPassLabel);

    addAndMakeVisible(sineWaveButton);
    addAndMakeVisible(sawWaveButton);
    addAndMakeVisible(squareWaveButton);
    addAndMakeVisible(triangleWaveButton);

    selectWave();

    sineWaveButton.setRadioGroupId(1);
    sawWaveButton.setRadioGroupId(1);
    squareWaveButton.setRadioGroupId(1);
    triangleWaveButton.setRadioGroupId(1);

    sineWaveButton.addListener(this);
    sawWaveButton.addListener(this);
    squareWaveButton.addListener(this);
    triangleWaveButton.addListener(this);

    addAndMakeVisible(keyboardComponent);

    addAndMakeVisible(adsrDisplay);
    addAndMakeVisible(filterDisplay);

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
    int imageY = getHeight() - neon.getHeight();
    g.drawImageAt(neon, 0, imageY);
    juce::Image point_1 = juce::ImageCache::getFromMemory(BinaryData::Points_png, BinaryData::Points_pngSize);
    g.drawImageAt(point_1, 532, 260);
    juce::Image point_2 = juce::ImageCache::getFromMemory(BinaryData::Points_png, BinaryData::Points_pngSize);
    g.drawImageAt(point_2, 635, 260);
    juce::Image point_3 = juce::ImageCache::getFromMemory(BinaryData::Points_png, BinaryData::Points_pngSize);
    g.drawImageAt(point_3, 738, 260);
    juce::Image point_4 = juce::ImageCache::getFromMemory(BinaryData::Points_png, BinaryData::Points_pngSize);
    g.drawImageAt(point_4, 841, 260);
    juce::Image point_5 = juce::ImageCache::getFromMemory(BinaryData::Points_png, BinaryData::Points_pngSize);
    g.drawImageAt(point_5, 15, 400);
    juce::Image point_6 = juce::ImageCache::getFromMemory(BinaryData::Points_png, BinaryData::Points_pngSize);
    g.drawImageAt(point_6, 359, 160);
    juce::Image point_7 = juce::ImageCache::getFromMemory(BinaryData::Points_png, BinaryData::Points_pngSize);
    g.drawImageAt(point_7, 359, 266);
    juce::Image filter_pic = juce::ImageCache::getFromMemory(BinaryData::filter_png, BinaryData::filter_pngSize);
    g.drawImageAt(filter_pic, 70, 160);
}

void SynthV_ADAudioProcessorEditor::resized()
{
    int sliderSize = 80;
    int padding = 10;
    int labelHeight = 20;
    int buttonSize = 90;

    attackLabel.setBounds(532, 340, sliderSize, labelHeight);
    attackSlider.setSliderStyle(juce::Slider::Rotary);
    attackSlider.setBounds(532, 260, sliderSize, sliderSize);

    decayLabel.setBounds(635, 340, sliderSize, labelHeight);
    decaySlider.setSliderStyle(juce::Slider::Rotary);
    decaySlider.setBounds(635, 260, sliderSize, sliderSize);

    depthSlider.setBounds(100, 340, sliderSize, sliderSize);
    frequencySlider.setBounds(100, 320, sliderSize, sliderSize);

    highPassFreqSlider.setBounds(359, 160, sliderSize, sliderSize);
    lowPassFreqSlider.setBounds(359, 266, sliderSize, sliderSize);

    sustainLabel.setBounds(738, 340, sliderSize, labelHeight);
    sustainSlider.setSliderStyle(juce::Slider::Rotary);
    sustainSlider.setBounds(738, 260, sliderSize, sliderSize);

    releaseLabel.setBounds(841, 340, sliderSize, labelHeight);
    releaseSlider.setSliderStyle(juce::Slider::Rotary);
    releaseSlider.setBounds(841, 260, sliderSize, sliderSize);

    sineWaveButton.setBounds(59, 40, buttonSize, buttonSize);
    sawWaveButton.setBounds(158, 40, buttonSize, buttonSize);
    squareWaveButton.setBounds(257, 40, buttonSize, buttonSize);
    triangleWaveButton.setBounds(356, 40, buttonSize, buttonSize);

    volumeSlider.setSliderStyle(juce::Slider::Rotary);
    volumeSlider.setBounds(15, 400, sliderSize, sliderSize);

    volumeLabel.setBounds(15, 475, sliderSize, labelHeight);
    highPassLabel.setBounds(359, 340, sliderSize, labelHeight);
    lowPassLabel.setBounds(359, 235, sliderSize, labelHeight);

    auto bounds = getLocalBounds().removeFromBottom(84).reduced(100, 0);
    bounds.setX(150);

    keyboardComponent.setBounds(bounds);

    adsrDisplay.setBounds(564, 46, 330, 194);
    filterDisplay.setBounds(70, 160, 268, 183);
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
    if (slider == &highPassFreqSlider)
    {
        audioProcessor.getSynth().setHighPassFreq(highPassFreqSlider.getValue());
        filterDisplay.repaint();
    }
    if (slider == &lowPassFreqSlider)
    {
        audioProcessor.getSynth().setLowPassFreq(lowPassFreqSlider.getValue());
        filterDisplay.repaint();
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