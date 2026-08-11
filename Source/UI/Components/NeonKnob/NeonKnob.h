#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../../LookAndFeel/NeonLookAndFeel.h"


class WheelHelperSlider : public juce::Slider
{
public:

    std::function<void(const juce::MouseEvent&, const juce::MouseWheelDetails&)> onWheel;

    void mouseWheelMove(
        const juce::MouseEvent& event,
        const juce::MouseWheelDetails& wheel) override
    {
        if (onWheel != nullptr)
            onWheel(event, wheel);
    }
};

class NeonKnob : public juce::Component
{
public:

    explicit NeonKnob(const juce::String& title);

    ~NeonKnob() override;

    void paint(juce::Graphics&) override;

    void resized() override;

    void mouseDoubleClick(
    const juce::MouseEvent& event) override;


    juce::Slider& getSlider();

    NeonLookAndFeel& getLookAndFeel();

    void setTitle(const juce::String& newTitle);

    void setUnit(const juce::String& newUnit);

private:

    juce::String knobTitle;

    juce::String knobUnit = "dB";

    juce::Slider slider;

    WheelHelperSlider wheelHelper;

    NeonLookAndFeel lookAndFeel;

    juce::Label valueLabel;
    juce::TextEditor valueEditor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeonKnob)
};