#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../../LookAndFeel/NeonLookAndFeel.h"



class NeonKnob : public juce::Component, public juce::SettableTooltipClient
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

    void setMode(NeonLookAndFeel::KnobMode newMode);

private:

    void updateReadout();

    juce::String knobTitle;

    juce::Slider slider;

   juce::Slider wheelHelper;

    NeonLookAndFeel lookAndFeel;

    juce::Label valueLabel;
    juce::Label modeLabel;
    juce::TextEditor valueEditor;

    NeonLookAndFeel::KnobMode readoutMode = NeonLookAndFeel::KnobMode::gain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeonKnob)
};
