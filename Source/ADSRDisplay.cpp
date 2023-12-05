#include "ADSRDisplay.h"

ADSRDisplay::ADSRDisplay(Synth& synth)
    : synth(synth)
{
}

void ADSRDisplay::paint(juce::Graphics& g)
{
    auto width = static_cast<float>(getWidth());
    auto height = static_cast<float>(getHeight());

    // Ïîëó÷èòå ïàðàìåòðû ADSR
    auto attack = synth.getAttack();
    auto decay = synth.getDecay();
    auto sustain = synth.getSustain();
    auto release = synth.getRelease();

    // Ðàññ÷èòàéòå êîîðäèíàòû òî÷åê äëÿ ëèíèé ADSR
    juce::Point<float> attackPoint(attack * width, 0);
    juce::Point<float> decayPoint(attackPoint.getX() + decay * width, height * (1.0f - sustain));
    juce::Point<float> releasePoint(width, height * (1.0f - release));

    // Нарисуйте линии ADSR
    g.setColour(juce::Colours::lightgrey); // меняем цвет на светло-серый
    g.drawLine(0, height, attackPoint.getX(), attackPoint.getY(), 4.0f); // увеличиваем толщину линии до 4
    g.drawLine(attackPoint.getX(), attackPoint.getY(), decayPoint.getX(), decayPoint.getY(), 4.0f); // увеличиваем толщину линии до 4
    g.drawLine(decayPoint.getX(), decayPoint.getY(), releasePoint.getX(), decayPoint.getY(), 4.0f); // увеличиваем толщину линии до 4
    g.drawLine(releasePoint.getX(), decayPoint.getY(), releasePoint.getX(), releasePoint.getY(), 4.0f); // увеличиваем толщину линии до 4

    // Добавляем точки на концах линий
    g.setColour(juce::Colours::red.withAlpha(0.9f)); // меняем цвет на красный с прозрачностью 90%
    float radius = 10.0f; // увеличиваем радиус до 10
    g.fillEllipse(attackPoint.getX() - radius, attackPoint.getY() - radius, 2 * radius, 2 * radius); // рисуем круг радиусом 10 вокруг точки
    g.fillEllipse(decayPoint.getX() - radius, decayPoint.getY() - radius, 2 * radius, 2 * radius); // рисуем круг радиусом 10 вокруг точки
    g.fillEllipse(releasePoint.getX() - radius, releasePoint.getY() - radius, 2 * radius, 2 * radius); // рисуем круг радиусом 10 вокруг точки
}