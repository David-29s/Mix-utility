#pragma once

#include "juce_gui_basics/juce_gui_basics.h"
#include "../../Theme/NeonColours.h"


enum class IconType
{
    bypass,
    mono,
    phase,
    mute,
    ab,

    gain,
    pan,
    width
};

class IconToggleButton : public juce::Component, public juce::SettableTooltipClient
{
public:

    IconToggleButton();


    //==========================================================
    // State
    //==========================================================

    void setIconType(
        IconType newType);

    void setLabel(
        const juce::String& newLabel);

    //==========================================================
    // A/B
    //==========================================================

    // Alterna entre el estado A y B.
    void toggleAB();

    // Devuelve true cuando está seleccionado A.
    bool isStateA() const;

    void setStateA(bool shouldUseStateA);

    //==========================================================
    // Component
    //==========================================================

    void paint(
        juce::Graphics& g) override;

    void setColour(
    juce::Colour newColour);


    void setActive(
        bool shouldBeActive);

    // Devuelve el estado actual del botón.
    bool isActive() const;

    void mouseUp(
    const juce::MouseEvent& event) override;

    void mouseEnter(
        const juce::MouseEvent& event) override;

    void mouseExit(
        const juce::MouseEvent& event) override;

    //==========================================================
    // Events
    //==========================================================

    // Se ejecuta cuando el botón es presionado.
    std::function<void()> onClick;




private:

    //==========================================================
    // State
    //==========================================================

    IconType iconType = IconType::bypass;

    juce::String label = "BYPASS";

    juce::Colour colour =
      NeonColours::cyan;

    bool active = false;

    //==========================================================
    // A/B
    //==========================================================

    // Indica si está seleccionado el estado A.
    bool stateA = true;

    bool isHovered = false;



    juce::Colour getCurrentColour() const;



    //==========================================================
    // Draw
    //==========================================================

    void drawIcon(
        juce::Graphics& g);

    void drawBypass(
        juce::Graphics& g);

    void drawMono(
        juce::Graphics& g);

    void drawPhase(
        juce::Graphics& g);

    void drawMute(
        juce::Graphics& g);

    void drawAB(
        juce::Graphics& g);

    void drawGain(
    juce::Graphics& g);

    void drawPan(
        juce::Graphics& g);

    void drawWidth(
        juce::Graphics& g);

    void drawPowerArc(
        juce::Graphics& g);

    void drawPowerLine(
        juce::Graphics& g);

    void drawLabel(
        juce::Graphics& g);

    void drawHoverGlow(
    juce::Graphics& g);



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IconToggleButton)
};
