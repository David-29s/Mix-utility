#include "PluginFrame.h"
#include "../../UI/Theme/NeonColours.h"
#include "FrameMetrics.h"

void PluginFrame::draw(
    juce::Graphics& g,
    juce::Rectangle<int> bounds,
    const NeonAnimator& animator)
{
    auto area = bounds.toFloat();

    const float decorationGlow =
     animator.getDecorationGlow();

    const float frameGlow =
        animator.getFrameGlow();

    drawPanel(g, area);

    drawOuterBorder(g, area);

    // drawInnerBorder(g, area);

    drawTopDecoration(
     g,
     area,
     decorationGlow);

    drawNeon(
        g,
        area,
        frameGlow);

    // drawNeonGlow(g, area);
}

//============================================================
// Layer 01 - Panel
//============================================================

void PluginFrame::drawPanel(
    juce::Graphics& g,
    juce::Rectangle<float> bounds)
{
    constexpr float cornerRadius = 20.0f;

    g.setColour(NeonColours::frameBorder);

    g.fillRoundedRectangle(
        bounds,
        cornerRadius);
}

//============================================================
// Layer 02 - Outer Border
//============================================================

void PluginFrame::drawOuterBorder(
    juce::Graphics& g,
    juce::Rectangle<float> bounds)
{
    constexpr float cornerRadius = 20.0f; //Redondeo

    g.setColour(NeonColours::frameBorder);

    g.drawRoundedRectangle(
        bounds.reduced(3.0f),
        cornerRadius,
        1.5f);
}

//============================================================
// Layer 03 - Inner Border
//============================================================

// void PluginFrame::drawInnerBorder(
//     juce::Graphics& g,
//     juce::Rectangle<float> bounds)
// {
//     constexpr float cornerRadius = 18.0f;
//
//     auto innerBounds =
//         bounds.reduced(8.0f);
//
//     g.setColour(
//         NeonColours::frameHighlight);
//
//     g.drawRoundedRectangle(
//         innerBounds,
//         cornerRadius,
//         1.0f);
// }

//============================================================
// Layer 04 - Neon Outline
//============================================================

void PluginFrame::drawNeon(
    juce::Graphics& g,
    juce::Rectangle<float> bounds,
    float glow)
{
    drawBottomNeon(
        g,
        bounds,
        glow);

    drawLeftNeon(
        g,
        bounds,
        glow);

    drawRightNeon(
        g,
        bounds,
        glow);
}


void PluginFrame::drawBottomNeon(
    juce::Graphics& g,
    juce::Rectangle<float> bounds,
    float glow)
{
    auto line =
        bounds.reduced(FrameMetrics::neonInset);

    const float r =
        FrameMetrics::cornerRadius;

    juce::Path path;

    //==========================================================
    // Esquina inferior izquierda
    //==========================================================

    path.startNewSubPath(
        line.getX(),
        line.getBottom() - r);

    path.quadraticTo(
        line.getX(),
        line.getBottom(),
        line.getX() + r,
        line.getBottom());

    //==========================================================
    // Línea inferior
    //==========================================================

    path.lineTo(
        line.getRight() - r,
        line.getBottom());


    //==========================================================
    // Esquina inferior derecha
    //==========================================================

    path.quadraticTo(
        line.getRight(),
        line.getBottom(),
        line.getRight(),
        line.getBottom() - r);

    //==========================================================
    // Glow del marco
    //==========================================================

    for (int i = FrameMetrics::neonGlowLayers; i >= 1; --i)
    {
        g.setColour(
            NeonColours::magenta.withAlpha(
                0.03f * i * glow));

        g.strokePath(
            path,
            juce::PathStrokeType(
                FrameMetrics::neonThickness + i,
                juce::PathStrokeType::curved,
                juce::PathStrokeType::rounded));
    }

    //==========================================================
    // Línea principal del marco
    //==========================================================

    g.setColour(
        NeonColours::magenta);

    g.strokePath(
        path,
        juce::PathStrokeType(
            FrameMetrics::neonThickness,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));
}

void PluginFrame::drawLeftNeon(
    juce::Graphics& g,
    juce::Rectangle<float> bounds,
    float glow)
{
    auto line =
        bounds.reduced(
            FrameMetrics::neonInset);

    //==========================================================
    // Geometría de la línea
    //==========================================================

    juce::Path path;

    path.startNewSubPath(
        line.getX(),
        line.getY() + FrameMetrics::topInset);

    path.lineTo(
        line.getX(),
        line.getBottom() - FrameMetrics::bottomInset);

    //==========================================================
    // Glow del marco
    //==========================================================

    for (int i = FrameMetrics::neonGlowLayers; i >= 1; --i)
    {
        juce::ColourGradient glowGradient(
            NeonColours::magenta.withAlpha(0.0f),
            line.getX(),
            line.getY(),

            NeonColours::magenta.withAlpha(
                0.03f * i * glow),
            line.getX(),
            line.getBottom(),

            false);

        glowGradient.addColour(
     0.0,
     NeonColours::cyan.withAlpha(
         FrameMetrics::cyanStart * glow));

        glowGradient.addColour(
            0.20,
            NeonColours::cyan.withAlpha(
                FrameMetrics::cyanMiddle * glow));

        glowGradient.addColour(
            0.40,
            NeonColours::cyan.withAlpha(
                FrameMetrics::cyanPeak * glow));

        glowGradient.addColour(
            0.60,
            NeonColours::magenta.withAlpha(
                FrameMetrics::magentaPeak * glow));

        g.setGradientFill(
            glowGradient);

        g.strokePath(
            path,
            juce::PathStrokeType(
                FrameMetrics::neonThickness + i,
                juce::PathStrokeType::curved,
                juce::PathStrokeType::rounded));
    }

    //==========================================================
    // Gradiente principal
    //==========================================================

    juce::ColourGradient gradient(
        NeonColours::magenta.withAlpha(0.0f),
        line.getX(),
        line.getY(),

        NeonColours::magenta,
        line.getX(),
        line.getBottom(),

        false);

    gradient.addColour(
     0.0,
     NeonColours::cyan.withAlpha(
         FrameMetrics::cyanStart));

    gradient.addColour(
        0.20,
        NeonColours::cyan.withAlpha(
            FrameMetrics::cyanMiddle));

    gradient.addColour(
        0.40,
        NeonColours::cyan.withAlpha(
            FrameMetrics::cyanPeak));

    gradient.addColour(
        0.60,
        NeonColours::magenta.withAlpha(
            FrameMetrics::magentaPeak));

    gradient.addColour(
        1.0,
        NeonColours::magenta);

    g.setGradientFill(
        gradient);

    //==========================================================
    // Línea principal del marco
    //==========================================================

    g.strokePath(
        path,
        juce::PathStrokeType(
            FrameMetrics::neonThickness));
}

void PluginFrame::drawRightNeon(
    juce::Graphics& g,
    juce::Rectangle<float> bounds,
    float glow)
{
    auto line =
        bounds.reduced(
            FrameMetrics::neonInset);

    //==========================================================
    // Geometría de la línea
    //==========================================================

    juce::Path path;

    path.startNewSubPath(
        line.getRight(),
        line.getY() + FrameMetrics::topInset);

    path.lineTo(
        line.getRight(),
        line.getBottom() - FrameMetrics::bottomInset);

    //==========================================================
    // Glow del marco
    //==========================================================

    for (int i = FrameMetrics::neonGlowLayers; i >= 1; --i)
    {
        juce::ColourGradient glowGradient(
            NeonColours::magenta.withAlpha(0.0f),
            line.getRight(),
            line.getY(),

            NeonColours::magenta.withAlpha(
                FrameMetrics::neonGlowAlpha * i * glow),
            line.getRight(),
            line.getBottom(),

            false);

        glowGradient.addColour(
            0.0,
            NeonColours::cyan.withAlpha(
                FrameMetrics::cyanStart * glow));

        glowGradient.addColour(
            0.20,
            NeonColours::cyan.withAlpha(
                FrameMetrics::cyanMiddle * glow));

        glowGradient.addColour(
            0.40,
            NeonColours::cyan.withAlpha(
                FrameMetrics::cyanPeak * glow));

        glowGradient.addColour(
            0.60,
            NeonColours::magenta.withAlpha(
                FrameMetrics::magentaPeak * glow));

        glowGradient.addColour(
            1.0,
            NeonColours::magenta.withAlpha(
                glow));

        g.setGradientFill(
            glowGradient);

        g.strokePath(
            path,
            juce::PathStrokeType(
                FrameMetrics::neonThickness + i,
                juce::PathStrokeType::curved,
                juce::PathStrokeType::rounded));
    }

    //==========================================================
    // Gradiente principal
    //==========================================================

    juce::ColourGradient gradient(
        NeonColours::magenta.withAlpha(0.0f),
        line.getRight(),
        line.getY(),

        NeonColours::magenta,
        line.getRight(),
        line.getBottom(),

        false);

    gradient.addColour(
        0.0,
        NeonColours::cyan.withAlpha(
            FrameMetrics::cyanStart));

    gradient.addColour(
        0.20,
        NeonColours::cyan.withAlpha(
            FrameMetrics::cyanMiddle));

    gradient.addColour(
        0.40,
        NeonColours::cyan.withAlpha(
            FrameMetrics::cyanPeak));

    gradient.addColour(
        0.60,
        NeonColours::magenta.withAlpha(
            FrameMetrics::magentaPeak));

    gradient.addColour(
        1.0,
        NeonColours::magenta);

    g.setGradientFill(
        gradient);

    //==========================================================
    // Línea principal del marco
    //==========================================================

    g.strokePath(
        path,
        juce::PathStrokeType(
            FrameMetrics::neonThickness));
}

//============================================================
// Layer 05 - Neon Glow
//============================================================

// void PluginFrame::drawNeonGlow(
//     juce::Graphics& g,
//     juce::Rectangle<float> bounds)
// {
//     constexpr float cornerRadius = 18.0f;
//
//     auto glowBounds =
//         bounds.reduced(8.0f);
//
//     for (int i = 0; i < 5; ++i)
//     {
//         g.setColour(
//             NeonColours::magenta.withAlpha(
//                 0.10f - i * 0.018f));
//
//         g.drawRoundedRectangle(
//             glowBounds.expanded((float)i),
//             cornerRadius + i,
//             2.0f);
//     }
// }


// decoracion texto de Gain
void PluginFrame::drawTopDecoration(
    juce::Graphics& g,
    juce::Rectangle<float> bounds,
    float glow)
{
    drawLeftDecoration(
    g,
    bounds,
    glow);

    drawRightDecoration(
        g,
        bounds,
        glow);
}

void PluginFrame::drawLeftDecoration(
    juce::Graphics& g,
    juce::Rectangle<float> bounds,
    float glow)
{
    const float y =
        FrameMetrics::decorationY;

    const float startX =
        FrameMetrics::decorationMargin;

    const float endX =
        bounds.getCentreX() -
        FrameMetrics::decorationGap;

    //==========================================================
    // Línea
    //==========================================================

    juce::Path path;

    path.startNewSubPath(
        startX,
        y);

    path.lineTo(
        endX,
        y);

    //==========================================================
    // Glow de la línea
    //==========================================================

    for (int i = 3; i >= 1; --i)
    {
        juce::ColourGradient glowGradient(
            NeonColours::cyan.withAlpha(0.05f * i * glow),
            startX,
            y,

            NeonColours::cyan.withAlpha(0.0f),
            endX,
            y,

            false);

        glowGradient.addColour(
            0.0,
            NeonColours::cyan.withAlpha(0.05f * i * glow));

        glowGradient.addColour(
            0.70,
            NeonColours::cyan.withAlpha(0.05f * i * glow));

        glowGradient.addColour(
            1.0,
            NeonColours::cyan.withAlpha(0.0f));

        g.setGradientFill(glowGradient);

        g.strokePath(
            path,
            juce::PathStrokeType(
                FrameMetrics::decorationThickness + i,
                juce::PathStrokeType::curved,
                juce::PathStrokeType::rounded));
    }

    //==========================================================
    // Gradiente
    //==========================================================

    juce::ColourGradient gradient(
        NeonColours::cyan,
        startX,
        y,

        NeonColours::cyan.withAlpha(0.0f),
        endX,
        y,

        false);

    // Mantener brillante la mayor parte
    gradient.addColour(
        0.0,
        NeonColours::cyan);

    gradient.addColour(
        0.65,
        NeonColours::cyan);

    gradient.addColour(
        1.0,
        NeonColours::cyan.withAlpha(0.0f));

    g.setGradientFill(
        gradient);

    g.strokePath(
        path,
        juce::PathStrokeType(
            FrameMetrics::decorationThickness,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));

    //==========================================================
    // Glow del punto
    //==========================================================

    for (int i = 5; i >= 1; --i)
    {
        g.setColour(
            NeonColours::cyan.withAlpha(
                FrameMetrics::neonGlowAlpha * i * glow));

        g.fillEllipse(
            startX
                - FrameMetrics::decorationGlowRadius
                - i,

            y
                - FrameMetrics::decorationGlowRadius
                - i,

            (FrameMetrics::decorationGlowRadius + i) * 2.0f,

            (FrameMetrics::decorationGlowRadius + i) * 2.0f);
    }

    //==========================================================
    // Punto
    //==========================================================

    g.setColour(
        NeonColours::cyan);

    g.fillEllipse(
        startX
            - FrameMetrics::decorationDotRadius,

        y
            - FrameMetrics::decorationDotRadius,

        FrameMetrics::decorationDotRadius * 2.0f,

        FrameMetrics::decorationDotRadius * 2.0f);
}

void PluginFrame::drawRightDecoration(
    juce::Graphics& g,
    juce::Rectangle<float> bounds,
    float glow)
{
    const float y =
        FrameMetrics::decorationY;

    const float startX =
        bounds.getCentreX() +
        FrameMetrics::decorationGap;

    const float endX =
        bounds.getWidth() -
        FrameMetrics::decorationMargin;

    //==========================================================
    // Línea
    //==========================================================

    juce::Path path;

    path.startNewSubPath(
        startX,
        y);

    path.lineTo(
        endX,
        y);

    //==========================================================
    // Glow de la línea
    //==========================================================

    for (int i = 3; i >= 1; --i)
    {
        juce::ColourGradient glowGradient(
            NeonColours::magenta.withAlpha(0.0f),
            startX,
            y,

            NeonColours::magenta.withAlpha(0.05f * i * glow),
            endX,
            y,

            false);

        glowGradient.addColour(
            0.0,
            NeonColours::magenta.withAlpha(0.0f));

        glowGradient.addColour(
            0.30,
            NeonColours::magenta.withAlpha(0.05f * i * glow));

        glowGradient.addColour(
            1.0,
            NeonColours::magenta.withAlpha(0.05f * i * glow));

        g.setGradientFill(
            glowGradient);

        g.strokePath(
            path,
            juce::PathStrokeType(
                FrameMetrics::decorationThickness + i,
                juce::PathStrokeType::curved,
                juce::PathStrokeType::rounded));
    }

    //==========================================================
    // Gradiente
    //==========================================================

    juce::ColourGradient gradient(
        NeonColours::magenta.withAlpha(0.0f),
        startX,
        y,

        NeonColours::magenta,
        endX,
        y,

        false);

    gradient.addColour(
        0.0,
        NeonColours::magenta.withAlpha(0.0f));

    gradient.addColour(
        0.35,
        NeonColours::magenta);

    gradient.addColour(
        1.0,
        NeonColours::magenta);

    g.setGradientFill(
        gradient);

    g.strokePath(
        path,
        juce::PathStrokeType(
            FrameMetrics::decorationThickness,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));

    //==========================================================
    // Glow del punto
    //==========================================================

    for (int i = 5; i >= 1; --i)
    {
        g.setColour(
            NeonColours::magenta.withAlpha(
                FrameMetrics::neonGlowAlpha * i * glow));

        g.fillEllipse(
            endX
                - FrameMetrics::decorationGlowRadius
                - i,

            y
                - FrameMetrics::decorationGlowRadius
                - i,

            (FrameMetrics::decorationGlowRadius + i) * 2.0f,

            (FrameMetrics::decorationGlowRadius + i) * 2.0f);
    }

    //==========================================================
    // Punto
    //==========================================================

    g.setColour(
        NeonColours::magenta);

    g.fillEllipse(
        endX
            - FrameMetrics::decorationDotRadius,

        y
            - FrameMetrics::decorationDotRadius,

        FrameMetrics::decorationDotRadius * 2.0f,

        FrameMetrics::decorationDotRadius * 2.0f);
}