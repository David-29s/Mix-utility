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

//==========================================================
// Processing
//==========================================================

// Procesa el bloque completo de audio.
void GainProcessor::process(
    juce::AudioBuffer<float>& buffer)
{

    //==========================================================
    // States
    //==========================================================

    // Si el bypass está activo, no procesa el audio.
    if (bypass)
        return;


    //==========================================================
    // Audio
    //==========================================================

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