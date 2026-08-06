#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../../Animation/NeonAnimator.h"

class PluginFrame
{
public:

    static void draw(
     juce::Graphics& g,
     juce::Rectangle<int> bounds,
     const NeonAnimator& animator);

private:

    //==========================================================
    // Panel
    //==========================================================

    static void drawPanel(
        juce::Graphics& g,
        juce::Rectangle<float> bounds);


    //==========================================================
    //Top Decoration
    //==========================================================

    static void drawTopDecoration(
    juce::Graphics& g,
    juce::Rectangle<float> bounds,
    float glow);

    static void drawLeftDecoration(
     juce::Graphics& g,
     juce::Rectangle<float> bounds,
     float glow);

    static void drawRightDecoration(
        juce::Graphics& g,
        juce::Rectangle<float> bounds,
        float glow);


    //==========================================================
    // Neon Glow
    //==========================================================

    static void drawNeonGlow(
        juce::Graphics& g,
        juce::Rectangle<float> bounds);

    //Nuevo
    static juce::Path buildFramePath(
     juce::Rectangle<float> bounds);

    static void drawFrameGlow(
    juce::Graphics& g,
    const juce::Path& path,
    float glow);

    static void drawFrameStroke(
     juce::Graphics& g,
     const juce::Path& path,
     juce::Rectangle<float> bounds);
};