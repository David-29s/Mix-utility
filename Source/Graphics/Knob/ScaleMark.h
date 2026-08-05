#pragma once

#include <juce_core/juce_core.h>

struct ScaleMark
{
    // Posición de la marca dentro del arco.
    // Siempre entre 0.0 y 1.0
    float position = 0.0f;

    // Marca grande o pequeña
    bool isMajor = false;

    // Texto que se dibuja.
    // Las pequeñas normalmente estarán vacías.
    juce::String label;

    // Valor del parámetro.
    // Solo las principales lo utilizarán.
    float value = 0.0f;

    //==========================================================
    // Ajuste individual del label
    //==========================================================

    float labelRadiusOffset = 0.0f;

};