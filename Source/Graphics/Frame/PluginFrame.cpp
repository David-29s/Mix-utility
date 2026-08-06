#include "PluginFrame.h"
#include "../../Theme/NeonColours.h"
#include "../../Metrics/Frame/FrameMetrics.h"

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


    drawTopDecoration(
     g,
     area,
     decorationGlow);


    //Frame
    auto framePath =
    buildFramePath(area);

    drawFrameGlow(
        g,
        framePath,
        frameGlow);

    drawFrameStroke(
     g,
     framePath,
     area);

}



//============================================================
// Layer 01 - Panel
//============================================================

void PluginFrame::drawPanel(
    juce::Graphics& g,
    juce::Rectangle<float> bounds)
{
    constexpr float cornerRadius = 2.0f;

    g.setColour(NeonColours::frameBorder);

    g.fillRoundedRectangle(
        bounds,
        cornerRadius);
}



juce::Path PluginFrame::buildFramePath(
    juce::Rectangle<float> bounds)
{
    auto line =
        bounds.reduced(
            FrameMetrics::neonInset);

    const float r =
        FrameMetrics::cornerRadius;

    juce::Path path;

    //==========================================================
    // Inicio del marco
    //==========================================================

    path.startNewSubPath(
        line.getX(),
        line.getY() + FrameMetrics::topInset);

    //==========================================================
    // Lado izquierdo
    //==========================================================

    path.lineTo(
        line.getX(),
        line.getBottom() - r);

    //==========================================================
    // Curva inferior izquierda
    //==========================================================

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
    // Curva inferior derecha
    //==========================================================

    path.quadraticTo(
        line.getRight(),
        line.getBottom(),

        line.getRight(),
        line.getBottom() - r);

    //==========================================================
    // Lado derecho
    //==========================================================

    path.lineTo(
        line.getRight(),
        line.getY() + FrameMetrics::topInset);

    return path;
}

void PluginFrame::drawFrameGlow(
    juce::Graphics& g,
    const juce::Path& path,
    float glow)
{
    for (int i = FrameMetrics::neonGlowLayers;
         i >= 1;
         --i)
    {
        g.setColour(
            NeonColours::magenta.withAlpha(
                FrameMetrics::neonGlowAlpha *
                i *
                glow));

        g.strokePath(
            path,
            juce::PathStrokeType(
                FrameMetrics::neonThickness + i,
                juce::PathStrokeType::curved,
                juce::PathStrokeType::rounded));
    }
}

void PluginFrame::drawFrameStroke(
    juce::Graphics& g,
    const juce::Path& path,
    juce::Rectangle<float> bounds)
{
    auto line =
        bounds.reduced(
            FrameMetrics::neonInset);

    //==========================================================
    // Gradiente principal del marco
    //==========================================================

    juce::ColourGradient gradient(
        NeonColours::cyan,
        line.getX(),
        line.getCentreY(),

        NeonColours::magenta,
        line.getRight(),
        line.getCentreY(),

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
            FrameMetrics::neonThickness,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));
}


// decoracion lineas texto Gain
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