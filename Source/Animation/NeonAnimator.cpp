#include "NeonAnimator.h"

#include <cmath>

//==========================================================
// Animation Settings
//==========================================================

namespace
{
    //======================================================
    // Velocidad de la respiración
    //======================================================
    // Aumentar  -> Respiración más rápida.
    // Disminuir -> Respiración más lenta.
    constexpr float breathingSpeed = 0.12f;

    //======================================================
    // Brillo mínimo
    //======================================================
    // Controla el brillo más bajo que alcanzará el neón.
    // Valores pequeños hacen que casi se apague.
    constexpr float minimumBrightness = 0.25f;

    //======================================================
    // Intensidad de la respiración
    //======================================================
    // Controla cuánto aumenta el brillo desde el mínimo.
    // Valores grandes producen una respiración más evidente.
    constexpr float breathingAmplitude = 0.65f;

    //======================================================
    // Velocidad de la onda seno
    //======================================================
    // Controla qué tan rápido cambia el brillo dentro
    // de la respiración.
    //
    // Valores pequeños = transición más suave.
    // Valores grandes  = pulsación más rápida.
    constexpr float waveSpeed = 0.5f;
}

//==========================================================
// Actualiza la animación
//==========================================================

void NeonAnimator::update()
{
    phase += breathingSpeed;

    pulse =
        minimumBrightness +
        breathingAmplitude *
        std::sin(phase * waveSpeed);
}

//==========================================================
// Devuelve la intensidad actual del brillo
//==========================================================

float NeonAnimator::getPulse() const
{
    return pulse;
}