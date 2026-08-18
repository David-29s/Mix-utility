#include "CorrelationMeter.h"
#include "../../../Theme/NeonColours.h"
#include "../../../Metrics/CorrelationMetrics/CorrelationMetrics.h"

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
void CorrelationMeter::paint(juce::Graphics& g)
{
    auto bounds =
        getLocalBounds().toFloat();


    //==========================================================
    // Background
    //==========================================================

    g.setColour(
        NeonColours::correlationPanel.withAlpha(0.25f));

    g.fillRoundedRectangle(
        bounds,
        CorrelationMetrics::panelCornerRadius);


    //==========================================================
    // Border
    //==========================================================

    g.setColour(
        NeonColours::correlationGradientGreen.withAlpha(0.8f));

    g.drawRoundedRectangle(
        bounds.reduced(
            CorrelationMetrics::panelBorderInset),
        CorrelationMetrics::panelCornerRadius,
        CorrelationMetrics::panelBorderThickness);


    //==========================================================
    // Title
    //==========================================================

    auto titleArea =
        bounds.removeFromTop(
            CorrelationMetrics::titleHeight);

    g.setColour(
        NeonColours::correlationText);

    g.setFont(
        juce::FontOptions(
            CorrelationMetrics::titleFontSize));

    g.drawText(
        "CORRELATION",
        titleArea,
        juce::Justification::centred);


    //==========================================================
    // Scale
    //==========================================================

    const float sidePadding =
        CorrelationMetrics::scaleSidePadding;

    auto scaleArea =
        bounds.removeFromTop(
            CorrelationMetrics::scaleHeight);

    const float scaleX =
        scaleArea.getX() + sidePadding;

    const float scaleWidth =
        scaleArea.getWidth()
        - sidePadding * 2.0f;

    const float labelWidth =
        CorrelationMetrics::scaleLabelWidth;


    const float positions[] =
    {
        0.0f,
        0.25f,
        0.50f,
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


    g.setColour(
        NeonColours::correlationText.withAlpha(0.75f));

    g.setFont(
        juce::FontOptions(
            CorrelationMetrics::scaleFontSize));


    for (int i = 0; i < 5; ++i)
    {
        const float x =
            scaleX +
            positions[i] * scaleWidth;

        g.drawText(
            labels[i],
            x - labelWidth * 0.5f,
            scaleArea.getY()
                + CorrelationMetrics::scaleLabelOffsetY,
            labelWidth,
            scaleArea.getHeight(),
            juce::Justification::centred);
    }


    //==========================================================
    // Tick Marks
    //==========================================================

    const float tickAreaY =
        scaleArea.getBottom()
        + CorrelationMetrics::tickOffsetY;

    for (int i = 0;
         i < CorrelationMetrics::numberOfTicks;
         ++i)
    {
        const float normalized =
            static_cast<float>(i)
            / static_cast<float>(
                CorrelationMetrics::numberOfTicks - 1);

        const float x =
            scaleX +
            normalized * scaleWidth;

        const bool isMajor =
            (i % 4 == 0);

        const float tickHeight =
            isMajor
                ? CorrelationMetrics::majorTickHeight
                : CorrelationMetrics::minorTickHeight;

        const float tickWidth =
            isMajor
                ? CorrelationMetrics::majorTickWidth
                : CorrelationMetrics::minorTickWidth;

        g.setColour(
            isMajor
                ? NeonColours::correlationText.withAlpha(0.75f)
                : NeonColours::correlationTextSecondary.withAlpha(0.30f));

        const float currentTickY =
            isMajor
                ? tickAreaY
                : tickAreaY
                    + CorrelationMetrics::minorTickOffsetY;

        g.fillRect(
            x - tickWidth * 0.5f,
            currentTickY,
            tickWidth,
            tickHeight);
    }


    //==========================================================
    // Correlation Bar
    //==========================================================

    auto barArea =
        bounds.removeFromTop(
            CorrelationMetrics::barHeight);

    barArea.translate(
        CorrelationMetrics::barOffsetX,
        CorrelationMetrics::barOffsetY);

    barArea =
        barArea.reduced(
            sidePadding,
            0.0f);


    // Background
    g.setColour(
        NeonColours::correlationBarBackground);

    g.fillRoundedRectangle(
        barArea,
        CorrelationMetrics::barCornerRadius);


    //==========================================================
    // Gradient
    //==========================================================

    juce::ColourGradient gradient(
        NeonColours::correlationGradientRed,
        barArea.getX(),
        barArea.getCentreY(),

        NeonColours::correlationGradientGreen,
        barArea.getRight(),
        barArea.getCentreY(),

        false);


    gradient.addColour(
        0.25,
        NeonColours::correlationGradientOrange);

    gradient.addColour(
        0.50,
        NeonColours::correlationGradientYellow);

    gradient.addColour(
        0.75,
        NeonColours::correlationGradientGreen);


    g.setGradientFill(
        gradient);

    g.fillRoundedRectangle(
        barArea,
        CorrelationMetrics::barCornerRadius);


    //==========================================================
    // Correlation Marker
    //==========================================================

    const float normalized =
        juce::jmap(
            correlation,
            -1.0f,
            1.0f,
            0.0f,
            1.0f);


    const float markerX =
        barArea.getX()
        + normalized * barArea.getWidth();


    g.setColour(
        NeonColours::correlationMarker);

    g.fillRect(
        markerX
            - CorrelationMetrics::markerWidth * 0.5f,

        barArea.getY()
            - CorrelationMetrics::markerTopExtension,

        CorrelationMetrics::markerWidth,

        barArea.getHeight()
            + CorrelationMetrics::markerTopExtension
            + CorrelationMetrics::markerBottomExtension);


    //==========================================================
    // Zone Guide Lines
    //==========================================================

    const float guideY =
        barArea.getBottom()
        + CorrelationMetrics::guideStartOffset;


    const float outOfPhaseX =
        barArea.getX()
        + CorrelationMetrics::outOfPhasePosition
            * barArea.getWidth();

    const float lowX =
        barArea.getX()
        + CorrelationMetrics::lowPosition
            * barArea.getWidth();

    const float inPhaseX =
        barArea.getX()
        + CorrelationMetrics::inPhasePosition
            * barArea.getWidth();


    // OUT OF PHASE
    g.setColour(
        NeonColours::correlationOutOfPhase
            .withAlpha(0.75f));

    g.fillRect(
        outOfPhaseX
            - CorrelationMetrics::guideWidth * 0.5f,

        guideY,

        CorrelationMetrics::guideWidth,

        CorrelationMetrics::guideLength);


    // LOW
    g.setColour(
        NeonColours::correlationLow
            .withAlpha(0.75f));

    g.fillRect(
        lowX
            - CorrelationMetrics::guideWidth * 0.5f,

        guideY,

        CorrelationMetrics::guideWidth,

        CorrelationMetrics::guideLength);


    // IN PHASE
    g.setColour(
        NeonColours::correlationInPhase
            .withAlpha(0.75f));

    g.fillRect(
        inPhaseX
            - CorrelationMetrics::guideWidth * 0.5f,

        guideY,

        CorrelationMetrics::guideWidth,

        CorrelationMetrics::guideLength);


    //==========================================================
    // Zone Labels
    //==========================================================

    const float labelY =
        guideY
        + CorrelationMetrics::guideLength
        + 2.0f;


    // OUT OF PHASE
    g.setColour(
        NeonColours::correlationOutOfPhase
            .withAlpha(0.85f));

    g.setFont(
        juce::FontOptions(
            CorrelationMetrics::zoneLabelFontSize));

    g.drawText(
        "OUT OF PHASE",

        outOfPhaseX
            - CorrelationMetrics::zoneLabelWidth * 0.5f,

        labelY,

        CorrelationMetrics::zoneLabelWidth,

        CorrelationMetrics::zoneLabelHeight,

        juce::Justification::centred);


    // LOW
    g.setColour(
        NeonColours::correlationLow
            .withAlpha(0.85f));

    g.drawText(
        "LOW",

        lowX
            - CorrelationMetrics::zoneLabelWidth * 0.5f,

        labelY,

        CorrelationMetrics::zoneLabelWidth,

        CorrelationMetrics::zoneLabelHeight,

        juce::Justification::centred);


    // IN PHASE
    g.setColour(
        NeonColours::correlationInPhase
            .withAlpha(0.85f));

    g.drawText(
        "IN PHASE",

        inPhaseX
            - CorrelationMetrics::zoneLabelWidth * 0.5f,

        labelY,

        CorrelationMetrics::zoneLabelWidth,

        CorrelationMetrics::zoneLabelHeight,

        juce::Justification::centred);
}