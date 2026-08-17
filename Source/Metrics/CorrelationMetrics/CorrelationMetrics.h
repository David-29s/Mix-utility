//
// Created by david sanchez on 17/08/2026.
//

#ifndef AUDIO_PLUGIN_EXAMPLE_CORRELATIONMETRICS_H
#define AUDIO_PLUGIN_EXAMPLE_CORRELATIONMETRICS_H

#endif //AUDIO_PLUGIN_EXAMPLE_CORRELATIONMETRICS_H
#pragma once

namespace CorrelationMetrics
{

    //==========================================================
    // Component Bounds
    //==========================================================

    inline constexpr int x = 260;
    inline constexpr int y = 380;
    inline constexpr int width = 263;
    inline constexpr int height = 105;

    //==========================================================
    // Panel
    //==========================================================

    inline constexpr float panelCornerRadius = 8.0f;
    inline constexpr float panelBorderInset = 0.5f;
    inline constexpr float panelBorderThickness = 1.5f;


    //==========================================================
    // Title
    //==========================================================

    inline constexpr float titleHeight = 24.0f;
    inline constexpr float titleFontSize = 13.0f;


    //==========================================================
    // Scale
    //==========================================================

    inline constexpr float scaleSidePadding = 12.0f;
    inline constexpr float scaleHeight = 22.0f;
    inline constexpr float scaleLabelWidth = 30.0f;
    inline constexpr float scaleFontSize = 12.0f;

    inline constexpr float scaleLabelOffsetY = -9.0f;


    //==========================================================
    // Tick Marks
    //==========================================================

    inline constexpr int numberOfTicks = 17;

    inline constexpr float majorTickHeight = 8.0f;
    inline constexpr float minorTickHeight = 4.0f;

    inline constexpr float minorTickOffsetY = 5.0f;

    inline constexpr float majorTickWidth = 1.5f;
    inline constexpr float minorTickWidth = 1.0f;

    inline constexpr float tickOffsetY = -11.0f;


    //==========================================================
    // Correlation Bar
    //==========================================================

    inline constexpr float barHeight = 18.0f;
    inline constexpr float barCornerRadius = 2.0f;


    inline constexpr float barOffsetX = 0.0f;
    inline constexpr float barOffsetY = 4.0f;

    //==========================================================
    // Marker
    //==========================================================

    inline constexpr float markerWidth = 3.0f;
    inline constexpr float markerTopExtension = 5.0f;
    inline constexpr float markerBottomExtension = 5.0f;


    //==========================================================
    // Zone Labels
    //==========================================================

    inline constexpr float zoneLabelY = 54.0f;
    inline constexpr float zoneLabelHeight = 16.0f;
    inline constexpr float zoneLabelFontSize = 11.2f;

    inline constexpr float zoneLabelWidth = 90.0f;

    inline constexpr float outOfPhasePosition = 0.125f;
    inline constexpr float lowPosition = 0.50f;
    inline constexpr float inPhasePosition = 0.875f;


    //==========================================================
    // Zone Guide Lines
    //==========================================================

    inline constexpr float guideStartOffset = 6.5f;
    inline constexpr float guideLength = 12.0f;

    inline constexpr float guideWidth = 1.0f;
}