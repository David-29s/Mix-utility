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
    // Pan
    //==========================================================

    // Establece el paneo.
    // -100 = izquierda
    //    0 = centro
    // +100 = derecha
    void setPan(float newPan);

    // Devuelve el paneo actual.
    float getPan() const;

    //==========================================================
    // Width
    //==========================================================

    // Establece el ancho estéreo.
    // 0.0 = mono
    // 1.0 = stereo normal
    // 2.0 = stereo ampliado
    void setWidth(float newWidth);

    // Devuelve el ancho estéreo actual.
    float getWidth() const;

    //==========================================================
    // States
    //==========================================================

    // Activa o desactiva el bypass.
    void setBypass(bool shouldBypass);

    // Devuelve el estado actual del bypass.
    bool isBypassed() const;

    // Activa o desactiva el modo Mono.
    void setMono(bool shouldBeMono);

    // Devuelve el estado actual del modo Mono.
    bool isMono() const;

    //----------------------------------------------------------

    // Activa o desactiva la inversión de fase.
    void setPhase(bool shouldBePhase);

    // Devuelve el estado actual de la fase.
    bool isPhase() const;

    //----------------------------------------------------------

    // Activa o desactiva el mute.
    void setMute(bool shouldBeMute);

    // Devuelve el estado actual del mute.
    bool isMuted() const;

    //==========================================================
    // Processing
    //==========================================================

    // Procesa el bloque completo de audio.
    void process(juce::AudioBuffer<float>& buffer);



private:

    //==========================================================
    // Processing Stages
    //==========================================================

    // Procesa el estado de bypass.
    bool processBypass() const;

    // Procesa el estado de mute.
    bool processMute(
        juce::AudioBuffer<float>& buffer) const;

    // Aplica el procesamiento Mono.
    void processMono(
        juce::AudioBuffer<float>& buffer);

    // Aplica la inversión de fase.
    void processPhase(
        juce::AudioBuffer<float>& buffer);

    // Aplica la ganancia.
    void processGain(
        juce::AudioBuffer<float>& buffer);

    // Aplica el paneo estéreo.
    void processPan(
        juce::AudioBuffer<float>& buffer);

    // Aplica el ancho estéreo.
    void processWidth(
        juce::AudioBuffer<float>& buffer);

    //==========================================================
    // Parameters
    //==========================================================

    // Ganancia lineal aplicada al audio.
    float gain = 1.0f;

    // Paneo.
    // -1.0 = izquierda
    //  0.0 = centro
    // +1.0 = derecha
    float pan = 0.0f;

    // 0.0 = Mono
    // 1.0 = Stereo normal
    // 2.0 = Stereo ampliado
    float width = 1.0f;

    //==========================================================
    // States
    //==========================================================

    // Indica si el procesador está en bypass.
    bool bypass = false;

    // Indica si el modo Mono está activo.
    bool mono = false;

    //----------------------------------------------------------

    // Indica si la inversión de fase está activa.
    bool phase = false;

    //----------------------------------------------------------

    // Indica si el mute está activo.
    bool mute = false;
};