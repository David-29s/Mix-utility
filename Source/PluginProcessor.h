#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "DSP/GainProcessor.h"
#include "DSP/Metering/MeteringProcessor.h"



class AudioPluginAudioProcessor final : public juce::AudioProcessor
{
public:

    //==========================================================
    // Parameters
    //==========================================================
    juce::AudioProcessorValueTreeState& getAPVTS();

    void requestAutoGainCapture();
    void cancelAutoGainCapture();
    int getAutoGainCaptureState() const;
    float getAutoGainResultDb() const;
    void beginGainParameterGesture();
    void endGainParameterGesture();

    // Actualiza el parámetro Bypass del APVTS.
    void setBypassParameter(
        bool enabled);

    // Devuelve el valor actual del parámetro Bypass.
    bool getBypassParameter();

    void setMonoParameter(bool enabled);
    bool getMonoParameter();

    void setPhaseParameter(bool enabled);
    bool getPhaseParameter();

    void setMuteParameter(bool enabled);
    bool getMuteParameter();

    void setStereoSwapParameter(bool enabled);
    bool getStereoSwapParameter();

    void setBassMonoParameter(bool enabled);
    bool getBassMonoParameter();

    void setBassMonoCrossoverParameter(float frequency);
    float getBassMonoCrossoverParameter();

    void setBassMonoListen(bool shouldListen);

    bool getBassMonoListenParameter();

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
    // Metering
    //==========================================================

    float getInputLevelL() const;
    float getInputLevelR() const;

    float getOutputLevelL() const;
    float getOutputLevelR() const;

    float getInputRmsL() const;
    float getInputRmsR() const;

    float getOutputRmsL() const;
    float getOutputRmsR() const;

    float getOutputCorrelation() const;

    //==========================================================
    // A/B Compare
    //==========================================================

    // Cambia entre los bancos A y B.
    void toggleCompareState();

    // Devuelve true cuando el banco activo es A.
    bool isUsingStateA() const;

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

    //==========================================================
    // Processing
    //==========================================================

    // Actualiza todos los parámetros del DSP a partir del APVTS.
    void updateParameters();

    //==========================================================
    // DSP
    //==========================================================

    GainProcessor gainProcessor;

    MeteringProcessor meteringProcessor;

    //==========================================================
    // Parameters
    //==========================================================

    juce::AudioProcessorValueTreeState apvts;


    //==========================================================
    // Bancos A/B
    //==========================================================

    // Banco A.
    juce::ValueTree stateA;

    // Banco B.
    juce::ValueTree stateB;

    // Indica cuál banco está activo.
    bool usingStateA = true;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessor)
};
