#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class AudioPluginAudioProcessor;
class BassMono : public juce::Component
{
public:

    BassMono(AudioPluginAudioProcessor& processor);

    void paint(juce::Graphics& g) override;
    void resized() override;

    void mouseDown(const juce::MouseEvent& event) override;

private:

    AudioPluginAudioProcessor& processorRef;

    juce::ToggleButton enabledButton;

    bool listenEnabled = false;

    juce::Slider crossoverSlider;

    juce::Label crossoverLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BassMono)
};