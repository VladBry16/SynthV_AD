#include "FilterDisplay.h"

FilterDisplay::FilterDisplay(Synth& synth)
    : synth(synth)
{
}

void FilterDisplay::paint(juce::Graphics& g)
{
    auto width = static_cast<float>(getWidth());
    auto height = static_cast<float>(getHeight());

    auto highPassFreq = synth.getLowPassFreq();
    auto lowPassFreq = synth.getHighPassFreq();

    float maxFreq = 20000.0f;
    float lowCutX = (lowPassFreq / maxFreq) * (width / 2);

    float highCutX = (width / 2) + ((highPassFreq / maxFreq) * (width / 2));

    juce::Point<float> lowCutPoint(lowCutX, height / 2.7);
    juce::Point<float> highCutPoint(width, height);
    juce::Point<float> topPoint(highCutX, height / 2.7);

    juce::Path path;
    path.startNewSubPath(0, height);
    path.lineTo(lowCutPoint.getX(), lowCutPoint.getY());
    path.lineTo(topPoint.getX(), topPoint.getY());
    path.lineTo(highCutPoint.getX(), highCutPoint.getY());
    path.lineTo(width, height);
    path.closeSubPath();

    g.setColour(juce::Colour::fromString("FFF96F").withAlpha(0.2f));
    g.fillPath(path);

    g.setColour(juce::Colour::fromString("FFF96F").withAlpha(0.5f));
    g.drawLine(0, height, lowCutPoint.getX(), lowCutPoint.getY(), 4.0f);
    g.drawLine(lowCutPoint.getX(), lowCutPoint.getY(), topPoint.getX(), topPoint.getY(), 4.0f);
    g.drawLine(topPoint.getX(), topPoint.getY(), highCutPoint.getX(), highCutPoint.getY(), 4.0f);

    g.setColour(juce::Colour::fromString("FFFFFF").withAlpha(0.9f));
    float radius = 8.0f;
    g.fillEllipse(lowCutPoint.getX() - radius, lowCutPoint.getY() - radius, 2 * radius, 2 * radius);
    g.fillEllipse(highCutPoint.getX() - radius, highCutPoint.getY() - radius, 2 * radius, 2 * radius);
    g.fillEllipse(topPoint.getX() - radius, topPoint.getY() - radius, 2 * radius, 2 * radius);
    g.fillEllipse(highCutPoint.getX() - radius - width, highCutPoint.getY() - radius, 2 * radius, 2 * radius);
}