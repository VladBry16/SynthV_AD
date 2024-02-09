#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
    knobImage = juce::ImageCache::getFromMemory(BinaryData::Krutilka_png, BinaryData::Krutilka_pngSize);
}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
    const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    juce::AffineTransform transform;
    transform = transform.rotation(angle, knobImage.getWidth() / 2.0f, knobImage.getHeight() / 2.0f);

    g.drawImageTransformed(knobImage, transform, false);
}
