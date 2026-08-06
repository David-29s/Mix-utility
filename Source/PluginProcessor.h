#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "DSP/GainProcessor.h"



class AudioPluginAudioProcessor final : public juce::AudioProcessor
{
public:

    //==========================================================
    // Parameters
    //==========================================================
    juce::AudioProcessorValueTreeState& getAPVTS();

    //==========================================================
    // States
    //==========================================================

    // Activa o desactiva el bypass del plugin.
    void setBypassEnabled(bool shouldBypass);

    // Devuelve el estado actual del bypass.
    bool isBypassEnabled() const;

    //----------------------------------------------------------

    // Activa o desactiva el modo Mono.
    void setMonoEnabled(bool shouldBeMono);

    // Devuelve el estado actual del modo Mono.
    bool isMonoEnabled() const;

    //----------------------------------------------------------

    // Activa o desactiva la inversión de fase.
    void setPhaseEnabled(bool shouldBePhase);

    // Devuelve el estado actual de la fase.
    bool isPhaseEnabled() const;

    //----------------------------------------------------------

    // Activa o desactiva el mute.
    void setMuteEnabled(bool shouldBeMute);

    // Devuelve el estado actual del mute.
    bool isMuteEnabled() const;

    //==========================================================
    // Constructor / Destructor
    //==========================================================
    AudioPluginAudioProcessor();

    ~AudioPluginAudioProcessor() override;



    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:

    GainProcessor gainProcessor;

    juce::AudioProcessorValueTreeState apvts;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessor)
};
