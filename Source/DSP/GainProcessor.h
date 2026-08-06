#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

class GainProcessor
{
public:

    //==========================================================
    // Constructor / Destructor
    //==========================================================

    GainProcessor();

    ~GainProcessor();

    //==========================================================
    // Parameters
    //==========================================================

    // Establece la ganancia lineal del procesador.
    void setGain(float newGain);

    // Devuelve la ganancia actual.
    float getGain() const;

    //==========================================================
    // States
    //==========================================================

    // Activa o desactiva el bypass.
    void setBypass(bool shouldBypass);

    // Devuelve el estado actual del bypass.
    bool isBypassed() const;

    //==========================================================
    // Processing
    //==========================================================

    // Procesa el bloque completo de audio.
    void process(juce::AudioBuffer<float>& buffer);

private:

    //==========================================================
    // Parameters
    //==========================================================

    // Ganancia lineal aplicada al audio.
    float gain = 1.0f;

    //==========================================================
    // States
    //==========================================================

    // Indica si el procesador está en bypass.
    bool bypass = false;
};