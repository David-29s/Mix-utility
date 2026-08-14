#pragma once

namespace FrameMetrics
{
    //==========================================================
    // Panel
    //==========================================================

    inline constexpr float cornerRadius = 14.0f; //Borde esquina del marco

    //==========================================================
    // Borders
    //==========================================================

    inline constexpr float outerBorderInset = 3.0f;

    inline constexpr float innerBorderInset = 8.0f;

    inline constexpr float borderThickness = 1.5f;

    inline constexpr float innerBorderThickness = 1.0f;

    //==========================================================
    // Neon
    //==========================================================

    inline constexpr float neonThickness = 1.0f;

    inline constexpr float neonInset = 8.0f;

    //==========================================================
    // Neon Glow
    //==========================================================

    inline constexpr int neonGlowLayers = 2;

    inline constexpr float neonGlowAlpha = 0.10f;

    //==========================================================
    // Side Gradient
    //==========================================================

    inline constexpr float cyanStart = 0.15f;

    inline constexpr float cyanMiddle = 0.40f;

    inline constexpr float cyanPeak = 0.75f;

    inline constexpr float magentaPeak = 0.80f;

    // Longitud de cada lado

    inline constexpr float topInset = 55.0f;

    inline constexpr float bottomInset = 20.0f;

    inline constexpr float leftInset = 40.0f;

    inline constexpr float rightInset = 40.0f;

    //==========================================================
    //Top Decoration
    //==========================================================

    inline constexpr float decorationY = 36.0f;

    inline constexpr float decorationMargin = 110.0f; // se corten de ambos lados, aumenta:

    inline constexpr float decorationGap = 50.0f;

    inline constexpr float decorationThickness = 1.0f;

    inline constexpr float decorationDotRadius = 2.5f;

    inline constexpr float decorationGlowRadius = 3.0f; //Glow de el punto de el texto gain
}