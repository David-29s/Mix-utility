#pragma once

#include "juce_gui_basics/juce_gui_basics.h"
#include "../Theme/NeonColours.h"

enum class IconType
{
    bypass,
    mono,
    phase,
    mute,
    ab
};

class IconToggleButton : public juce::Component
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
    // Component
    //==========================================================

    void paint(
        juce::Graphics& g) override;

    void setColour(
    juce::Colour newColour);


    void setActive(
        bool shouldBeActive);

    void mouseUp(
    const juce::MouseEvent& event) override;

    void mouseEnter(
        const juce::MouseEvent& event) override;

    void mouseExit(
        const juce::MouseEvent& event) override;



private:

    //==========================================================
    // State
    //==========================================================

    IconType iconType = IconType::bypass;

    juce::String label = "BYPASS";

    juce::Colour colour =
      NeonColours::cyan;

    bool active = false;

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