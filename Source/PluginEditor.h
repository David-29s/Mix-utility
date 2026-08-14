#pragma once

#include "PluginProcessor.h"
#include "UI/Components/NeonKnob/NeonKnob.h"
#include "Animation/NeonAnimator.h"
#include "UI/Buttons/IconToggleButton.h"
#include "UI/Buttons/ControlModeButton.h"
#include "UI/Components/Meters/LevelMeter.h"
#include "UI/Components/Meters/CorrelationMeter.h"
#include "UI/Components/BassMono/BassMono.h"


//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor,private juce::Timer
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;


    LevelMeter inputMeter;
    LevelMeter outputMeter;
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;



private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& processorRef;

    NeonKnob gainKnob { "GAIN" };

    CorrelationMeter correlationMeter;

    BassMono bassMono;

    void timerCallback() override;

    NeonAnimator animator;

    std::unique_ptr<
        juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;

    std::unique_ptr<
    juce::AudioProcessorValueTreeState::SliderAttachment> panAttachment;

    std::unique_ptr<
        juce::AudioProcessorValueTreeState::SliderAttachment> widthAttachment;

    //==========================================================
    // Buttons
    //==========================================================

    IconToggleButton bypassButton;

    IconToggleButton monoButton;

    IconToggleButton phaseButton;

    IconToggleButton muteButton;

    IconToggleButton abButton;

    ControlModeButton controlModeButton;

    void setButtonPosition(
    IconToggleButton& button,
    int index);

    void refreshButtons();

    void updateKnobAttachment(
    ControlMode mode);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
