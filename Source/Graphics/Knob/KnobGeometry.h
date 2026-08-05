#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

struct KnobGeometry
{
    juce::Rectangle<float> outerBounds;
    juce::Rectangle<float> innerBounds;

    juce::Point<float> centre;

    float radius = 0.0f;
    float innerRadius = 0.0f;
    float tickRadius = 0.0f;
};

KnobGeometry createKnobGeometry(
    int x,
    int y,
    int width,
    int height);