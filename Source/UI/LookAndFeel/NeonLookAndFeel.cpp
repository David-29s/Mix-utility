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
// Knob Mode
//============================================================

void NeonLookAndFeel::setKnobMode(
    KnobMode newMode)
{
    knobMode = newMode;
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
//     auto shadowBounds = geo.outerBounds.expanded(0.5f);
//
//     for (int i = 0; i < 6; ++i)
//     {
//         auto current = shadowBounds.expanded((float)i);
//
//         g.setColour(
//             juce::Colours::black.withAlpha(
//                 0.92f - i * 0.018f));
//
//         g.drawEllipse(
//             current,
//             0.2f);
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

    // g.setColour(juce::Colour(0xff2C3C52));
    // g.drawEllipse(geo.outerBounds, 1.5f);

    //aro neon

    // g.setColour(
    // NeonColours::cyan.withAlpha(0.95f));
    //
    // g.drawEllipse(
    //     geo.outerBounds.reduced(2.5f),
    //     2.0f);


    // Centro

    juce::ColourGradient centerGradient(
    juce::Colour(0xff303B4D).withAlpha(0.65f),
    geo.centre,

    juce::Colour(0xff151A22),
    geo.innerBounds.getBottomRight(),

    true);     // ← radial

    g.setGradientFill(centerGradient);

    g.fillEllipse(geo.innerBounds);

    //aro del centro blanco
    // auto centerCore =
    // geo.innerBounds.reduced(8.50f);
    //
    // g.setColour(
    // juce::Colour(0xff1B2028));

    // g.fillEllipse(centerCore);

}

//============================================================
// Layer 03 - Outer Ring
//============================================================

void NeonLookAndFeel::drawOuterRing(
    juce::Graphics& g,
    const KnobGeometry& geo)
{
    constexpr float ringThickness = 14.0f;

    auto ringBounds =
        geo.outerBounds.reduced(6.0f);

    juce::Path ringArc;

    ringArc.addCentredArc(
        ringBounds.getCentreX(),
        ringBounds.getCentreY(),
        ringBounds.getWidth() * 0.5f,
        ringBounds.getHeight() * 0.5f,
        1.9f,
        juce::MathConstants<float>::pi * 0.6f,
        juce::MathConstants<float>::twoPi * 1.10f,
        true);

    g.setColour(
        juce::Colour(0xff2F3742));

    g.strokePath(
        ringArc,
        juce::PathStrokeType(ringThickness));
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

    drawArcHighlight(g, arc);

    drawArcCore(g, arc, geo);


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

        NeonColours::magenta.withAlpha(0.75f),
        geo.outerBounds.getBottomRight(),

        false);

    g.setGradientFill(gradient);

    g.strokePath(
        arc,
        juce::PathStrokeType(16.0f));
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
        juce::PathStrokeType(18.0f));
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

    // Hace que el magenta aparezca antes
    gradient.addColour(
        0.78,
        NeonColours::magenta);

    g.setGradientFill(gradient);

    g.strokePath(
        arc,
        juce::PathStrokeType(18.0f));
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
        juce::PathStrokeType(20.0f));
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

    // Distancia adicional desde el centro
    constexpr float tickOffset = 6.6f;

    const float minorInner =
        geo.tickRadius + tickOffset;

    const float minorOuter =
        geo.tickRadius + tickOffset + minorLength;

    const float majorInner =
        geo.tickRadius + tickOffset - majorInset;

    const float majorOuter =
        geo.tickRadius + tickOffset + majorLength;

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
    //============================================================
    // Labels según el modo
    //============================================================

    juce::String leftLabel;
    juce::String rightLabel;

    switch (knobMode)
    {
    case KnobMode::gain:

        leftLabel = "-24";
        rightLabel = "+24";

        break;


    case KnobMode::pan:

        leftLabel = "L";
        rightLabel = "R";

        break;


    case KnobMode::width:

        leftLabel = "N";
        rightLabel = "W";

        break;
    }
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
    //============================================================
    // Configuración de etiquetas
    //============================================================

    //----------------------
    // GAIN
    //----------------------

    constexpr float gainRadius = 12.0f;

    constexpr float gainWidth  = 46.0f;
    constexpr float gainHeight = 18.0f;

    constexpr float gainFontSize = 15.0f;

    // Posición general
    constexpr float gainX = 0.0f;
    constexpr float gainY = 13.0f;

    // Posición individual
    constexpr float gainLeftX  = -6.0f;
    constexpr float gainLeftY  = 0.0f;

    constexpr float gainRightX = 6.0f;
    constexpr float gainRightY = 0.0f;


    //----------------------
    // PAN
    //----------------------

    constexpr float panRadius = 12.0f;

    constexpr float panWidth  = 46.0f;
    constexpr float panHeight = 18.0f;

    constexpr float panFontSize = 15.0f;

    // Posición general
    constexpr float panX = 0.0f;
    constexpr float panY = 13.0f;

    // Posición individual
    constexpr float panLeftX  = -4.0f;
    constexpr float panLeftY  = 0.0f;

    constexpr float panRightX = 4.0f;
    constexpr float panRightY = 0.0f;


    //----------------------
    // WIDTH
    //----------------------

    constexpr float widthRadius = 13.0f;

    constexpr float widthLabelWidth  = 62.0f;
    constexpr float widthLabelHeight = 18.0f;

    constexpr float widthFontSize = 16.0f;

    // Posición general
    constexpr float widthX = 0.0f;
    constexpr float widthY = 13.0f;

    // Posición individual
    constexpr float widthLeftX  = -4.0f;
    constexpr float widthLeftY  = 0.0f;

    constexpr float widthRightX = 4.0f;
    constexpr float widthRightY = 0.0f;


    g.setColour(
        juce::Colours::white.withAlpha(0.60f));


    //============================================================
    // Dibujar Labels
    //============================================================

    const float leftAngle =
        juce::degreesToRadians(128.0f);

    const float rightAngle =
        juce::degreesToRadians(412.0f);

    const auto drawLabel =
 [&](const juce::String& text,
     float angle,
     float individualX,
     float individualY)
 {
     float radius = 0.0f;
     float width = 0.0f;
     float height = 0.0f;
     float fontSize = 0.0f;
     float generalX = 0.0f;
     float generalY = 0.0f;

     //============================================================
     // Configuración según el modo
     //============================================================

     switch (knobMode)
     {
     case KnobMode::gain:

         radius = gainRadius;
         width = gainWidth;
         height = gainHeight;
         fontSize = gainFontSize;

         generalX = gainX;
         generalY = gainY;

         break;


     case KnobMode::pan:

         radius = panRadius;
         width = panWidth;
         height = panHeight;
         fontSize = panFontSize;

         generalX = panX;
         generalY = panY;

         break;


     case KnobMode::width:

         radius = widthRadius;
         width = widthLabelWidth;
         height = widthLabelHeight;
         fontSize = widthFontSize;

         generalX = widthX;
         generalY = widthY;

         break;
     }


     //============================================================
     // Dirección
     //============================================================

     const auto direction =
         juce::Point<float>(
             std::cos(angle),
             std::sin(angle));


     //============================================================
     // Posición radial
     //============================================================

     const float finalRadius =
         geo.tickRadius + radius;

     auto centre =
         geo.centre +
         direction * finalRadius;


     //============================================================
     // Posición general + posición individual
     //============================================================

     centre.x += generalX + individualX;
     centre.y += generalY + individualY;


     //============================================================
     // Área del texto
     //============================================================

     juce::Rectangle<float> area(
         width,
         height);

     area.setCentre(centre);


     //============================================================
     // Fuente
     //============================================================

     g.setFont(fontSize);


     //============================================================
     // Dibujar
     //============================================================

     g.drawFittedText(
         text,
         area.toNearestInt(),
         juce::Justification::centred,
         1);
 };


    //============================================================
    // Left Label & Right Label
    //============================================================

    switch (knobMode)
    {
    case KnobMode::gain:

        drawLabel(
            leftLabel,
            leftAngle,
            gainLeftX,
            gainLeftY);

        drawLabel(
            rightLabel,
            rightAngle,
            gainRightX,
            gainRightY);

        break;


    case KnobMode::pan:

        drawLabel(
            leftLabel,
            leftAngle,
            panLeftX,
            panLeftY);

        drawLabel(
            rightLabel,
            rightAngle,
            panRightX,
            panRightY);

        break;


    case KnobMode::width:

        drawLabel(
            leftLabel,
            leftAngle,
            widthLeftX,
            widthLeftY);

        drawLabel(
            rightLabel,
            rightAngle,
            widthRightX,
            widthRightY);

        break;
    }
}