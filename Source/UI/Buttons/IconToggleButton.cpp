#include "IconToggleButton.h"
#include "../../Metrics/Icons/IconMetrics.h"
#include <iostream>

IconToggleButton::IconToggleButton()
{
    setMouseCursor(
        juce::MouseCursor::PointingHandCursor);

    setInterceptsMouseClicks(
        true,
        true);
}

void IconToggleButton::paint(
    juce::Graphics& g)
{
    //==========================================================
    // Fondo transparente
    //==========================================================

    g.fillAll(
        juce::Colours::transparentBlack);

    //==========================================================
    // Icono
    //==========================================================

    drawHoverGlow(g);

    drawIcon(g);

    drawLabel(g);




}


void IconToggleButton::drawBypass(
    juce::Graphics& g)
{
    drawPowerArc(g);

    drawPowerLine(g);

}

void IconToggleButton::drawPowerArc(
    juce::Graphics& g)
{
    //==========================================================
    // Área del botón
    //==========================================================

    auto area =
        getLocalBounds().toFloat();

    auto centre =
        area.getCentre();

    //==========================================================
    // Medidas
    //==========================================================

    const float radius =
    IconMetrics::iconSize * 0.5f;

    const float thickness =
        IconMetrics::thickness;

    //==========================================================
    // Arco
    //==========================================================

    juce::Path arc;

    arc.addCentredArc(
        centre.x,
        centre.y,

        radius,
        radius,

        2.7f,

        juce::MathConstants<float>::pi * 1.38f,
        juce::MathConstants<float>::pi * 2.9f,

        true);

    //==========================================================
    // Color
    //==========================================================

    g.setColour(
     getCurrentColour());

    //==========================================================
    // Dibujar arco
    //==========================================================

    g.strokePath(
        arc,
        juce::PathStrokeType(
            thickness,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));
}

void IconToggleButton::drawPowerLine(
    juce::Graphics& g)
{
    //==========================================================
    // Área del botón
    //==========================================================

    auto area =
        getLocalBounds().toFloat();

    auto centre =
        area.getCentre();

    //==========================================================
    // Medidas
    //==========================================================


    const float thickness =
     IconMetrics::thickness;

    const float lineTop =
    IconMetrics::iconSize * 0.45f;

    const float lineBottom =
    IconMetrics::iconSize * 0.18f;

    //==========================================================
    // Línea
    //==========================================================

    juce::Path line;

    line.startNewSubPath(
        centre.x,
        centre.y - lineTop);

    line.lineTo(
        centre.x,
        centre.y - lineBottom);

    //==========================================================
    // Color
    //==========================================================

    g.setColour(
      getCurrentColour());

    //==========================================================
    // Dibujar línea
    //==========================================================

    g.strokePath(
        line,
        juce::PathStrokeType(
            thickness,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));
}

//Texto

void IconToggleButton::drawIcon(
    juce::Graphics& g)
{
    switch (iconType)
    {
    case IconType::bypass:
        drawBypass(g);
        break;

    case IconType::mono:
        drawMono(g);
        break;

    case IconType::phase:
        drawPhase(g);
        break;

    case IconType::mute:
        drawMute(g);
        break;

    case IconType::ab:
        drawAB(g);
        break;
    }
}

void IconToggleButton::drawMono(
    juce::Graphics& g)
{
    //==========================================================
    // Área del botón
    //==========================================================

    auto area =
        getLocalBounds().toFloat();

    auto centre =
        area.getCentre();

    //==========================================================
    // Medidas
    //==========================================================

    const float radius =
        IconMetrics::iconSize * 0.5f;

    const float spacing =
        IconMetrics::iconSize * 0.25f;

    const float thickness =
        IconMetrics::thickness;

    //==========================================================
    // Círculo izquierdo
    //==========================================================

    juce::Path leftCircle;

    leftCircle.addEllipse(
        centre.x - radius - spacing,
        centre.y - radius,

        radius * 2.0f,
        radius * 2.0f);

    //==========================================================
    // Círculo derecho
    //==========================================================

    juce::Path rightCircle;

    rightCircle.addEllipse(
        centre.x - radius + spacing,
        centre.y - radius,

        radius * 2.0f,
        radius * 2.0f);

    //==========================================================
    // Color
    //==========================================================

    g.setColour(
      getCurrentColour());

    //==========================================================
    // Dibujar
    //==========================================================

    g.strokePath(
        leftCircle,
        juce::PathStrokeType(
            thickness,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));

    g.strokePath(
        rightCircle,
        juce::PathStrokeType(
            thickness,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));
}

void IconToggleButton::drawPhase(
    juce::Graphics& g)
{
    //==========================================================
    // Área del botón
    //==========================================================

    auto area =
        getLocalBounds().toFloat();

    auto centre =
        area.getCentre();

    //==========================================================
    // Medidas
    //==========================================================

    const float radius =
     IconMetrics::iconSize * 0.5f;

    const float thickness =
        IconMetrics::thickness;

    const float offset =
        IconMetrics::iconSize * 0.05f;

    //==========================================================
    // Círculo
    //==========================================================

    juce::Path circle;

    circle.addEllipse(
        centre.x - radius,
        centre.y - radius,
        radius * 2.0f,
        radius * 2.0f);

    //==========================================================
    // Línea diagonal
    //==========================================================

    juce::Path slash;

    slash.startNewSubPath(
        centre.x - radius + offset,
        centre.y + radius - offset);

    slash.lineTo(
        centre.x + radius - offset,
        centre.y - radius + offset);

    //==========================================================
    // Color
    //==========================================================

    g.setColour(
      getCurrentColour());

    //==========================================================
    // Dibujar
    //==========================================================

    g.strokePath(
        circle,
        juce::PathStrokeType(
            thickness,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));

    g.strokePath(
        slash,
        juce::PathStrokeType(
            thickness,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));
}

void IconToggleButton::drawMute(
    juce::Graphics& g)
{
    //==========================================================
    // Área del botón
    //==========================================================

    auto area =
        getLocalBounds().toFloat();

    auto centre =
        area.getCentre();

    //==========================================================
    // Medidas
    //==========================================================

    const float size =
     IconMetrics::iconSize;

    const float thickness =
        IconMetrics::thickness;

    const float width =
        size * 0.44f;

    const float height =
        size * 0.56f;

    const float bodyOffsetX =
        size * 0.56f;

    const float bodyHalfHeight =
        size * 0.28f;

    const float coneOffsetX =
        size * 0.11f;

    const float crossOffsetX =
        size * 0.44f;

    const float crossEndOffsetX =
        size * 0.89f;

    const float crossHalfHeight =
        size * 0.22f;

    //==========================================================
    // Altavoz
    //==========================================================

    juce::Path speaker;

    speaker.startNewSubPath(
        centre.x - bodyOffsetX,
        centre.y - bodyHalfHeight);

    speaker.lineTo(
        centre.x - bodyOffsetX + width,
        centre.y - bodyHalfHeight);

    speaker.lineTo(
        centre.x + coneOffsetX,
        centre.y - height);

    speaker.lineTo(
        centre.x + coneOffsetX,
        centre.y + height);

    speaker.lineTo(
        centre.x - bodyOffsetX + width,
        centre.y + bodyHalfHeight);

    speaker.lineTo(
        centre.x - bodyOffsetX,
        centre.y + bodyHalfHeight);

    speaker.closeSubPath();

    //==========================================================
    // X
    //==========================================================

    juce::Path cross;

    cross.startNewSubPath(
        centre.x + crossOffsetX,
        centre.y - crossHalfHeight);

    cross.lineTo(
        centre.x + crossEndOffsetX,
        centre.y + crossHalfHeight);

    cross.startNewSubPath(
        centre.x + crossEndOffsetX,
        centre.y - crossHalfHeight);

    cross.lineTo(
        centre.x + crossOffsetX,
        centre.y + crossHalfHeight);

    //==========================================================
    // Color
    //==========================================================

    g.setColour(
        getCurrentColour());

    //==========================================================
    // Dibujar altavoz
    //==========================================================

    g.strokePath(
        speaker,
        juce::PathStrokeType(
            thickness,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));

    //==========================================================
    // Dibujar X
    //==========================================================

    g.strokePath(
        cross,
        juce::PathStrokeType(
            thickness,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));
}

void IconToggleButton::drawAB(
    juce::Graphics& g)
{
    //==========================================================
    // Área del botón
    //==========================================================

    auto area =
        getLocalBounds().toFloat();

    auto centre =
        area.getCentre();

    //==========================================================
    // Medidas
    //==========================================================

    const float fontSize =
    IconMetrics::iconSize * 1.50f;

    const float letterWidth =
        IconMetrics::iconSize * 1.20f;

    const float letterHeight =
        IconMetrics::iconSize * 1.20f;

    const float letterOffsetY =
        IconMetrics::iconSize * 0.60f;

    //==========================================================
    // Color
    //==========================================================

    g.setColour(
        getCurrentColour());

    //==========================================================
    // Fuente
    //==========================================================

    g.setFont(
        juce::Font(
            fontSize,
            juce::Font::bold));

    //==========================================================
    // Letra
    //==========================================================

    g.drawText(
        stateA ? "A" : "B",

        static_cast<int>(
            centre.x - letterWidth * 0.5f),

        static_cast<int>(
            centre.y - letterOffsetY),

        static_cast<int>(
            letterWidth),

        static_cast<int>(
            letterHeight),

        juce::Justification::centred);
}

void IconToggleButton::setIconType(
    IconType newType)
{
    iconType = newType;

    repaint();
}

void IconToggleButton::setLabel(
    const juce::String& newLabel)
{
    label = newLabel;

    repaint();
}

void IconToggleButton::drawLabel(
    juce::Graphics& g)
{
    auto area =
        getLocalBounds();

    const float fontSize =
    IconMetrics::labelFont;

    const float offset =
        IconMetrics::labelOffset;

    const float textHeight =
        16.0f;

    g.setColour(
     getCurrentColour());

    g.setFont(
        fontSize);

    g.drawText(
        label,
        0,
        static_cast<int>(
            area.getBottom() - offset),
        area.getWidth(),
        static_cast<int>(
            textHeight),
        juce::Justification::centred);
}

void IconToggleButton::setColour(
    juce::Colour newColour)
{
    colour = newColour;

    repaint();
}

void IconToggleButton::setActive(
    bool shouldBeActive)
{
    active = shouldBeActive;

    repaint();
}

bool IconToggleButton::isActive() const
{
    return active;
}

//==========================================================
// A/B
//==========================================================

// Alterna entre el estado A y B.
void IconToggleButton::toggleAB()
{
    stateA = !stateA;

    repaint();
}

// Devuelve true cuando está seleccionado A.
bool IconToggleButton::isStateA() const
{
    return stateA;
}

juce::Colour IconToggleButton::getCurrentColour() const
{
    //==========================================================
    // A/B
    //==========================================================

    if (iconType == IconType::ab)
    {
        if (stateA)
            return NeonColours::cyan;

        return NeonColours::violet;
    }

    //==========================================================
    // Botones normales
    //==========================================================

    if (active)
        return colour;

    return NeonColours::grey;
}

void IconToggleButton::mouseUp(
    const juce::MouseEvent& event)
{
    juce::Component::mouseUp(event);

    //==========================================================
    // State
    //==========================================================

    if (iconType == IconType::ab)
    {
        toggleAB();
    }
    else
    {
        setActive(!active);
    }

    //==========================================================
    // Event
    //==========================================================

    if (onClick)
        onClick();
}

void IconToggleButton::mouseEnter(
    const juce::MouseEvent& event)
{
    juce::ignoreUnused(event);

    isHovered = true;

    repaint();
}

void IconToggleButton::mouseExit(
    const juce::MouseEvent& event)
{
    juce::ignoreUnused(event);

    isHovered = false;

    repaint();
}

void IconToggleButton::drawHoverGlow(
    juce::Graphics& g)
{
    if (!isHovered)
        return;

    auto area =
        getLocalBounds().toFloat();

    auto centre =
        area.getCentre();

    const float radius =
    IconMetrics::glowRadius;

    g.setColour(
        getCurrentColour().withAlpha(
            0.20f));

    g.fillEllipse(
        centre.x - radius,
        centre.y - radius,
        radius * 2.0f,
        radius * 2.0f);
}


void IconToggleButton::setStateA(
    bool shouldUseStateA)
{
    stateA = shouldUseStateA;

    repaint();
}