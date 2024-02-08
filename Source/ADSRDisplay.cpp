#include "ADSRDisplay.h"

ADSRDisplay::ADSRDisplay(Synth& synth)
    : synth(synth)
{
}

void ADSRDisplay::paint(juce::Graphics& g)
{
    auto width = static_cast<float>(getWidth());
    auto height = static_cast<float>(getHeight());

    auto attack = synth.getAttack();
    auto decay = synth.getDecay();
    auto sustain = synth.getSustain();
    auto release = synth.getRelease();

    auto total = attack + decay + sustain + release;

    float attackX = (attack / total) * width;
    float decayX = attackX + (decay / total) * width;
    float sustainX = decayX + (sustain / total) * width;
    float releaseX = sustainX + (release / total) * width;

    juce::Point<float> attackPoint(attackX, 0);
    juce::Point<float> decayPoint(decayX, height * (1.0f - sustain));
    juce::Point<float> sustainPoint(sustainX, decayPoint.getY());
    juce::Point<float> releasePoint(releaseX, height);

    // Создаем объект Path
    juce::Path path;
    path.startNewSubPath(0, height);
    path.lineTo(attackPoint.getX(), attackPoint.getY());
    path.lineTo(decayPoint.getX(), decayPoint.getY());
    path.lineTo(sustainPoint.getX(), sustainPoint.getY());
    path.lineTo(releasePoint.getX(), releasePoint.getY());
    path.lineTo(width, height);
    path.closeSubPath();

    // Заливаем путь цветом
    g.setColour(juce::Colour::fromString("32ECCA").withAlpha(0.2f));
    g.fillPath(path);

    g.setColour(juce::Colour::fromString("32ECCA").withAlpha(0.5f));
    g.drawLine(0, height, attackPoint.getX(), attackPoint.getY(), 4.0f);
    g.drawLine(attackPoint.getX(), attackPoint.getY(), decayPoint.getX(), decayPoint.getY(), 4.0f);
    g.drawLine(decayPoint.getX(), decayPoint.getY(), sustainPoint.getX(), sustainPoint.getY(), 4.0f); // Рисуем линию Sustain
    g.drawLine(sustainPoint.getX(), sustainPoint.getY(), releasePoint.getX(), releasePoint.getY(), 4.0f); // Рисуем линию Release

    g.setColour(juce::Colour::fromString("FFFFFF").withAlpha(0.9f));
    float radius = 8.0f;
    g.fillEllipse(attackPoint.getX() - radius, attackPoint.getY() - radius, 2 * radius, 2 * radius);
    g.fillEllipse(decayPoint.getX() - radius, decayPoint.getY() - radius, 2 * radius, 2 * radius);
    g.fillEllipse(sustainPoint.getX() - radius, sustainPoint.getY() - radius, 2 * radius, 2 * radius); // Рисуем круг на Sustain
    g.fillEllipse(releasePoint.getX() - radius, releasePoint.getY() - radius, 2 * radius, 2 * radius); // Рисуем круг внизу
}
