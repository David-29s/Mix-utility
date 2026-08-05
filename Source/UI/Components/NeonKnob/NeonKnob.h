#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../../LookAndFeel/NeonLookAndFeel.h"

class NeonKnob : public juce::Component
{
public:

    explicit NeonKnob(const juce::String& title);

    ~NeonKnob() override;

    void paint(juce::Graphics&) override;

    void resized() override;

    juce::Slider& getSlider();

private:

    juce::String knobTitle;

    juce::Slider slider;

    NeonLookAndFeel lookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeonKnob)
};