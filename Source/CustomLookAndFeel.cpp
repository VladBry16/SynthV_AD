#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
    // Загрузите изображение из BinaryData
    knobImage = juce::ImageCache::getFromMemory(BinaryData::Krutilka_png, BinaryData::Krutilka_pngSize);
}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
    const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    // Вычислите угол поворота на основе позиции слайдера
    float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // Поверните изображение
    juce::AffineTransform transform;
    transform = transform.rotation(angle, knobImage.getWidth() / 2.0f, knobImage.getHeight() / 2.0f);

    // Отрисуйте изображение
    g.drawImageTransformed(knobImage, transform, false);
}
