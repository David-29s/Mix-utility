#include "CorrelationMeter.h"

CorrelationMeter::CorrelationMeter()
{
}

//==========================================================
// Set correlation
//==========================================================

void CorrelationMeter::setCorrelation(float newCorrelation)
{
    targetCorrelation = juce::jlimit(
        -1.0f,
        1.0f,
        newCorrelation);

    correlation +=
        (targetCorrelation - correlation) * 0.05f; //Suaviza el marcador 

    repaint();
}

//==========================================================
// Paint
//==========================================================

//==========================================================
// Paint
//==========================================================

void CorrelationMeter::paint(juce::Graphics& g)
{
    auto bounds =
        getLocalBounds().toFloat();

    //==========================================================
    // Background
    //==========================================================

    g.setColour(
        juce::Colours::black.withAlpha(0.25f));

    g.fillRoundedRectangle(
        bounds,
        8.0f);

    //==========================================================
    // Title
    //==========================================================

    auto titleArea =
        bounds.removeFromTop(24.0f);

    g.setColour(
        juce::Colours::white);

    g.setFont(
        juce::FontOptions(13.0f));

    g.drawText(
        "CORRELATION",
        titleArea,
        juce::Justification::centred);

    //==========================================================
    // Scale labels
    //==========================================================

    const float sidePadding = 12.0f;

    auto scaleArea =
        bounds.removeFromTop(22.0f);

    const float scaleX =
        scaleArea.getX() + sidePadding;

    const float scaleWidth =
        scaleArea.getWidth() -
        sidePadding * 2.0f;

    const float labelWidth = 30.0f;

    g.setColour(
        juce::Colours::white.withAlpha(0.75f));

    g.setFont(
        juce::FontOptions(12.0f));

    const float positions[] =
    {
        0.0f,
        0.25f,
        0.5f,
        0.75f,
        1.0f
    };

    const juce::String labels[] =
    {
        "-1",
        "-0.5",
        "0",
        "+0.5",
        "+1"
    };

    for (int i = 0; i < 5; ++i)
    {
        const float x =
            scaleX +
            positions[i] * scaleWidth;

        g.drawText(
            labels[i],
            x - labelWidth * 0.5f,
            scaleArea.getY(),
            labelWidth,
            scaleArea.getHeight(),
            juce::Justification::centred);
    }

    //==========================================================
    // Correlation bar
    //==========================================================

    const float barHeight = 18.0f;

    auto barArea =
        bounds.removeFromTop(barHeight);

    barArea =
        barArea.reduced(
            sidePadding,
            0.0f);

    // Background
    g.setColour(
        juce::Colours::black.withAlpha(0.35f));

    g.fillRoundedRectangle(
        barArea,
        2.0f);

    //==========================================================
    // Gradient
    //==========================================================

    juce::ColourGradient gradient(
        juce::Colours::red,
        barArea.getX(),
        barArea.getCentreY(),

        juce::Colours::green,
        barArea.getRight(),
        barArea.getCentreY(),
        false);

    gradient.addColour(
        0.25,
        juce::Colours::orange);

    gradient.addColour(
        0.50,
        juce::Colours::yellow);

    gradient.addColour(
        0.75,
        juce::Colours::green);

    g.setGradientFill(
        gradient);

    g.fillRoundedRectangle(
        barArea,
        2.0f);

    //==========================================================
    // Correlation marker
    //==========================================================

    const float normalized =
        juce::jmap(
            correlation,
            -1.0f,
            1.0f,
            0.0f,
            1.0f);

    const float markerX =
        barArea.getX() +
        normalized * barArea.getWidth();

    g.setColour(
        juce::Colours::white);

    g.fillRect(
        markerX - 1.5f,
        barArea.getY() - 5.0f,
        3.0f,
        barArea.getHeight() + 10.0f);
}