#include "GainProcessor.h"

//==========================================================
// Constructor / Destructor
//==========================================================

GainProcessor::GainProcessor()
{

}

GainProcessor::~GainProcessor()
{

}

//==========================================================
// Parameters
//==========================================================

// Establece la ganancia lineal que se aplicará al audio.
void GainProcessor::setGain(
    float newGain)
{
    gain = newGain;
}

// Devuelve la ganancia actual.
float GainProcessor::getGain() const
{
    return gain;
}

//==========================================================
// Pan
//==========================================================

// Establece el paneo.
// El valor recibido está en el rango -1.0 a +1.0.
void GainProcessor::setPan(
    float newPan)
{
    pan = juce::jlimit(
        -1.0f,
        1.0f,
        newPan);
}


// Devuelve el paneo actual.
float GainProcessor::getPan() const
{
    return pan;
}

//==========================================================
// Width
//==========================================================

void GainProcessor::setWidth(
    float newWidth)
{
    width = juce::jlimit(
        0.0f,
        2.0f,
        newWidth);
}


float GainProcessor::getWidth() const
{
    return width;
}

//==========================================================
// States
//==========================================================

// Activa o desactiva el bypass del procesador.
void GainProcessor::setBypass(
    bool shouldBypass)
{
    bypass = shouldBypass;
}

// Devuelve el estado actual del bypass.
bool GainProcessor::isBypassed() const
{
    return bypass;
}

//----------------------------------------------------------

// Activa o desactiva el modo Mono.
void GainProcessor::setMono(
    bool shouldBeMono)
{
    mono = shouldBeMono;
}

// Devuelve el estado actual del modo Mono.
bool GainProcessor::isMono() const
{
    return mono;
}

//----------------------------------------------------------

// Activa o desactiva la inversión de fase.
void GainProcessor::setPhase(
    bool shouldBePhase)
{
    phase = shouldBePhase;
}

// Devuelve el estado actual de la fase.
bool GainProcessor::isPhase() const
{
    return phase;
}

//----------------------------------------------------------

// Activa o desactiva el mute.
void GainProcessor::setMute(
    bool shouldBeMute)
{
    mute = shouldBeMute;
}

// Devuelve el estado actual del mute.
bool GainProcessor::isMuted() const
{
    return mute;
}
//==========================================================
// Processing
//==========================================================

// Procesa el bloque completo de audio.
//==========================================================
// Processing
//==========================================================

// Procesa el bloque completo de audio.
void GainProcessor::process(
    juce::AudioBuffer<float>& buffer)
{
    //==========================================================
    // Processing Stages
    //==========================================================

    if (processBypass())
        return;

    if (processMute(buffer))
        return;

    processMono(buffer);

    processPhase(buffer);

    processPan(buffer);

    processWidth(buffer);

    processGain(buffer);
}

//==========================================================
// Processing Stages
//==========================================================

// Procesa el estado de bypass.
bool GainProcessor::processBypass() const
{
    return bypass;
}

//----------------------------------------------------------

// Procesa el estado de mute.
bool GainProcessor::processMute(
    juce::AudioBuffer<float>& buffer) const
{
    if (!mute)
        return false;

    buffer.clear();

    return true;
}

//----------------------------------------------------------
// Aplica el procesamiento Mono.
void GainProcessor::processMono(
    juce::AudioBuffer<float>& buffer)
{
    //==========================================================
    // State
    //==========================================================

    // El botón Mono está desactivado.
    if (!mono)
        return;

    //==========================================================
    // Audio
    //==========================================================

    // El procesamiento Mono requiere al menos dos canales.
    if (buffer.getNumChannels() < 2)
        return;

    const int numSamples =
        buffer.getNumSamples();

    auto* left =
        buffer.getWritePointer(0);

    auto* right =
        buffer.getWritePointer(1);

    //==========================================================
    // Processing
    //==========================================================

    for (int sample = 0;
         sample < numSamples;
         ++sample)
    {
        const float monoSample =
            (left[sample] + right[sample]) * 0.5f;

        left[sample] =
            monoSample;

        right[sample] =
            monoSample;
    }
}

//----------------------------------------------------------
// Aplica la inversión de fase.
void GainProcessor::processPhase(
    juce::AudioBuffer<float>& buffer)
{
    //==========================================================
    // State
    //==========================================================

    // El botón Phase está desactivado.
    if (!phase)
        return;

    //==========================================================
    // Audio
    //==========================================================

    const int numChannels =
        buffer.getNumChannels();

    const int numSamples =
        buffer.getNumSamples();

    //==========================================================
    // Processing
    //==========================================================

    for (int channel = 0;
         channel < numChannels;
         ++channel)
    {
        auto* channelData =
            buffer.getWritePointer(channel);

        for (int sample = 0;
             sample < numSamples;
             ++sample)
        {
            channelData[sample] *= -1.0f;
        }
    }
}

//----------------------------------------------------------

// Aplica la ganancia al bloque de audio.
void GainProcessor::processGain(
    juce::AudioBuffer<float>& buffer)
{
    const int numChannels =
        buffer.getNumChannels();

    const int numSamples =
        buffer.getNumSamples();

    for (int channel = 0;
         channel < numChannels;
         ++channel)
    {
        auto* channelData =
            buffer.getWritePointer(channel);

        for (int sample = 0;
             sample < numSamples;
             ++sample)
        {
            channelData[sample] *= gain;
        }
    }
}

//==========================================================
// Pan
//==========================================================

// Aplica el paneo al bloque estéreo.
void GainProcessor::processPan(
    juce::AudioBuffer<float>& buffer)
{
    // Necesitamos como mínimo L + R.
    if (buffer.getNumChannels() < 2)
        return;

    const int numSamples =
        buffer.getNumSamples();

    auto* left =
        buffer.getWritePointer(0);

    auto* right =
        buffer.getWritePointer(1);

    //==========================================================
    // Pan
    //==========================================================

    // Convierte:
    //
    // -1.0 = izquierda
    //  0.0 = centro
    // +1.0 = derecha
    //
    // a una posición 0.0 → 1.0.

    const float panPosition =
        (pan + 1.0f) * 0.5f;

    //==========================================================
    // Equal Power Panning
    //==========================================================

    const float angle =
        panPosition *
        juce::MathConstants<float>::halfPi;

    const float leftGain =
        std::cos(angle);

    const float rightGain =
        std::sin(angle);

    //==========================================================
    // Processing
    //==========================================================

    for (int sample = 0;
         sample < numSamples;
         ++sample)
    {
        left[sample] *= leftGain;
        right[sample] *= rightGain;
    }
}

//==========================================================
// Width
//==========================================================

void GainProcessor::processWidth(
    juce::AudioBuffer<float>& buffer)
{
    // WIDTH necesita como mínimo dos canales.
    if (buffer.getNumChannels() < 2)
        return;

    const int numSamples =
        buffer.getNumSamples();

    auto* left =
        buffer.getWritePointer(0);

    auto* right =
        buffer.getWritePointer(1);

    //==========================================================
    // Mid / Side
    //==========================================================

    for (int sample = 0;
         sample < numSamples;
         ++sample)
    {
        const float inputLeft =
            left[sample];

        const float inputRight =
            right[sample];

        // Centro de la señal.
        const float mid =
            (inputLeft + inputRight) * 0.5f;

        // Información estéreo.
        const float side =
            (inputLeft - inputRight) * 0.5f;

        //======================================================
        // Aplicar WIDTH
        //======================================================

        const float newSide =
            side * width;

        //======================================================
        // Mid / Side → Left / Right
        //======================================================

        left[sample] =
            mid + newSide;

        right[sample] =
            mid - newSide;
    }
}