#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <iostream>


//==========================================================
// Parameters
//==========================================================

// Crea y registra todos los parámetros del plugin.
// Cada parámetro agregado aquí podrá ser utilizado por
// el DSP, la interfaz gráfica y guardarse junto con el preset.
juce::AudioProcessorValueTreeState::ParameterLayout
AudioPluginAudioProcessor::createParameterLayout()
{
    //==========================================================
    // Contenedor de parámetros
    //==========================================================

    // Almacena todos los parámetros que tendrá el plugin.
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    //==========================================================
    // Gain
    //==========================================================

    // Agrega el parámetro de ganancia.
    layout.add(
        std::make_unique<juce::AudioParameterFloat>(

            "GAIN",     // ID interno (único).

            "Gain",     // Nombre visible.

            // Rango permitido:
            // Mínimo, máximo y resolución.
            juce::NormalisableRange<float>(
                -24.0f,
                 24.0f,
                  0.01f),

            // Valor inicial.
            0.0f
        ));

    //==========================================================
    // Pan
    //==========================================================

    // -100 = izquierda
    //    0 = centro
    // +100 = derecha

    layout.add(
        std::make_unique<juce::AudioParameterFloat>(
            "PAN",
            "Pan",

            juce::NormalisableRange<float>(
                -100.0f,
                 100.0f,
                 0.01f),

            0.0f
        ));


    //==========================================================
    // Width
    //==========================================================

    layout.add(
        std::make_unique<juce::AudioParameterFloat>(
            "WIDTH",
            "Width",

            []()
            {
                juce::NormalisableRange<float> range(
                    0.0f,
                    400.0f,
                    0.01f);

                range.setSkewForCentre(100.0f);

                return range;
            }(),

            100.0f
        ));

    //==========================================================
    // Bypass
    //==========================================================

    // Agrega el parámetro de bypass.
    layout.add(
        std::make_unique<juce::AudioParameterBool>(

            "BYPASS",   // ID interno (único).

            "Bypass",   // Nombre visible.

            // Valor inicial.
            false
        ));

    //==========================================================
    // Mono
    //==========================================================

    layout.add(
        std::make_unique<juce::AudioParameterBool>(
            "MONO",
            "Mono",
            false));

    //==========================================================
    // Phase
    //==========================================================

    layout.add(
        std::make_unique<juce::AudioParameterBool>(
            "PHASE",
            "Phase",
            false));

    //==========================================================
    // Mute
    //==========================================================

    layout.add(
        std::make_unique<juce::AudioParameterBool>(
            "MUTE",
            "Mute",
            false));

    //==========================================================
    // Return
    //==========================================================

    //==========================================================
    // Bass Mono
    //==========================================================

    layout.add(
        std::make_unique<juce::AudioParameterBool>(
            "BASS_MONO",
            "Bass Mono",
            false));

    layout.add(
        std::make_unique<juce::AudioParameterFloat>(
            "BASS_MONO_CROSSOVER",
            "Bass Mono Crossover",

            juce::NormalisableRange<float>(
                20.0f,
                500.0f,
                0.01f),

            150.0f));

    // Devuelve todos los parámetros registrados.
    return layout;
}
//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
      apvts(*this,
            nullptr,
            "Parameters",
            createParameterLayout())
{

    //==========================================================
    // Inicializar bancos A/B
    //==========================================================

    stateA = apvts.copyState();
    stateB = apvts.copyState();

}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
}

juce::AudioProcessorValueTreeState&
AudioPluginAudioProcessor::getAPVTS()
{
    return apvts;
}

//==========================================================
// States
//==========================================================

// Activa o desactiva el bypass del procesador.
void AudioPluginAudioProcessor::setBypassEnabled(
    bool shouldBypass)
{
    gainProcessor.setBypass(
        shouldBypass);
}

// Devuelve el estado actual del bypass.
bool AudioPluginAudioProcessor::isBypassEnabled() const
{
    return gainProcessor.isBypassed();
}

//----------------------------------------------------------

// Activa o desactiva el modo Mono.
void AudioPluginAudioProcessor::setMonoEnabled(
    bool shouldBeMono)
{
    gainProcessor.setMono(
        shouldBeMono);
}

// Devuelve el estado actual del modo Mono.
bool AudioPluginAudioProcessor::isMonoEnabled() const
{
    return gainProcessor.isMono();
}

//----------------------------------------------------------

// Activa o desactiva la inversión de fase.
void AudioPluginAudioProcessor::setPhaseEnabled(
    bool shouldBePhase)
{
    gainProcessor.setPhase(
        shouldBePhase);
}

// Devuelve el estado actual de la fase.
bool AudioPluginAudioProcessor::isPhaseEnabled() const
{
    return gainProcessor.isPhase();
}

//----------------------------------------------------------

// Activa o desactiva el mute.
void AudioPluginAudioProcessor::setMuteEnabled(
    bool shouldBeMute)
{
    gainProcessor.setMute(
        shouldBeMute);
}

// Devuelve el estado actual del mute.
bool AudioPluginAudioProcessor::isMuteEnabled() const
{
    return gainProcessor.isMuted();
}

//==========================================================
// Metering
//==========================================================

float AudioPluginAudioProcessor::getInputLevelL() const
{
    return meteringProcessor.getInputLevelL();
}

float AudioPluginAudioProcessor::getInputLevelR() const
{
    return meteringProcessor.getInputLevelR();
}


float AudioPluginAudioProcessor::getOutputLevelL() const
{
    return meteringProcessor.getOutputLevelL();
}

float AudioPluginAudioProcessor::getOutputLevelR() const
{
    return meteringProcessor.getOutputLevelR();
}

float AudioPluginAudioProcessor::getOutputCorrelation() const
{
    return meteringProcessor.getOutputCorrelation();
}

//==========================================================
// A/B Compare
//==========================================================

// Cambia entre los bancos A y B.
void AudioPluginAudioProcessor::toggleCompareState()
{
    //==========================================================
    // Banco A -> Banco B
    //==========================================================

    if (usingStateA)
    {
        // Guardar estado actual en A.
        stateA = apvts.copyState();

        // Cargar estado de B.
        if (stateB.isValid())
            apvts.replaceState(
                stateB.createCopy());
    }

    //==========================================================
    // Banco B -> Banco A
    //==========================================================

    else
    {
        // Guardar estado actual en B.
        stateB = apvts.copyState();

        // Cargar estado de A.
        if (stateA.isValid())
            apvts.replaceState(
                stateA.createCopy());
    }

    //==========================================================
    // Cambiar banco activo
    //==========================================================

    usingStateA = !usingStateA;

    //==========================================================
    // Actualizar DSP
    //==========================================================

    updateParameters();
}

//----------------------------------------------------------

// Devuelve true cuando el banco activo es A.
bool AudioPluginAudioProcessor::isUsingStateA() const
{
    return usingStateA;
}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String AudioPluginAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay (
    double sampleRate,
    int samplesPerBlock)
{
   

    gainProcessor.prepare(
    sampleRate,
    samplesPerBlock);
}

void AudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

//==========================================================
// Processing
//==========================================================

// Procesa cada bloque de audio recibido desde el DAW.
void AudioPluginAudioProcessor::processBlock(
    juce::AudioBuffer<float>& buffer,
    juce::MidiBuffer& midiMessages)
{
    //==========================================================
    // MIDI
    //==========================================================

    // Este plugin no utiliza mensajes MIDI.
    juce::ignoreUnused(
        midiMessages);

    //==========================================================
    // CPU
    //==========================================================

    // Evita problemas de rendimiento causados por números
    // muy pequeños (denormals) durante el procesamiento.
    juce::ScopedNoDenormals noDenormals;

    //==========================================================
    // Audio Channels
    //==========================================================

    // Obtiene la cantidad de canales de entrada y salida.
    auto totalNumInputChannels =
        getTotalNumInputChannels();

    auto totalNumOutputChannels =
        getTotalNumOutputChannels();

    //==========================================================
    // Clear Unused Channels
    //==========================================================

    // Limpia cualquier canal de salida que no tenga una
    // entrada correspondiente.
    for (auto i = totalNumInputChannels;
         i < totalNumOutputChannels;
         ++i)
    {
        buffer.clear(
            i,
            0,
            buffer.getNumSamples());
    }

    //==========================================================
    // Parameters
    //==========================================================

    // Actualiza todos los parámetros del DSP.
    updateParameters();


    //==========================================================
    // Metering
    //==========================================================

    // Mide el nivel de la señal de entrada
    // antes de aplicar cualquier procesamiento.
    meteringProcessor.processInput(
        buffer);

    //==========================================================
    // DSP
    //==========================================================

    // Procesa el bloque completo de audio.
    gainProcessor.process(
        buffer);

    // Output metering
    meteringProcessor.processOutput(buffer);
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor (*this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation(
    juce::MemoryBlock& destData)
{
    //==========================================================
    // Guardar el estado actual en el banco activo
    //==========================================================

    if (usingStateA)
        stateA = apvts.copyState();
    else
        stateB = apvts.copyState();

    //==========================================================
    // Estado principal
    //==========================================================

    auto state = apvts.copyState();

    //==========================================================
    // Banco A
    //==========================================================

    juce::ValueTree compareA("CompareA");

    if (stateA.isValid())
    {
        compareA.appendChild(
            stateA.createCopy(),
            nullptr);
    }

    state.appendChild(
        compareA,
        nullptr);

    //==========================================================
    // Banco B
    //==========================================================

    juce::ValueTree compareB("CompareB");

    if (stateB.isValid())
    {
        compareB.appendChild(
            stateB.createCopy(),
            nullptr);
    }

    state.appendChild(
        compareB,
        nullptr);

    //==========================================================
    // Banco activo
    //==========================================================

    state.setProperty(
    "usingStateA",
    usingStateA,
    nullptr);

    //==========================================================
    // Crear XML
    //==========================================================

    std::unique_ptr<juce::XmlElement> xml(
        state.createXml());

    copyXmlToBinary(
        *xml,
        destData);
}

void AudioPluginAudioProcessor::setStateInformation(
    const void* data,
    int sizeInBytes)
{
    //==========================================================
    // Obtener XML
    //==========================================================

    std::unique_ptr<juce::XmlElement> xmlState(
        getXmlFromBinary(
            data,
            sizeInBytes));

    if (xmlState == nullptr)
        return;

    //==========================================================
    // Convertir XML a ValueTree
    //==========================================================

    auto state =
        juce::ValueTree::fromXml(
            *xmlState);

    if (!state.isValid())
        return;

    //==========================================================
    // Restaurar Banco A
    //==========================================================

    auto compareA =
        state.getChildWithName("CompareA");

    if (compareA.isValid()
        && compareA.getNumChildren() > 0)
    {
        stateA =
            compareA.getChild(0).createCopy();
    }

    //==========================================================
    // Restaurar Banco B
    //==========================================================

    auto compareB =
        state.getChildWithName("CompareB");

    if (compareB.isValid()
        && compareB.getNumChildren() > 0)
    {
        stateB =
            compareB.getChild(0).createCopy();
    }

    //==========================================================
    // Al abrir el proyecto siempre comenzar en A
    //==========================================================

    usingStateA =
       state.getProperty(
           "usingStateA",
           true);

    if (usingStateA)
    {
        if (stateA.isValid())
            apvts.replaceState(
                stateA.createCopy());
    }
    else
    {
        if (stateB.isValid())
            apvts.replaceState(
                stateB.createCopy());
    }

    //==========================================================
    // Actualizar DSP
    //==========================================================

    updateParameters();
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}

//==========================================================
// Processing
//==========================================================

// Actualiza todos los parámetros del DSP a partir del APVTS.
void AudioPluginAudioProcessor::updateParameters()
{
    //==========================================================
    // Gain
    //==========================================================

    // Obtiene el valor actual del parámetro Gain.
    auto* gainValue =
        apvts.getRawParameterValue(
            "GAIN");

    // Convierte el valor atómico en un float.
    const float gainDB =
        gainValue->load();

    // Convierte decibeles a ganancia lineal.
    const float linearGain =
        juce::Decibels::decibelsToGain(
            gainDB);

    // Envía la ganancia al procesador DSP.
    gainProcessor.setGain(
        linearGain);

    //==========================================================
    // Pan
    //==========================================================

    // Obtiene el valor de PAN desde APVTS.
    // Rango del parámetro:
    // -100 = izquierda
    //    0 = centro
    // +100 = derecha

    auto* panValue =
        apvts.getRawParameterValue(
            "PAN");

    const float panPercent =
        panValue->load();

    // Convierte:
    // -100 ... +100
    //
    // a:
    // -1.0 ... +1.0

    const float panNormalized =
        panPercent / 100.0f;

    // Envía el valor al DSP.
    gainProcessor.setPan(
        panNormalized);

    //==========================================================
    // Width
    //==========================================================

    auto* widthValue =
    apvts.getRawParameterValue("WIDTH");

    const float widthPercent =
        widthValue->load();

    const float widthNormalized =
        widthPercent / 100.0f;

    gainProcessor.setWidth(
        widthNormalized);

    //==========================================================
    // Bypass
    //==========================================================

    // Obtiene el valor actual del parámetro Bypass.
    auto* bypassValue =
        apvts.getRawParameterValue(
            "BYPASS");

    // Convierte el valor atómico en un bool.
    const bool bypassEnabled =
        bypassValue->load() > 0.5f;

    // Envía el estado al procesador DSP.
    gainProcessor.setBypass(
        bypassEnabled);

    //==========================================================
    // Mono
    //==========================================================

    auto* monoValue =
        apvts.getRawParameterValue("MONO");

    const bool monoEnabled =
        monoValue->load() > 0.5f;

    gainProcessor.setMono(
        monoEnabled);

    //==========================================================
    // Phase
    //==========================================================

    auto* phaseValue =
        apvts.getRawParameterValue("PHASE");

    const bool phaseEnabled =
        phaseValue->load() > 0.5f;

    gainProcessor.setPhase(
        phaseEnabled);

    //==========================================================
    // Mute
    //==========================================================

    auto* muteValue =
        apvts.getRawParameterValue("MUTE");

    const bool muteEnabled =
        muteValue->load() > 0.5f;

    gainProcessor.setMute(
        muteEnabled);

    //==========================================================
    // Bass Mono
    //==========================================================

    gainProcessor.setBassMono(
        apvts.getRawParameterValue(
            "BASS_MONO")->load() > 0.5f);

    gainProcessor.setBassMonoCrossover(
        apvts.getRawParameterValue(
            "BASS_MONO_CROSSOVER")->load());
}

void AudioPluginAudioProcessor::setBypassParameter(
    bool enabled)
{

    if (auto* parameter =
        apvts.getParameter("BYPASS"))
    {
        parameter->setValueNotifyingHost(
            enabled ? 1.0f : 0.0f);
    }

}

bool AudioPluginAudioProcessor::getBypassParameter()
{
    if (auto* value =
        apvts.getRawParameterValue("BYPASS"))
    {
        return value->load() > 0.5f;
    }

    return false;
}

void AudioPluginAudioProcessor::setMonoParameter(
    bool enabled)
{
    if (auto* parameter =
        apvts.getParameter("MONO"))
    {
        parameter->setValueNotifyingHost(
            enabled ? 1.0f : 0.0f);
    }
}

bool AudioPluginAudioProcessor::getMonoParameter()
{
    if (auto* value =
        apvts.getRawParameterValue("MONO"))
    {
        return value->load() > 0.5f;
    }

    return false;
}


void AudioPluginAudioProcessor::setPhaseParameter(bool enabled)
{
    if (auto* parameter = apvts.getParameter("PHASE"))
        parameter->setValueNotifyingHost(enabled ? 1.0f : 0.0f);
}

bool AudioPluginAudioProcessor::getPhaseParameter()
{
    if (auto* value = apvts.getRawParameterValue("PHASE"))
        return value->load() > 0.5f;

    return false;
}

void AudioPluginAudioProcessor::setMuteParameter(bool enabled)
{
    if (auto* parameter = apvts.getParameter("MUTE"))
    {
        parameter->setValueNotifyingHost(
            enabled ? 1.0f : 0.0f);
    }
}

bool AudioPluginAudioProcessor::getMuteParameter()
{
    if (auto* value = apvts.getRawParameterValue("MUTE"))
        return value->load() > 0.5f;

    return false;
}

//==========================================================
// Bass Mono Parameters
//==========================================================

void AudioPluginAudioProcessor::setBassMonoParameter(
    bool enabled)
{
    if (auto* parameter =
            apvts.getParameter("BASS_MONO"))
    {
        parameter->setValueNotifyingHost(
            enabled ? 1.0f : 0.0f);
    }
}

void AudioPluginAudioProcessor::setBassMonoListen(bool shouldListen)
{
    gainProcessor.setBassMonoListen(shouldListen);
}

bool AudioPluginAudioProcessor::getBassMonoParameter()
{
    return apvts.getRawParameterValue(
        "BASS_MONO")->load() > 0.5f;
}

void AudioPluginAudioProcessor::setBassMonoCrossoverParameter(
    float frequency)
{
    if (auto* parameter =
            apvts.getParameter("BASS_MONO_CROSSOVER"))
    {
        parameter->setValueNotifyingHost(
            apvts.getParameterRange(
                "BASS_MONO_CROSSOVER")
                .convertTo0to1(frequency));
    }
}

float AudioPluginAudioProcessor::getBassMonoCrossoverParameter()
{
    return apvts.getRawParameterValue(
        "BASS_MONO_CROSSOVER")->load();
}
