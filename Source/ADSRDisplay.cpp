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

    float decayX = juce::jmax(attack, decay);
    float sustainX = juce::jmax(decayX, sustain);
    float releaseX = juce::jmax(sustainX, release);

    juce::Point<float> attackPoint(attack * width, 0);
    juce::Point<float> decayPoint(decayX * width, height * (1.0f - sustain));
    juce::Point<float> sustainPoint(sustainX * width, decayPoint.getY());
    juce::Point<float> releasePoint(releaseX * width, height);

    g.setColour(juce::Colours::cyan.withAlpha(0.5f));
    g.drawLine(0, height, attackPoint.getX(), attackPoint.getY(), 4.0f);
    g.drawLine(attackPoint.getX(), attackPoint.getY(), decayPoint.getX(), decayPoint.getY(), 4.0f);
    g.drawLine(decayPoint.getX(), decayPoint.getY(), sustainPoint.getX(), sustainPoint.getY(), 4.0f); // Рисуем линию Sustain
    g.drawLine(sustainPoint.getX(), sustainPoint.getY(), releasePoint.getX(), releasePoint.getY(), 4.0f); // Рисуем линию Release

    g.setColour(juce::Colours::red.withAlpha(0.9f));
    float radius = 10.0f;
    g.fillEllipse(attackPoint.getX() - radius, attackPoint.getY() - radius, 2 * radius, 2 * radius);
    g.fillEllipse(decayPoint.getX() - radius, decayPoint.getY() - radius, 2 * radius, 2 * radius);
    g.fillEllipse(sustainPoint.getX() - radius, sustainPoint.getY() - radius, 2 * radius, 2 * radius); // Рисуем круг на Sustain
    g.fillEllipse(releasePoint.getX() - radius, releasePoint.getY() - radius, 2 * radius, 2 * radius); // Рисуем круг внизу
}
