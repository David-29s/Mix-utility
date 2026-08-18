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

bool LevelMeter::isPeakReadoutArea(
    juce::Point<int> position) const
{
    const float meterOffset = scaleOnRight
     ? 0.0f
     : MeterMetrics::scaleWidth +
       MeterMetrics::labelGap;

    const float readoutOffsetX = scaleOnRight
        ? MeterMetrics::outputReadoutOffsetX
        : MeterMetrics::inputReadoutOffsetX;

    const juce::Rectangle<int> readoutBounds(
        static_cast<int>(meterOffset + readoutOffsetX),
        17,
        MeterMetrics::inputLabelWidth,
        18);

    return readoutBounds.contains(position);
}

void LevelMeter::mouseDown(
    const juce::MouseEvent& event)
{
    if (!isPeakReadoutArea(event.getPosition()))
        return;

    peakReadoutDb = MeterMetrics::minDb;

    leftPeakDb = MeterMetrics::minDb;
    rightPeakDb = MeterMetrics::minDb;

    repaint();
}

//==========================================================
// Set levels
//==========================================================

void LevelMeter::setLevels(
    float peakLeftDb,
    float peakRightDb,
    float rmsLeftDb,
    float rmsRightDb)
{
    leftLevelDb = peakLeftDb;
    rightLevelDb = peakRightDb;

    //==========================================================
    // Peak readout
    //==========================================================

    const float currentPeakDb =
        juce::jmax(
            leftLevelDb,
            rightLevelDb);

    peakReadoutDb =
        juce::jmax(
            peakReadoutDb,
            currentPeakDb);

    this->leftRmsDb = rmsLeftDb;
    this->rightRmsDb = rmsRightDb;

    //==========================================================
    // Smooth RMS for meter display
    //==========================================================

    const float rmsRiseSpeed = 0.35f;
    const float rmsFallSpeed = 0.15f;

    const float leftSpeed =
        leftRmsDb > smoothedRmsLeftDb
            ? rmsRiseSpeed
            : rmsFallSpeed;

    const float rightSpeed =
        rightRmsDb > smoothedRmsRightDb
            ? rmsRiseSpeed
            : rmsFallSpeed;

    smoothedRmsLeftDb +=
    (leftRmsDb - smoothedRmsLeftDb)
    * leftSpeed;

    smoothedRmsRightDb +=
        (rightRmsDb - smoothedRmsRightDb)
        * rightSpeed;

    // Snap al mínimo cuando ya no hay señal
    if (smoothedRmsLeftDb < MeterMetrics::minDb + 0.5f)
        smoothedRmsLeftDb = MeterMetrics::minDb;

    if (smoothedRmsRightDb < MeterMetrics::minDb + 0.5f)
        smoothedRmsRightDb = MeterMetrics::minDb;

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
    // Displayed level
    //==========================================================

    if (leftLevelDb > displayedLevelDb ||
        rightLevelDb > displayedLevelDb)
    {
        const float currentLevelDb =
            juce::jmax(
                leftLevelDb,
                rightLevelDb);

        displayedLevelDb +=
            MeterMetrics::labelRiseDbPerSecond
            * deltaTimeSeconds;

        displayedLevelDb =
            juce::jmin(
                displayedLevelDb,
                currentLevelDb);
    }
    else
    {
        const float currentLevelDb =
            juce::jmax(
                leftLevelDb,
                rightLevelDb);

        displayedLevelDb -=
            MeterMetrics::labelFallDbPerSecond
            * deltaTimeSeconds;

        displayedLevelDb =
            juce::jmax(
                displayedLevelDb,
                currentLevelDb);
    }

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
    constexpr float segmentGap = 0.0f;

    const float segmentHeight =
        (meterHeight -
         (segmentGap * (numSegments - 1)))
        / static_cast<float>(numSegments);

    //======================================================
    // Clamp RMS levels
    //======================================================

    const float leftDb =
        juce::jlimit(
            MeterMetrics::minDb,
            MeterMetrics::maxDb,
            smoothedRmsLeftDb);

    const float rightDb =
        juce::jlimit(
            MeterMetrics::minDb,
            MeterMetrics::maxDb,
            smoothedRmsRightDb);

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
    // Convert Peak to segment count
    //======================================================

    const float leftPeakNormalizedCurrent =
        juce::jmap(
            juce::jlimit(
                MeterMetrics::minDb,
                MeterMetrics::maxDb,
                leftLevelDb),
            MeterMetrics::minDb,
            MeterMetrics::maxDb,
            0.0f,
            1.0f);

    const float rightPeakNormalizedCurrent =
        juce::jmap(
            juce::jlimit(
                MeterMetrics::minDb,
                MeterMetrics::maxDb,
                rightLevelDb),
            MeterMetrics::minDb,
            MeterMetrics::maxDb,
            0.0f,
            1.0f);

    const int leftPeakSegments =
        static_cast<int>(
            std::ceil(
                leftPeakNormalizedCurrent * numSegments));

    const int rightPeakSegments =
        static_cast<int>(
            std::ceil(
                rightPeakNormalizedCurrent * numSegments));

    //======================================================
// Draw one channel
//======================================================

auto drawChannel =
    [&](float x,
        int rmsSegments,
        int peakSegments,
        float levelDb)
{
    //==================================================
    // Channel background
    //==================================================

    const juce::Rectangle<float> channelBounds(
        x,
        meterBounds.getY(),
        meterWidth,
        meterHeight);

    g.setColour(
        juce::Colour(25, 30, 34));

        g.fillRoundedRectangle(
        channelBounds,
        2.0f);


        //==================================================
        // RMS bar
        //==================================================

        if (rmsSegments > 0)
        {
            const float rmsHeight =
                static_cast<float>(rmsSegments)
                * (segmentHeight + segmentGap)
                - segmentGap;

            const float rmsY =
                meterBounds.getBottom()
                - rmsHeight;

            const juce::Rectangle<float> rmsBounds(
                x,
                rmsY,
                meterWidth,
                rmsHeight);

            // RMS = verde fuerte
            g.setColour(
                juce::Colour(0, 255, 70));

            g.fillRoundedRectangle(
                rmsBounds,
                2.0f);
        }

    //==================================================
    // Peak overlay
    //==================================================

    if (peakSegments > rmsSegments)
    {
        const float peakHeight =
            static_cast<float>(
                peakSegments - rmsSegments)
            * (segmentHeight + segmentGap);

        const float peakY =
            meterBounds.getBottom()
            - static_cast<float>(peakSegments)
              * (segmentHeight + segmentGap);

        const juce::Rectangle<float> peakBounds(
            x,
            peakY,
            meterWidth,
            peakHeight);

        g.setColour(
            juce::Colours::limegreen.withAlpha(0.6f));

        g.fillRect(peakBounds);
    }
};


    //==========================================================
    // Draw meter label
    //==========================================================

    g.setColour(
        juce::Colours::white);

    g.setFont(
        juce::FontOptions(12.0f)); // cambiar tamano input/output

    const float labelOffsetX = scaleOnRight
    ? MeterMetrics::outputLabelOffsetX
    : MeterMetrics::inputLabelOffsetX;

    const float labelOffsetY = scaleOnRight
    ? MeterMetrics::outputLabelOffsetY
    : MeterMetrics::inputLabelOffsetY;

    g.drawText(
        labelText,
        meterOffset + labelOffsetX,
        labelOffsetY,
        MeterMetrics::inputLabelWidth,
        18.0f,
        juce::Justification::centred);

    //==========================================================
    // Draw current level
    //==========================================================

    const juce::String currentLevelText =
        juce::String(peakReadoutDb, 1) + " dB";

    g.setFont(
        juce::FontOptions(11.0f)); //Cambiar tamano de los numeros de la etiqueta

    const float readoutOffsetX = scaleOnRight
        ? MeterMetrics::outputReadoutOffsetX
        : MeterMetrics::inputReadoutOffsetX;

    const bool isClipping = peakReadoutDb > 0.0f;

    //==========================================================
    // Readout background
    //==========================================================

    g.setColour(
        isClipping
            ? juce::Colour(220, 45, 45)
            : juce::Colour(10, 15, 19));

    g.fillRoundedRectangle(
        meterOffset + readoutOffsetX,
        17.0f,
        static_cast<float>(MeterMetrics::inputLabelWidth),
        18.0f,
        3.0f);

    //==========================================================
    // Readout text
    //==========================================================

    g.setColour(
        juce::Colours::white);

    g.drawText(
        currentLevelText,
        meterOffset + readoutOffsetX,
        17.0f,
        MeterMetrics::inputLabelWidth,
        18.0f,
        juce::Justification::centred);

    //==========================================================
    // Readout border
    //==========================================================

    g.setColour(
        isClipping
            ? juce::Colour(255, 70, 70)
            : juce::Colours::white.withAlpha(0.1f));

    g.drawRoundedRectangle(
        meterOffset + readoutOffsetX,
        17.0f,
        static_cast<float>(MeterMetrics::inputLabelWidth),
        18.0f,
        3.0f,
        0.50f);
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
     leftPeakSegments,
     leftDb);

    drawChannel(
        rightX,
        rightActiveSegments,
        rightPeakSegments,
        rightDb);


    //==========================================================
    // Draw current peak
    //==========================================================


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

void LevelMeter::mouseMove(
    const juce::MouseEvent& event)
{
    if (isPeakReadoutArea(event.getPosition()))
    {
        setMouseCursor(
            juce::MouseCursor::PointingHandCursor);
    }
    else
    {
        setMouseCursor(
            juce::MouseCursor::NormalCursor);
    }
}