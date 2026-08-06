#pragma once

#include "PluginProcessor.h"
#include "UI/Components/NeonKnob/NeonKnob.h"
#include "Animation/NeonAnimator.h"
#include "UI/Buttons/IconToggleButton.h"

//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor,private juce::Timer
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;



private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& processorRef;

    NeonKnob gainKnob { "GAIN" };

    void timerCallback() override;

    NeonAnimator animator;

    std::unique_ptr<
        juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;

    //==========================================================
    // Buttons
    //==========================================================

    IconToggleButton bypassButton;

    IconToggleButton monoButton;

    IconToggleButton phaseButton;

    IconToggleButton muteButton;

    IconToggleButton abButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
