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
    // Layer 01 - Panel
    //==========================================================

    static void drawPanel(
        juce::Graphics& g,
        juce::Rectangle<float> bounds);

    //==========================================================
    // Layer 02 - Outer Border
    //==========================================================

    static void drawOuterBorder(
        juce::Graphics& g,
        juce::Rectangle<float> bounds);

    //==========================================================
    // Layer 03 - Inner Border
    //==========================================================

    static void drawInnerBorder(
        juce::Graphics& g,
        juce::Rectangle<float> bounds);

    //==========================================================
    // Layer 04 - Neon
    //==========================================================

    static void drawNeon(
     juce::Graphics& g,
     juce::Rectangle<float> bounds,
     float glow);

    //==========================================================
    // Layer 04 - Top Decoration
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

    static void drawBottomNeon(
      juce::Graphics& g,
      juce::Rectangle<float> bounds,
      float glow);

    static void drawLeftNeon(
     juce::Graphics& g,
     juce::Rectangle<float> bounds,
     float glow);

    static void drawRightNeon(
     juce::Graphics& g,
     juce::Rectangle<float> bounds,
     float glow);

    //==========================================================
    // Layer 05 - Neon Glow
    //==========================================================

    static void drawNeonGlow(
        juce::Graphics& g,
        juce::Rectangle<float> bounds);
};