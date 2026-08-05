#pragma once

class NeonAnimator
{
public:

    //======================================================
    // Actualiza la animación
    //======================================================

    void update();

    //======================================================
    // Getters
    //======================================================

    float getDecorationGlow() const;

    float getFrameGlow() const;

private:

    //======================================================
    // Fases de la animación
    //======================================================
    // Cada elemento animado tiene su propia fase para
    // permitir velocidades independientes.
    //======================================================

    float decorationPhase = 0.0f;

    float framePhase = 0.0f;

    //======================================================
    // Estado actual de la animación
    //======================================================
    // Valores calculados en update() y utilizados por la GUI.
    //======================================================

    float decorationGlow = 1.0f;

    float frameGlow = 1.0f;
};