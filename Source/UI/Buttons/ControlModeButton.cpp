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
        IconMetrics::controlModeIconSize * 1.15f;

    const float thickness =
        IconMetrics::thickness;

    g.setColour(
        getCurrentColour());

    // Perilla de nivel con indicador y marcas alrededor.
    const float radius = size * 0.35f;
    juce::Path knob;
    knob.addEllipse(
        centre.x - radius,
        centre.y - radius,
        radius * 2.0f,
        radius * 2.0f);

    g.strokePath(
        knob,
        juce::PathStrokeType(
            thickness,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));

    juce::Path pointer;
    pointer.startNewSubPath(centre.x, centre.y - radius * 0.92f);
    pointer.lineTo(centre.x, centre.y - radius * 0.18f);
    g.strokePath(
        pointer,
        juce::PathStrokeType(
            thickness * 1.15f,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));

    constexpr int markCount = 7;
    const float innerRadius = size * 0.43f;
    const float outerRadius = size * 0.49f;
    for (int i = 0; i < markCount; ++i)
    {
        const float t = static_cast<float>(i)
            / static_cast<float>(markCount - 1);
        const float angle = juce::MathConstants<float>::pi * 0.72f
            + t * juce::MathConstants<float>::pi * 1.56f;
        g.drawLine(
            centre.x + std::cos(angle) * innerRadius,
            centre.y + std::sin(angle) * innerRadius,
            centre.x + std::cos(angle) * outerRadius,
            centre.y + std::sin(angle) * outerRadius,
            thickness * 0.9f);
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
        IconMetrics::controlModeIconSize;

    const float thickness =
        IconMetrics::thickness;

    g.setColour(
        getCurrentColour());

    // Perilla central con flechas en ambos sentidos para indicar panorama.
    const float arrowTipX = size * 0.45f;
    const float arrowBaseX = size * 0.27f;
    const float arrowHalfHeight = size * 0.17f;
    const float knobRadius = size * 0.18f;

    juce::Path arrows;
    arrows.startNewSubPath(centre.x - arrowBaseX, centre.y);
    arrows.lineTo(centre.x - arrowTipX, centre.y);
    arrows.lineTo(
        centre.x - arrowTipX + size * 0.13f,
        centre.y - arrowHalfHeight);
    arrows.startNewSubPath(centre.x - arrowTipX, centre.y);
    arrows.lineTo(
        centre.x - arrowTipX + size * 0.13f,
        centre.y + arrowHalfHeight);

    arrows.startNewSubPath(centre.x + arrowBaseX, centre.y);
    arrows.lineTo(centre.x + arrowTipX, centre.y);
    arrows.lineTo(
        centre.x + arrowTipX - size * 0.13f,
        centre.y - arrowHalfHeight);
    arrows.startNewSubPath(centre.x + arrowTipX, centre.y);
    arrows.lineTo(
        centre.x + arrowTipX - size * 0.13f,
        centre.y + arrowHalfHeight);

    g.strokePath(
        arrows,
        juce::PathStrokeType(
            thickness,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));

    g.drawEllipse(
        centre.x - knobRadius,
        centre.y - knobRadius,
        knobRadius * 2.0f,
        knobRadius * 2.0f,
        thickness);

    g.fillEllipse(
        centre.x - size * 0.045f,
        centre.y - size * 0.045f,
        size * 0.09f,
        size * 0.09f);
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
        IconMetrics::controlModeIconSize;

    const float thickness =
        IconMetrics::thickness;

    g.setColour(
        getCurrentColour());

    // Dos canales se abren desde el centro: una lectura más directa
    // de la anchura estéreo que el icono anterior de altavoces/ondas.
    const float halfHeight = size * 0.34f;
    const float innerX = size * 0.10f;
    const float outerX = size * 0.39f;
    const float arrowSize = size * 0.16f;

    g.drawLine(
        centre.x - innerX,
        centre.y - halfHeight,
        centre.x - innerX,
        centre.y + halfHeight,
        thickness);
    g.drawLine(
        centre.x + innerX,
        centre.y - halfHeight,
        centre.x + innerX,
        centre.y + halfHeight,
        thickness);

    juce::Path outwardArrows;
    outwardArrows.startNewSubPath(
        centre.x - innerX - size * 0.04f, centre.y);
    outwardArrows.lineTo(centre.x - outerX, centre.y);
    outwardArrows.lineTo(
        centre.x - outerX + arrowSize, centre.y - arrowSize * 0.72f);
    outwardArrows.startNewSubPath(centre.x - outerX, centre.y);
    outwardArrows.lineTo(
        centre.x - outerX + arrowSize, centre.y + arrowSize * 0.72f);

    outwardArrows.startNewSubPath(
        centre.x + innerX + size * 0.04f, centre.y);
    outwardArrows.lineTo(centre.x + outerX, centre.y);
    outwardArrows.lineTo(
        centre.x + outerX - arrowSize, centre.y - arrowSize * 0.72f);
    outwardArrows.startNewSubPath(centre.x + outerX, centre.y);
    outwardArrows.lineTo(
        centre.x + outerX - arrowSize, centre.y + arrowSize * 0.72f);

    g.strokePath(
        outwardArrows,
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
            return NeonColours::gainNeon;

        case ControlMode::pan:
            return NeonColours::panNeon;

        case ControlMode::width:
            return NeonColours::widthNeon;
    }

    return NeonColours::grey;
}
