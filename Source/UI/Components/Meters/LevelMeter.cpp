#include "LevelMeter.h"
#include "../../../Metrics/Meter/MeterMetrics.h"

//==========================================================
// Constructor
//==========================================================

LevelMeter::LevelMeter()
{
}

//==========================================================
// Set label
//==========================================================

void LevelMeter::setLabel(
    const juce::String& label)
{
    labelText = label;

    repaint();
}

//==========================================================
// Set scale side
//==========================================================

void LevelMeter::setScaleOnRight(bool shouldBeOnRight)
{
    scaleOnRight = shouldBeOnRight;

    repaint();
}

//==========================================================
// Set levels
//==========================================================

void LevelMeter::setLevels(
    float leftDb,
    float rightDb)
{
    leftLevelDb = leftDb;
    rightLevelDb = rightDb;

    const double currentTimeMs =
        juce::Time::getMillisecondCounterHiRes();

    //==========================================================
    // Calculate frame time
    //==========================================================

    const float deltaTimeSeconds =
        lastUpdateTimeMs > 0.0
            ? static_cast<float>(
                  currentTimeMs - lastUpdateTimeMs) / 1000.0f
            : 0.0f;

    lastUpdateTimeMs = currentTimeMs;

    //==========================================================
    // Left peak detection
    //==========================================================

    if (leftLevelDb > leftPeakDb)
    {
        leftPeakDb = leftLevelDb;
        leftPeakTimeMs = currentTimeMs;
    }

    //==========================================================
    // Right peak detection
    //==========================================================

    if (rightLevelDb > rightPeakDb)
    {
        rightPeakDb = rightLevelDb;
        rightPeakTimeMs = currentTimeMs;
    }

    //==========================================================
    // Left peak decay
    //==========================================================

    const double leftElapsedMs =
        currentTimeMs - leftPeakTimeMs;

    if (leftElapsedMs >
        MeterMetrics::peakHoldTimeMs)
    {
        leftPeakDb -=
            MeterMetrics::peakDecayDbPerSecond
            * deltaTimeSeconds;

        leftPeakDb =
            juce::jmax(
                leftLevelDb,
                leftPeakDb);
    }

    //==========================================================
    // Right peak decay
    //==========================================================

    const double rightElapsedMs =
        currentTimeMs - rightPeakTimeMs;

    if (rightElapsedMs >
        MeterMetrics::peakHoldTimeMs)
    {
        rightPeakDb -=
            MeterMetrics::peakDecayDbPerSecond
            * deltaTimeSeconds;

        rightPeakDb =
            juce::jmax(
                rightLevelDb,
                rightPeakDb);
    }

    repaint();
}

//==========================================================
// Paint
//==========================================================

void LevelMeter::paint(
    juce::Graphics& g)
{
    const float meterWidth =
        static_cast<float>(MeterMetrics::meterWidth);

    const float meterHeight =
        static_cast<float>(MeterMetrics::meterHeight);

    const float channelGap =
        static_cast<float>(MeterMetrics::channelGap);

    //======================================================
    // Meter area
    //======================================================

    const auto bounds =
        getLocalBounds().toFloat();

    const float meterOffset = scaleOnRight
      ? 0.0f
      : MeterMetrics::scaleWidth +
        MeterMetrics::labelGap;

    const float meterTotalWidth =
    meterWidth * 2.0f + channelGap;

    const auto meterBounds =
     juce::Rectangle<float>(
         meterOffset,
         static_cast<float>(MeterMetrics::labelTopHeight),
         meterTotalWidth,
         meterHeight);

    //======================================================
    // Meter background
    //======================================================

    g.setColour(
        juce::Colour(10, 15, 19));

    g.fillRoundedRectangle(
        meterBounds,
        3.0f);

    g.setColour(
        juce::Colour(45, 55, 60));

    g.drawRoundedRectangle(
        meterBounds,
        3.0f,
        1.0f);

    //==========================================================
    // dB scale
    //==========================================================

    constexpr float dbMarks[] =
    {
        12.0f,
        6.0f,
        0.0f,
        -6.0f,
        -12.0f,
        -18.0f,
        -24.0f,
        -30.0f,
        -36.0f,
        -42.0f,
        -48.0f,
        -54.0f,
        -60.0f
    };

    g.setFont(
        juce::FontOptions(12.0f));

    g.setColour(
        juce::Colours::white.withAlpha(0.75f));

    for (const float db : dbMarks)
    {
        const float normalized =
            juce::jmap(
                db,
                MeterMetrics::minDb,
                MeterMetrics::maxDb,
                1.0f,
                0.0f);

        const float y =
     static_cast<float>(MeterMetrics::labelTopHeight)
     + normalized * meterHeight;

        const juce::String text =
            juce::String(
                static_cast<int>(db));

        const float scaleX = scaleOnRight
    ? meterOffset + meterTotalWidth + MeterMetrics::labelGap
    : 0.0f;

        const auto scaleJustification = scaleOnRight
            ? juce::Justification::centredLeft
            : juce::Justification::centredRight;

        g.drawText(
            text,
            scaleX,
            juce::jlimit(
                0.0f,
                meterHeight + 36.0f, // cambiar la altura del numero -60
                y - 6.0f),
            MeterMetrics::scaleWidth,
            12.0f,
            scaleJustification);
    }

    //======================================================
    // Segment configuration
    //======================================================

    constexpr int numSegments = 30;
    constexpr float segmentGap = 2.0f;

    const float segmentHeight =
        (meterHeight -
         (segmentGap * (numSegments - 1)))
        / static_cast<float>(numSegments);

    //======================================================
    // Clamp levels
    //======================================================

    const float leftDb =
        juce::jlimit(
            MeterMetrics::minDb,
            MeterMetrics::maxDb,
            leftLevelDb);

    const float rightDb =
        juce::jlimit(
            MeterMetrics::minDb,
            MeterMetrics::maxDb,
            rightLevelDb);

    //======================================================
    // Convert dB to segment count
    //======================================================

    const float leftNormalized =
        juce::jmap(
            leftDb,
            MeterMetrics::minDb,
            MeterMetrics::maxDb,
            0.0f,
            1.0f);

    const float rightNormalized =
        juce::jmap(
            rightDb,
            MeterMetrics::minDb,
            MeterMetrics::maxDb,
            0.0f,
            1.0f);

    const int leftActiveSegments =
        static_cast<int>(
            std::ceil(
                leftNormalized * numSegments));

    const int rightActiveSegments =
        static_cast<int>(
            std::ceil(
                rightNormalized * numSegments));

    //======================================================
    // Draw one channel
    //======================================================

    auto drawChannel =
     [&](float x, int activeSegments, float levelDb)
     {
         for (int i = 0; i < numSegments; ++i)
         {
             const float y =
     meterBounds.getBottom()
     - segmentHeight
     - (i * (segmentHeight + segmentGap));

             juce::Rectangle<float> segment(
                 x,
                 y,
                 meterWidth,
                 segmentHeight);

             //==================================================
             // Inactive segment
             //==================================================

             g.setColour(
                 juce::Colour(
                     25,
                     30,
                     34));

             g.fillRoundedRectangle(
                 segment,
                 1.5f);

             //==================================================
             // Active segment
             //==================================================

             if (i < activeSegments)
             {
                 // Convert segment position to dB
                 const float segmentDb =
                     juce::jmap(
                         static_cast<float>(i),
                         0.0f,
                         static_cast<float>(numSegments - 1),
                         MeterMetrics::minDb,
                         MeterMetrics::maxDb);

                 juce::Colour segmentColour;

                 // Green
                 if (segmentDb < -12.0f)
                 {
                     segmentColour =
                         juce::Colours::limegreen;
                 }

                 // Yellow
                 else if (segmentDb < -6.0f)
                 {
                     segmentColour =
                         juce::Colours::yellow;
                 }

                 // Orange
                 else if (segmentDb < 0.0f)
                 {
                     segmentColour =
                         juce::Colours::orange;
                 }

                 // Red
                 else
                 {
                     segmentColour =
                         juce::Colours::red;
                 }

                 g.setColour(segmentColour);

                 g.fillRoundedRectangle(
                     segment,
                     1.5f);
             }
         }
     };

    //==========================================================
    // Draw INPUT label
    //==========================================================

    //==========================================================
    // Draw meter label
    //==========================================================

    g.setColour(
        juce::Colours::white);

    g.setFont(
        juce::FontOptions(13.0f)); // cambiar tamano input/output

    g.drawText(
        labelText,
        meterOffset,
        0.0f,
        MeterMetrics::inputLabelWidth,
        18.0f,
        juce::Justification::centred);

    //==========================================================
    // Draw current level
    //==========================================================

    const float currentLevelDb =
     juce::jmax(leftLevelDb, rightLevelDb);

    displayedLevelDb +=
        (currentLevelDb - displayedLevelDb) * 0.05f; //para suavisar el texto que muestra db

    const juce::String currentLevelText =
    juce::String(displayedLevelDb, 1) + " dB";

    g.setFont(
        juce::FontOptions(11.0f)); //Cambiar tamano texto de abajo

    g.setColour(
        juce::Colours::white.withAlpha(0.75f));

    g.drawText(
        currentLevelText,
        meterOffset - 5.0f ,
        17.0f,
        MeterMetrics::inputLabelWidth,
        18.0f,
        juce::Justification::centred);

    //==========================================================
    // Draw L/R
    //==========================================================

    const float leftX =
        meterOffset;

    const float rightX =
        meterOffset +
        meterWidth +
        channelGap;

    drawChannel(
        leftX,
        leftActiveSegments,
        leftDb);

    drawChannel(
        rightX,
        rightActiveSegments,
        rightDb);

    //==========================================================
    // Draw peak hold
    //==========================================================

    const float leftPeakNormalized =
        juce::jmap(
            juce::jlimit(
                MeterMetrics::minDb,
                MeterMetrics::maxDb,
                leftPeakDb),
            MeterMetrics::minDb,
            MeterMetrics::maxDb,
            0.0f,
            1.0f);

    const float rightPeakNormalized =
        juce::jmap(
            juce::jlimit(
                MeterMetrics::minDb,
                MeterMetrics::maxDb,
                rightPeakDb),
            MeterMetrics::minDb,
            MeterMetrics::maxDb,
            0.0f,
            1.0f);

    const float leftPeakY =
        meterBounds.getBottom()
        - (leftPeakNormalized * meterHeight);

    const float rightPeakY =
        meterBounds.getBottom()
        - (rightPeakNormalized * meterHeight);

    g.setColour(
        juce::Colours::white);

    g.fillRect(
        leftX,
        leftPeakY - 1.0f,
        meterWidth,
        2.0f);

    g.fillRect(
        rightX,
        rightPeakY - 1.0f,
        meterWidth,
        2.0f);

    //==========================================================
    // Draw L/R labels
    //==========================================================

    g.setColour(
        juce::Colours::white.withAlpha(0.75f));

    g.setFont(
        juce::FontOptions(13.0f));

    g.drawText(
        "L",
        leftX,
        meterBounds.getBottom(),
        meterWidth,
        MeterMetrics::labelBottomHeight,
        juce::Justification::centred);

    g.drawText(
        "R",
        rightX,
        meterBounds.getBottom(),
        meterWidth,
        MeterMetrics::labelBottomHeight,
        juce::Justification::centred);
}