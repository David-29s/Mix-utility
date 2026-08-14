#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class LevelMeter : public juce::Component
{
public:

    LevelMeter();

    void setLevels(
        float leftDb,
        float rightDb);

    void paint(
        juce::Graphics& g) override;

private:

    //==========================================================
    // Current levels
    //==========================================================

    float leftLevelDb  = -60.0f;
    float rightLevelDb = -60.0f;

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