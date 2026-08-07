#include "NeonLookAndFeel.h"
#include "../../Theme/NeonColours.h"
#include "../../Graphics/Knob/GainScale.h"
#include <iostream>


//============================================================
// Constructor
//============================================================

NeonLookAndFeel::NeonLookAndFeel()
{
}

//============================================================
// Main Renderer
//============================================================

void NeonLookAndFeel::drawRotarySlider(
    juce::Graphics& g,
    int x,
    int y,
    int width,
    int height,
    float sliderPos,
    float rotaryStartAngle,
    float rotaryEndAngle,
    juce::Slider&)
{
    juce::ignoreUnused(
        sliderPos,
        rotaryStartAngle,
        rotaryEndAngle);

    auto geometry =
      createKnobGeometry(
          x,
          y,
          width,
          height);

    // drawShadow( g,geometry);

    drawKnobBody(g, geometry);

    drawOuterRing(
    g,
    geometry);

    drawProgressArc(
    g,
    geometry,
    sliderPos,
    rotaryStartAngle,
    rotaryEndAngle);

    drawTickMarks(
    g,
    geometry,
    rotaryStartAngle,
    rotaryEndAngle);

    drawLabels(
    g,
    geometry,
    rotaryStartAngle,
    rotaryEndAngle);

    drawIndicator(
    g,
    geometry,
    sliderPos,
    rotaryStartAngle,
    rotaryEndAngle);
}




//============================================================
// Layer 01 - Shadow
//============================================================

// void NeonLookAndFeel::drawShadow(
//     juce::Graphics& g,
//     const KnobGeometry& geo)
// {
//     auto shadowBounds = geo.outerBounds.expanded(6.0f);
//
//     for (int i = 0; i < 6; ++i)
//     {
//         auto current = shadowBounds.expanded((float)i);
//
//         g.setColour(
//             juce::Colours::black.withAlpha(
//                 0.12f - i * 0.018f));
//
//         g.drawEllipse(
//             current,
//             2.0f);
//     }
// }

//============================================================
// Layer 02 - Knob Body
//============================================================

void NeonLookAndFeel::drawKnobBody(
    juce::Graphics& g,
    const KnobGeometry& geo)
{
    // Base exterior

    g.setColour(NeonColours::background);
    g.fillEllipse(geo.outerBounds);

    // Borde

    g.setColour(juce::Colour(0xff2C3C52));
    g.drawEllipse(geo.outerBounds, 1.0f);

    //aro neon

    g.setColour(
    NeonColours::cyan.withAlpha(0.95f));

    g.drawEllipse(
        geo.outerBounds.reduced(2.5f),
        1.0f);


    // Centro

    juce::ColourGradient centerGradient(
    juce::Colour(0xff303B4D),
    geo.centre,

    juce::Colour(0xff151A22),
    geo.innerBounds.getBottomRight(),

    true);     // ← radial

    g.setGradientFill(centerGradient);

    g.fillEllipse(geo.innerBounds);

    //aro del centro blanco
    auto centerCore =
    geo.innerBounds.reduced(8.50f);

    g.setColour(
    juce::Colour(0xff1B2028));

    g.fillEllipse(centerCore);

}

//============================================================
// Layer 03 - Outer Ring
//============================================================

void NeonLookAndFeel::drawOuterRing(
    juce::Graphics& g,
    const KnobGeometry& geo)
{
    constexpr float ringThickness = 3.0f;

    auto ringBounds =
        geo.outerBounds.reduced(2.0f);

    g.setColour(
        juce::Colour(0xff2F3742));

    g.drawEllipse(
        ringBounds,
        ringThickness);
}

//============================================================
// Layer 04 - Progress Arc
//============================================================

void NeonLookAndFeel::drawProgressArc(
    juce::Graphics& g,
    const KnobGeometry& geo,
    float sliderPos,
    float rotaryStartAngle,
    float rotaryEndAngle)
{
    const float angle =
        juce::jmap(
            sliderPos,
            0.0f,
            1.0f,
            rotaryStartAngle,
            rotaryEndAngle);

    juce::Path arc;

    arc.addCentredArc(
        geo.centre.x,
        geo.centre.y,
        geo.radius - 4.0f,
        geo.radius - 4.0f,
        0.0f,
        rotaryStartAngle,
        angle,
        true);

    //--------------------------------------------------
    // Layers
    //--------------------------------------------------

    drawArcHalo(g, arc, geo);

    drawArcGlow(g, arc, geo);

    drawArcCore(g, arc, geo);

    drawArcHighlight(g, arc);
}

//============================================================
// Arc Layer 01 - Halo
//============================================================
//
// Función:
// Dibuja la luz más externa del arco.
// Es una capa muy tenue que simula la dispersión de la luz.
//
// Parámetros para modificar:
//
// Alpha
//     Intensidad del halo.
//
// PathStrokeType
//     Qué tan grande es el halo.
//
// ColourGradient
//     Colores del halo.
//
void NeonLookAndFeel::drawArcHalo(
    juce::Graphics& g,
    const juce::Path& arc,
    const KnobGeometry& geo)
{
    juce::ColourGradient gradient(
        NeonColours::cyan.withAlpha(0.45f),
        geo.outerBounds.getTopLeft(),

        NeonColours::magenta.withAlpha(0.45f),
        geo.outerBounds.getBottomRight(),

        false);

    g.setGradientFill(gradient);

    g.strokePath(
        arc,
        juce::PathStrokeType(13.0f));
}

//============================================================
// Arc Layer 02 - Glow
//============================================================
//
// Función:
// Dibuja el brillo principal alrededor del arco.
// Es más visible que el Halo.
//
// Parámetros para modificar:
//
// Alpha
//     Intensidad del glow.
//
// PathStrokeType
//     Grosor del glow.
//
// ColourGradient
//     Colores del glow.
//

void NeonLookAndFeel::drawArcGlow(
    juce::Graphics& g,
    const juce::Path& arc,
    const KnobGeometry& geo)
{
    juce::ColourGradient gradient(
        NeonColours::cyan.withAlpha(0.50f),
        geo.outerBounds.getTopLeft(),

        NeonColours::magenta.withAlpha(0.50f),
        geo.outerBounds.getBottomRight(),

        false);

    g.setGradientFill(gradient);

    g.strokePath(
        arc,
        juce::PathStrokeType(13.0f));
}

//============================================================
// Arc Layer 03 - Core
//============================================================
//
// Función:
// Dibuja el arco principal.
// Es la línea que realmente representa el valor del knob.
//
// Parámetros para modificar:
//
// ColourGradient
//     Colores principales.
//
// PathStrokeType
//     Grosor del arco.
//

void NeonLookAndFeel::drawArcCore(
    juce::Graphics& g,
    const juce::Path& arc,
    const KnobGeometry& geo)
{
    juce::ColourGradient gradient(
        NeonColours::cyan,
        geo.outerBounds.getTopLeft(),

        NeonColours::magenta,
        geo.outerBounds.getBottomRight(),

        false);

    g.setGradientFill(gradient);

    g.strokePath(
        arc,
        juce::PathStrokeType(3.0f));
}

//============================================================
// Arc Layer 04 - Highlight
//============================================================
//
// Función:
// Dibuja un reflejo encima del arco.
// Hace que parezca un material brillante.
//
// Parámetros para modificar:
//
// Alpha
//     Intensidad del reflejo.
//
// Colour
//     Color del reflejo.
//
// PathStrokeType
//     Grosor del reflejo.
//
void NeonLookAndFeel::drawArcHighlight(
    juce::Graphics& g,
    const juce::Path& arc)
{
    g.setColour(
        juce::Colours::white.withAlpha(0.20f));

    g.strokePath(
        arc,
        juce::PathStrokeType(3.0f));
}


//============================================================
// Layer 06 - Indicator
//============================================================

void NeonLookAndFeel::drawIndicator(
    juce::Graphics& g,
    const KnobGeometry& geo,
    float sliderPos,
    float rotaryStartAngle,
    float rotaryEndAngle)
{
    // Calcular el ángulo actual del knob
    const float angle = juce::jmap(
        sliderPos,
        0.0f,
        1.0f,
        rotaryStartAngle,
        rotaryEndAngle);

    // Configuración del indicador
    const float indicatorLength    = geo.innerRadius * 0.27f; // Controla qué tan largo es.
    const float indicatorThickness = 2.2f; // Controla qué tan grueso es.
    const float indicatorOffset    = geo.innerRadius * 0.64f; //Controla qué tan separado está del centro.

    juce::Path indicator;

    indicator.addRoundedRectangle(
        -indicatorThickness * 0.5f,
        -indicatorLength - indicatorOffset,
        indicatorThickness,
        indicatorLength,
        indicatorThickness * 0.5f);

    // Rotar y mover al centro del knob
    indicator.applyTransform(
        juce::AffineTransform::rotation(angle)
            .translated(
                geo.centre.x,
                geo.centre.y));

    // Color del indicador
    g.setColour(NeonColours::violet);

    g.fillPath(indicator);

}

void NeonLookAndFeel::drawTickMarks(
    juce::Graphics& g,
    const KnobGeometry& geo,
    float startAngle,
    float endAngle)
{
    drawMajorTicks(
        g,
        geo,
        startAngle,
        endAngle);
}

void NeonLookAndFeel::drawMajorTicks(
    juce::Graphics& g,
    const KnobGeometry& geo,
    float /*startAngle*/,
    float /*endAngle*/)
{
    KnobScale scale = createGainScale();
    const auto& marks = scale.getMarks();

    //--------------------------------------------------
    // Configuración de la escala
    //--------------------------------------------------

    const float tickStart =
        juce::degreesToRadians(128.0f);

    const float tickEnd =
        juce::degreesToRadians(412.0f);

    //--------------------------------------------------
    // Distancia y longitud de las marcas
    //--------------------------------------------------

    constexpr float minorLength = 7.0f;
    constexpr float majorLength = 11.5f;

    constexpr float majorInset = 1.5f;

    const float minorInner = geo.tickRadius;
    const float minorOuter = geo.tickRadius + minorLength;

    const float majorInner = geo.tickRadius - majorInset;
    const float majorOuter = geo.tickRadius + majorLength;

    //--------------------------------------------------
    // Colores
    //--------------------------------------------------

    auto minorColour =
    juce::Colour(0xff8F9CAA).withAlpha(0.55f);
    auto majorColour = juce::Colour(0xffD8E3F0);

    //--------------------------------------------------
    // Dibujar marcas
    //--------------------------------------------------

    for (const auto& mark : marks)
    {
        const float angle =
            juce::jmap(
                mark.position,
                0.0f,
                1.0f,
                tickStart,
                tickEnd);

        const bool major = mark.isMajor;

        drawTick(
            g,
            geo,
            angle,

            major ? majorInner : minorInner,
            major ? majorOuter : minorOuter,

            major ? 1.6f : 0.8f,

            major ? majorColour : minorColour);
    }
}

void NeonLookAndFeel::drawTick(
    juce::Graphics& g,
    const KnobGeometry& geo,
    float angle,
    float innerRadius,
    float outerRadius,
    float thickness,
    juce::Colour colour)
{
    g.setColour(colour);

    const auto direction =
        juce::Point<float>(
            std::cos(angle),
            std::sin(angle));

    const auto p1 =
        geo.centre + direction * innerRadius;

    const auto p2 =
        geo.centre + direction * outerRadius;

    g.drawLine(
        { p1, p2 },
        thickness);
}

void NeonLookAndFeel::drawLabels(
    juce::Graphics& g,
    const KnobGeometry& geo,
    float /*startAngle*/,
    float /*endAngle*/)
{
    KnobScale scale = createGainScale();
    const auto& marks = scale.getMarks();

    //--------------------------------------------------
    // Escala
    //--------------------------------------------------

    const float tickStart = juce::degreesToRadians(128.0f);
    const float tickEnd   = juce::degreesToRadians(412.0f);

    //--------------------------------------------------
    // Apariencia
    //--------------------------------------------------

    constexpr float labelRadius = 12.0f;

    constexpr float labelWidth  = 46.0f;
    constexpr float labelHeight = 18.0f;

    g.setColour(juce::Colours::white.withAlpha(0.60f));
    g.setFont(15.0f); //Tamaño de los numeros del knob

    //--------------------------------------------------
    // Dibujar Labels
    //--------------------------------------------------

    for (const auto& mark : marks)
    {
        if (mark.label.isEmpty())
            continue;

        const float angle = juce::jmap(
            mark.position,
            0.0f,
            1.0f,
            tickStart,
            tickEnd);

        const auto direction = juce::Point<float>(
            std::cos(angle),
            std::sin(angle));

        //==================================================
        // Posición final del label
        //==================================================

        const float radius =
            geo.tickRadius +
            labelRadius +
            mark.labelRadiusOffset;

        auto centre =
            geo.centre +
            direction * radius;

        juce::Rectangle<float> area(
            labelWidth,
            labelHeight);

        area.setCentre(centre);

        g.drawFittedText(
            mark.label,
            area.toNearestInt(),
            juce::Justification::centred,
            1);
    }
}