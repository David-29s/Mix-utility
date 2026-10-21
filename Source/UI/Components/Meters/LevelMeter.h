#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class LevelMeter : public juce::Component, public juce::SettableTooltipClient
{
public:

    LevelMeter();

    void setLevels(
        float peakLeftDb,
        float peakRightDb,
        float rmsLeftDb,
        float rmsRightDb);

    void paint(
        juce::Graphics& g) override;

    void mouseDown(
    const juce::MouseEvent& event) override;

    void mouseMove(
    const juce::MouseEvent& event) override;

    bool isPeakReadoutArea(
    juce::Point<int> position) const;

    void setLabel(const juce::String& label);

    void setScaleOnRight(bool shouldBeOnRight);

private:

    juce::String labelText = "INPUT";

    float displayedLevelDb = -60.0f;
    float peakReadoutDb = -60.0f;

    bool scaleOnRight = false;
    //==========================================================
    // Current levels
    //==========================================================

    float leftLevelDb  = -60.0f;
    float rightLevelDb = -60.0f;

    float leftRmsDb  = -60.0f;
    float rightRmsDb = -60.0f;

    float smoothedRmsLeftDb  = -60.0f;
    float smoothedRmsRightDb = -60.0f;

    //==========================================================
    // Peak hold
    //==========================================================

    float leftPeakDb  = -60.0f;
    float rightPeakDb = -60.0f;

    double leftPeakTimeMs = 0.0;
    double rightPeakTimeMs = 0.0;

    double lastUpdateTimeMs = 0.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelMeter)
};
