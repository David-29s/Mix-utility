#pragma once

#include "PluginProcessor.h"
#include "UI/Components/NeonKnob/NeonKnob.h"
#include "Animation/NeonAnimator.h"
#include "UI/Buttons/IconToggleButton.h"
#include "UI/Buttons/ControlModeButton.h"
#include "UI/Components/Meters/LevelMeter.h"
#include "UI/Components/Meters/CorrelationMeter.h"
#include "UI/Components/BassMono/BassMono.h"

class AutoGainButton final : public juce::Button
{
public:
    enum class DisplayState
    {
        idle,
        listening,
        applying,
        noSignal
    };

    AutoGainButton();
    void setDisplayState(DisplayState newState);
    void paintButton(
        juce::Graphics& g,
        bool isMouseOverButton,
        bool isButtonDown) override;

private:
    DisplayState displayState = DisplayState::idle;
};

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

    juce::Slider outputTrimSlider;
    juce::Label outputTrimTitle;
    juce::Label outputTrimValue;

    std::unique_ptr<
        juce::AudioProcessorValueTreeState::SliderAttachment> outputTrimAttachment;

    //==========================================================
    // Buttons
    //==========================================================

    IconToggleButton bypassButton;

    IconToggleButton monoButton;

    IconToggleButton phaseButton;

    IconToggleButton muteButton;

    IconToggleButton abButton;

    juce::TextButton stereoSwapButton { "SWAP" };
    AutoGainButton autoGainButton;

    bool autoGainCapturing = false;
    float autoGainStartDb = 0.0f;
    float autoGainTargetDb = 0.0f;
    int autoGainAnimationFrames = 0;
    int autoGainFeedbackFrames = 0;
    int autoGainWaitFrames = 0;
    bool autoGainGestureActive = false;

    ControlModeButton controlModeButton;

    juce::TooltipWindow tooltipWindow { this, 650 };

    void setButtonPosition(
    IconToggleButton& button,
    int index);

    void refreshButtons();

    void updateKnobAttachment(
    ControlMode mode);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
