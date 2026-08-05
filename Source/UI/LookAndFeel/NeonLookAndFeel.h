#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "../../Graphics/Knob/KnobGeometry.h"

class NeonLookAndFeel : public juce::LookAndFeel_V4
{
public:

    NeonLookAndFeel();

    void drawRotarySlider(
        juce::Graphics& g,
        int x,
        int y,
        int width,
        int height,
        float sliderPos,
        float rotaryStartAngle,
        float rotaryEndAngle,
        juce::Slider&) override;

private:

    // struct KnobGeometry
    // {
    //     juce::Rectangle<float> outerBounds;
    //     juce::Rectangle<float> innerBounds;
    //
    //     juce::Point<float> centre;
    //
    //     float radius = 0.0f;
    //     float innerRadius = 0.0f;
    //
    //     float tickRadius = 0.0f;      // NUEVO
    //
    //
    // };

    // KnobGeometry createGeometry(
    //     int x,
    //     int y,
    //     int width,
    //     int height) const;

    void drawKnobBody(
        juce::Graphics& g,
        const KnobGeometry& geometry);

    void drawOuterRing(
    juce::Graphics& g,
    const KnobGeometry& geometry);

    void drawProgressArc(
    juce::Graphics& g,
    const KnobGeometry& geometry,
    float sliderPos,
    float rotaryStartAngle,
    float rotaryEndAngle);

    void drawArcHalo(
    juce::Graphics& g,
    const juce::Path& arc,
    const KnobGeometry& geo);

    void drawArcGlow(
        juce::Graphics& g,
        const juce::Path& arc,
        const KnobGeometry& geo);

    void drawArcCore(
        juce::Graphics& g,
        const juce::Path& arc,
        const KnobGeometry& geo);

    void drawArcHighlight(
        juce::Graphics& g,
        const juce::Path& arc);

    void drawIndicator(
    juce::Graphics& g,
    const KnobGeometry& geometry,
    float sliderPos,
    float rotaryStartAngle,
    float rotaryEndAngle);

    void drawShadow(
    juce::Graphics& g,
    const KnobGeometry& geometry);

    void drawTickMarks(
    juce::Graphics& g,
    const KnobGeometry& geo,
    float startAngle,
    float endAngle);

    void drawMajorTicks(
        juce::Graphics& g,
        const KnobGeometry& geo,
        float startAngle,
        float endAngle);

    void drawTick(
        juce::Graphics& g,
        const KnobGeometry& geo,
        float angle,
        float innerRadius,
        float outerRadius,
        float thickness,
        juce::Colour colour);

    void drawLabels(
    juce::Graphics& g,
    const KnobGeometry& geo,
    float startAngle,
    float endAngle);


};

