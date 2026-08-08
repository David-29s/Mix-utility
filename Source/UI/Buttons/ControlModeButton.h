#pragma once
// Evita que este archivo .h sea incluido más de una vez
// durante la compilación.


#include "juce_gui_basics/juce_gui_basics.h"
// Incluye las clases básicas de JUCE necesarias para crear
// componentes gráficos, eventos del mouse, Graphics, Colour, etc.


#include "../../Theme/NeonColours.h"
// Incluye nuestros colores personalizados del proyecto.
// Por ejemplo: NeonColours::cyan, neonGreen, violet, etc.



//==========================================================
// Modo actual del botón
//==========================================================

enum class ControlMode
{
    gain,
    pan,
    width
};

class ControlModeButton : public juce::Component
{
public:

    ControlModeButton();

    //==========================================================
    // Mode
    //==========================================================

    void setMode(ControlMode newMode);

    ControlMode getMode() const;

    void nextMode();

    //==========================================================
    // Component
    //==========================================================

    void paint(
        juce::Graphics& g) override;

    void mouseUp(
        const juce::MouseEvent& event) override;

    void mouseEnter(
        const juce::MouseEvent& event) override;

    void mouseExit(
        const juce::MouseEvent& event) override;

    //==========================================================
    // Event
    //==========================================================

    std::function<void(ControlMode)> onModeChanged;

private:

    //==========================================================
    // State
    //==========================================================

    ControlMode mode =
        ControlMode::gain;

    bool isHovered = false;

    //==========================================================
    // Draw
    //==========================================================

    void drawIcon(
        juce::Graphics& g);

    void drawGain(
        juce::Graphics& g);

    void drawPan(
        juce::Graphics& g);

    void drawWidth(
        juce::Graphics& g);

    void drawLabel(
        juce::Graphics& g);

    void drawHoverGlow(
        juce::Graphics& g);

    juce::Colour getCurrentColour() const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(
        ControlModeButton)
};