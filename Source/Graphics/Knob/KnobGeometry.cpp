#include "KnobGeometry.h"

KnobGeometry createKnobGeometry(
    int x,
    int y,
    int width,
    int height)
{
    KnobGeometry geo;

    constexpr float knobDiameter = 190.0f; // aumentar para hacer mas grande el knob disminuir para lo contrario.

    const float diameter = knobDiameter;

    geo.outerBounds =
        juce::Rectangle<float>(diameter, diameter)
            .withCentre(
            {
                x + width * 0.5f,
                y + height * 0.5f
            });

    geo.innerBounds =
        geo.outerBounds.reduced(12.0f);

    geo.centre =
        geo.outerBounds.getCentre();

    geo.radius =
        geo.outerBounds.getWidth() * 0.5f;

    geo.innerRadius =
        geo.innerBounds.getWidth() * 0.5f;

    geo.tickRadius =
        geo.radius + 5.0f;

    return geo;
}