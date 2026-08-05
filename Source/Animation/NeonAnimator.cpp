#include "NeonAnimator.h"

#include <cmath>

//==========================================================
// Decoration Animation
//==========================================================

namespace
{
    //======================================================
    // Velocidad de la respiración
    //======================================================
    // Aumentar  -> Respiración más rápida.
    // Disminuir -> Respiración más lenta.
    constexpr float decorationSpeed = 0.06f;

    //======================================================
    // Brillo mínimo
    //======================================================
    // Controla el brillo más bajo que alcanzará el neón.
    // Valores pequeños hacen que casi se apague.
    constexpr float decorationMinimumBrightness = 0.25f;

    //======================================================
    // Intensidad de la respiración
    //======================================================
    // Controla cuánto aumenta el brillo desde el mínimo.
    // Valores grandes producen una respiración más evidente.
    constexpr float decorationAmplitude = 0.65f;

    //======================================================
    // Velocidad de la onda seno
    //======================================================
    // Controla qué tan rápido cambia el brillo dentro
    // de la respiración.
    //
    // Valores pequeños = transición más suave.
    // Valores grandes  = pulsación más rápida.
    constexpr float decorationWaveSpeed = 0.50f;
}

//==========================================================
// Frame Animation
//==========================================================

namespace
{
    //======================================================
    // Velocidad de la respiración
    //======================================================
    // Aumentar  -> Respiración más rápida.
    // Disminuir -> Respiración más lenta.
    constexpr float frameSpeed = 0.06f;

    //======================================================
    // Brillo mínimo
    //======================================================
    // Controla el brillo más bajo del marco.
    constexpr float frameMinimumBrightness = 0.30f;

    //======================================================
    // Intensidad de la respiración
    //======================================================
    // Controla cuánto aumenta el brillo del marco.
    constexpr float frameAmplitude = 0.50f;

    //======================================================
    // Velocidad de la onda seno
    //======================================================
    // Controla la suavidad de la respiración del marco.
    constexpr float frameWaveSpeed = 0.35f;
}

//==========================================================
// Actualiza la animación
//==========================================================

void NeonAnimator::update()
{
    //======================================================
    // Actualizar fases
    //======================================================

    decorationPhase += decorationSpeed;

    framePhase += frameSpeed;

    //======================================================
    // Decoración superior
    //======================================================

    decorationGlow =
        decorationMinimumBrightness +
        decorationAmplitude *
        std::sin(
            decorationPhase *
            decorationWaveSpeed);

    //======================================================
    // Marco
    //======================================================

    frameGlow =
        frameMinimumBrightness +
        frameAmplitude *
        std::sin(
            framePhase *
            frameWaveSpeed);
}

//==========================================================
// Decoration
//==========================================================

float NeonAnimator::getDecorationGlow() const
{
    return decorationGlow;
}

//==========================================================
// Frame
//==========================================================

float NeonAnimator::getFrameGlow() const
{
    return frameGlow;
}