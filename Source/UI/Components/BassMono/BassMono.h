#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class BassMono : public juce::Component
{
public:

    BassMono();

    void paint(juce::Graphics& g) override;
    void resized() override;

private:

    juce::ToggleButton enabledButton;

    juce::Slider crossoverSlider;

    juce::Label crossoverLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BassMono)
};