#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class CorrelationMeter : public juce::Component
{
public:

    CorrelationMeter();

    void setCorrelation(float correlation);

    void paint(juce::Graphics& g) override;

private:

    float correlation = 0.0f;
    float targetCorrelation = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CorrelationMeter)
};