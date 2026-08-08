#include "ControlModeButton.h"
#include "../../Metrics/Icons/IconMetrics.h"

//==========================================================
// Constructor
//==========================================================

ControlModeButton::ControlModeButton()
{
    setMouseCursor(
        juce::MouseCursor::PointingHandCursor);

    setInterceptsMouseClicks(
        true,
        true);
}


//==========================================================
// Paint
//==========================================================

void ControlModeButton::paint(
    juce::Graphics& g)
{
    //==========================================================
    // Fondo transparente
    //==========================================================

    g.fillAll(
        juce::Colours::transparentBlack);

    //==========================================================
    // Glow
    //==========================================================

    drawHoverGlow(g);

    //==========================================================
    // Icono
    //==========================================================

    drawIcon(g);

    //==========================================================
    // Label
    //==========================================================

    drawLabel(g);
}


//==========================================================
// Draw Icon
//==========================================================

void ControlModeButton::drawIcon(
    juce::Graphics& g)
{
    switch (mode)
    {
        case ControlMode::gain:
            drawGain(g);
            break;

        case ControlMode::pan:
            drawPan(g);
            break;

        case ControlMode::width:
            drawWidth(g);
            break;
    }
}


//==========================================================
// GAIN
//==========================================================

void ControlModeButton::drawGain(
    juce::Graphics& g)
{
    auto area =
        getLocalBounds().toFloat();

    auto centre =
        area.getCentre();

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
    // Marcas exteriores
    //==========================================================

    const int numberOfMarks = 15;

    const float outerRadius =
        radius * 1.42f;

    const float innerRadius =
        radius * 1.22f;

    for (int i = 0; i < numberOfMarks; ++i)
    {
        const float normalized =
            static_cast<float>(i)
            / static_cast<float>(numberOfMarks - 1);

        const float angle =
            juce::MathConstants<float>::pi * 0.75f
            +
            normalized *
            juce::MathConstants<float>::pi * 1.5f;

        const float x1 =
            centre.x +
            std::cos(angle) * innerRadius;

        const float y1 =
            centre.y +
            std::sin(angle) * innerRadius;

        const float x2 =
            centre.x +
            std::cos(angle) * outerRadius;

        const float y2 =
            centre.y +
            std::sin(angle) * outerRadius;

        g.drawLine(
            x1,
            y1,
            x2,
            y2,
            thickness * 1.5f);
    }
}


//==========================================================
// PAN
//==========================================================

void ControlModeButton::drawPan(
    juce::Graphics& g)
{
    auto area =
        getLocalBounds().toFloat();

    auto centre =
        area.getCentre();

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
        static_cast<int>(
            centre.x - radius * 0.78f),
        static_cast<int>(
            centre.y - size * 0.10f),
        static_cast<int>(
            size * 0.30f),
        static_cast<int>(
            size * 0.30f),
        juce::Justification::centred);

    g.drawText(
        "R",
        static_cast<int>(
            centre.x + radius * 0.48f),
        static_cast<int>(
            centre.y - size * 0.10f),
        static_cast<int>(
            size * 0.30f),
        static_cast<int>(
            size * 0.30f),
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

    const int numberOfDots = 9;

    const float spacing =
        (lineRight - lineLeft)
        / static_cast<float>(numberOfDots - 1);

    const float dotRadius =
        thickness * 0.75f;

    for (int i = 0; i < numberOfDots; ++i)
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

    // Punto central
    g.fillEllipse(
        centre.x - size * 0.06f,
        centre.y - size * 0.06f,
        size * 0.12f,
        size * 0.12f);
}


//==========================================================
// WIDTH
//==========================================================

void ControlModeButton::drawWidth(
    juce::Graphics& g)
{
    auto area =
        getLocalBounds().toFloat();

    auto centre =
        area.getCentre();

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

    const float leftX =
        centre.x - size * 0.43f;

    const float speakerHeight =
        size * 0.27f;

    const float bodyHeight =
        size * 0.16f;

    leftSpeaker.startNewSubPath(
        leftX,
        centre.y - bodyHeight);

    leftSpeaker.lineTo(
        leftX + size * 0.12f,
        centre.y - bodyHeight);

    leftSpeaker.lineTo(
        leftX + size * 0.23f,
        centre.y - speakerHeight);

    leftSpeaker.lineTo(
        leftX + size * 0.23f,
        centre.y + speakerHeight);

    leftSpeaker.lineTo(
        leftX + size * 0.12f,
        centre.y + bodyHeight);

    leftSpeaker.lineTo(
        leftX,
        centre.y + bodyHeight);

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

    const float rightX =
        centre.x + size * 0.20f;

    rightSpeaker.startNewSubPath(
        rightX,
        centre.y - bodyHeight);

    rightSpeaker.lineTo(
        rightX + size * 0.12f,
        centre.y - bodyHeight);

    rightSpeaker.lineTo(
        rightX + size * 0.23f,
        centre.y - speakerHeight);

    rightSpeaker.lineTo(
        rightX + size * 0.23f,
        centre.y + speakerHeight);

    rightSpeaker.lineTo(
        rightX + size * 0.12f,
        centre.y + bodyHeight);

    rightSpeaker.lineTo(
        rightX,
        centre.y + bodyHeight);

    rightSpeaker.closeSubPath();

    g.strokePath(
        rightSpeaker,
        juce::PathStrokeType(
            thickness,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));

    //==========================================================
    // Onda izquierda 1
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

    //==========================================================
    // Onda izquierda 2
    //==========================================================

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
    // Onda derecha 1
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

    //==========================================================
    // Onda derecha 2
    //==========================================================

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


//==========================================================
// Label
//==========================================================

void ControlModeButton::drawLabel(
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

    juce::String text;

    switch (mode)
    {
        case ControlMode::gain:
            text = "GAIN";
            break;

        case ControlMode::pan:
            text = "PAN";
            break;

        case ControlMode::width:
            text = "WIDTH";
            break;
    }

    g.setColour(
        getCurrentColour());

    g.setFont(
        fontSize);

    g.drawText(
        text,
        0,
        static_cast<int>(
            area.getBottom() - offset),
        area.getWidth(),
        static_cast<int>(
            textHeight),
        juce::Justification::centred);
}


//==========================================================
// Hover Glow
//==========================================================

void ControlModeButton::drawHoverGlow(
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


//==========================================================
// Mode
//==========================================================

void ControlModeButton::setMode(
    ControlMode newMode)
{
    mode = newMode;

    repaint();
}


ControlMode ControlModeButton::getMode() const
{
    return mode;
}


//==========================================================
// Next Mode
//==========================================================

void ControlModeButton::nextMode()
{
    switch (mode)
    {
        case ControlMode::gain:
            mode = ControlMode::pan;
            break;

        case ControlMode::pan:
            mode = ControlMode::width;
            break;

        case ControlMode::width:
            mode = ControlMode::gain;
            break;
    }

    repaint();

    if (onModeChanged)
        onModeChanged(mode);
}


//==========================================================
// Mouse
//==========================================================

void ControlModeButton::mouseUp(
    const juce::MouseEvent& event)
{
    juce::Component::mouseUp(event);

    nextMode();
}


void ControlModeButton::mouseEnter(
    const juce::MouseEvent& event)
{
    juce::ignoreUnused(event);

    isHovered = true;

    repaint();
}


void ControlModeButton::mouseExit(
    const juce::MouseEvent& event)
{
    juce::ignoreUnused(event);

    isHovered = false;

    repaint();
}


//==========================================================
// Colour
//==========================================================

juce::Colour ControlModeButton::getCurrentColour() const
{
    switch (mode)
    {
        case ControlMode::gain:
            return NeonColours::neonGreen;

        case ControlMode::pan:
            return NeonColours::cyan;

        case ControlMode::width:
            return NeonColours::violet;
    }

    return NeonColours::grey;
}