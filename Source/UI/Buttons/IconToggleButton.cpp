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
    const auto card = getLocalBounds().toFloat().reduced(2.0f);
    const bool selected = active || iconType == IconType::ab;
    const auto accent = getCurrentColour();

    g.setColour(juce::Colour(0xff151a22).withAlpha(
        isHovered ? 0.98f : 0.92f));
    g.fillRoundedRectangle(card, 8.0f);

    g.setColour(selected
        ? accent.withAlpha(0.58f)
        : accent.withAlpha(isHovered ? 0.50f : 0.24f));
    g.drawRoundedRectangle(card, 8.0f, selected ? 1.4f : 1.0f);

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

    case IconType::gain:
        drawGain(g);
        break;

    case IconType::pan:
        drawPan(g);
        break;

    case IconType::width:
        drawWidth(g);
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

void IconToggleButton::drawGain(
    juce::Graphics& g)
{
    auto area = getLocalBounds().toFloat();
    auto centre = area.getCentre();

    const float size =
        IconMetrics::iconSize;

    const float radius =
        size * 0.42f;

    const float thickness =
        IconMetrics::thickness;

    //==========================================================
    // Círculo principal
    //==========================================================

    juce::Path circle;

    circle.addEllipse(
        centre.x - radius,
        centre.y - radius,
        radius * 2.0f,
        radius * 2.0f);

    //==========================================================
    // Indicador
    //==========================================================

    juce::Path indicator;

    indicator.startNewSubPath(
        centre.x,
        centre.y - radius * 0.88f);

    indicator.lineTo(
        centre.x,
        centre.y - radius * 0.15f);

    //==========================================================
    // Color
    //==========================================================

    g.setColour(
        getCurrentColour());

    //==========================================================
    // Círculo
    //==========================================================

    g.strokePath(
        circle,
        juce::PathStrokeType(
            thickness,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));

    //==========================================================
    // Indicador
    //==========================================================

    g.strokePath(
        indicator,
        juce::PathStrokeType(
            thickness,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));

    //==========================================================
    // Marcas
    //==========================================================

    const int numMarks = 15;

    const float outerRadius =
        radius * 1.42f;

    const float innerRadius =
        radius * 1.22f;

    for (int i = 0; i < numMarks; ++i)
    {
        const float normalized =
            static_cast<float>(i)
            / static_cast<float>(numMarks - 1);

        const float angle =
            juce::MathConstants<float>::pi * 0.75f
            + normalized *
            juce::MathConstants<float>::pi * 1.5f;

        const float x1 =
            centre.x + std::cos(angle) * innerRadius;

        const float y1 =
            centre.y + std::sin(angle) * innerRadius;

        const float x2 =
            centre.x + std::cos(angle) * outerRadius;

        const float y2 =
            centre.y + std::sin(angle) * outerRadius;

        g.drawLine(
            x1, y1,
            x2, y2,
            thickness * 1.5f);
    }
}

void IconToggleButton::drawPan(
    juce::Graphics& g)
{
    auto area = getLocalBounds().toFloat();
    auto centre = area.getCentre();

    const float size =
        IconMetrics::iconSize;

    const float radius =
        size * 0.48f;

    const float thickness =
        IconMetrics::thickness;

    g.setColour(
        getCurrentColour());

    //==========================================================
    // Círculo exterior
    //==========================================================

    juce::Path circle;

    circle.addEllipse(
        centre.x - radius,
        centre.y - radius,
        radius * 2.0f,
        radius * 2.0f);

    g.strokePath(
        circle,
        juce::PathStrokeType(
            thickness,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));

    //==========================================================
    // Línea vertical
    //==========================================================

    g.drawLine(
        centre.x,
        centre.y - radius * 0.82f,
        centre.x,
        centre.y + radius * 0.82f,
        thickness);

    //==========================================================
    // L / R
    //==========================================================

    g.setFont(
        juce::Font(
            size * 0.34f,
            juce::Font::bold));

    g.drawText(
        "L",
        static_cast<int>(centre.x - radius * 0.78f),
        static_cast<int>(centre.y - size * 0.10f),
        static_cast<int>(size * 0.30f),
        static_cast<int>(size * 0.30f),
        juce::Justification::centred);

    g.drawText(
        "R",
        static_cast<int>(centre.x + radius * 0.48f),
        static_cast<int>(centre.y - size * 0.10f),
        static_cast<int>(size * 0.30f),
        static_cast<int>(size * 0.30f),
        juce::Justification::centred);

    //==========================================================
    // Línea horizontal
    //==========================================================

    const float lineY =
        centre.y + radius * 0.38f;

    const float lineLeft =
        centre.x - radius * 0.72f;

    const float lineRight =
        centre.x + radius * 0.72f;

    g.drawLine(
        lineLeft,
        lineY,
        lineRight,
        lineY,
        thickness);

    //==========================================================
    // Puntos
    //==========================================================

    const int numDots = 9;

    const float spacing =
        (lineRight - lineLeft)
        / static_cast<float>(numDots - 1);

    const float dotRadius =
        thickness * 0.75f;

    for (int i = 0; i < numDots; ++i)
    {
        const float x =
            lineLeft + spacing * i;

        g.fillEllipse(
            x - dotRadius,
            lineY - dotRadius,
            dotRadius * 2.0f,
            dotRadius * 2.0f);
    }

    //==========================================================
    // Indicador central
    //==========================================================

    g.drawLine(
        centre.x,
        lineY - size * 0.16f,
        centre.x,
        lineY + size * 0.16f,
        thickness * 1.5f);

    g.fillEllipse(
        centre.x - size * 0.06f,
        centre.y - size * 0.06f,
        size * 0.12f,
        size * 0.12f);
}

void IconToggleButton::drawWidth(
    juce::Graphics& g)
{
    auto area = getLocalBounds().toFloat();
    auto centre = area.getCentre();

    const float size =
        IconMetrics::iconSize;

    const float thickness =
        IconMetrics::thickness;

    g.setColour(
        getCurrentColour());

    //==========================================================
    // Línea central
    //==========================================================

    g.drawLine(
        centre.x,
        centre.y - size * 0.42f,
        centre.x,
        centre.y + size * 0.42f,
        thickness);

    //==========================================================
    // Altavoz izquierdo
    //==========================================================

    juce::Path leftSpeaker;

    const float speakerX =
        centre.x - size * 0.42f;

    const float speakerHalfHeight =
        size * 0.16f;

    leftSpeaker.startNewSubPath(
        speakerX,
        centre.y - speakerHalfHeight);

    leftSpeaker.lineTo(
        speakerX + size * 0.12f,
        centre.y - speakerHalfHeight);

    leftSpeaker.lineTo(
        speakerX + size * 0.23f,
        centre.y - size * 0.27f);

    leftSpeaker.lineTo(
        speakerX + size * 0.23f,
        centre.y + size * 0.27f);

    leftSpeaker.lineTo(
        speakerX + size * 0.12f,
        centre.y + speakerHalfHeight);

    leftSpeaker.lineTo(
        speakerX,
        centre.y + speakerHalfHeight);

    leftSpeaker.closeSubPath();

    g.strokePath(
        leftSpeaker,
        juce::PathStrokeType(
            thickness,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));

    //==========================================================
    // Altavoz derecho
    //==========================================================

    juce::Path rightSpeaker;

    const float rightSpeakerX =
        centre.x + size * 0.19f;

    rightSpeaker.startNewSubPath(
        rightSpeakerX,
        centre.y - speakerHalfHeight);

    rightSpeaker.lineTo(
        rightSpeakerX + size * 0.12f,
        centre.y - speakerHalfHeight);

    rightSpeaker.lineTo(
        rightSpeakerX + size * 0.23f,
        centre.y - size * 0.27f);

    rightSpeaker.lineTo(
        rightSpeakerX + size * 0.23f,
        centre.y + size * 0.27f);

    rightSpeaker.lineTo(
        rightSpeakerX + size * 0.12f,
        centre.y + speakerHalfHeight);

    rightSpeaker.lineTo(
        rightSpeakerX,
        centre.y + speakerHalfHeight);

    rightSpeaker.closeSubPath();

    g.strokePath(
        rightSpeaker,
        juce::PathStrokeType(
            thickness,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));

    //==========================================================
    // Ondas izquierda
    //==========================================================

    juce::Path leftWave1;

    leftWave1.addCentredArc(
        centre.x - size * 0.08f,
        centre.y,
        size * 0.22f,
        size * 0.30f,
        0.0f,
        -0.85f,
        0.85f,
        true);

    g.strokePath(
        leftWave1,
        juce::PathStrokeType(
            thickness,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));

    juce::Path leftWave2;

    leftWave2.addCentredArc(
        centre.x - size * 0.08f,
        centre.y,
        size * 0.34f,
        size * 0.44f,
        0.0f,
        -0.75f,
        0.75f,
        true);

    g.strokePath(
        leftWave2,
        juce::PathStrokeType(
            thickness,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));

    //==========================================================
    // Ondas derecha
    //==========================================================

    juce::Path rightWave1;

    rightWave1.addCentredArc(
        centre.x + size * 0.08f,
        centre.y,
        size * 0.22f,
        size * 0.30f,
        0.0f,
        2.29f,
        3.99f,
        true);

    g.strokePath(
        rightWave1,
        juce::PathStrokeType(
            thickness,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));

    juce::Path rightWave2;

    rightWave2.addCentredArc(
        centre.x + size * 0.08f,
        centre.y,
        size * 0.34f,
        size * 0.44f,
        0.0f,
        2.39f,
        3.89f,
        true);

    g.strokePath(
        rightWave2,
        juce::PathStrokeType(
            thickness,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));
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
