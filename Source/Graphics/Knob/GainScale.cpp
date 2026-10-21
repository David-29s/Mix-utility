#include "GainScale.h"

KnobScale createGainScale()
{
    KnobScale scale;
    juce::NormalisableRange<float> gainRange(-100.0f, 36.0f, 0.01f);
    gainRange.setSkewForCentre(0.0f);

    for (int tick = 0; tick <= 40; ++tick)
    {
        const float position = static_cast<float>(tick) / 40.0f;
        const float db = gainRange.convertFrom0to1(position);
        const bool major = tick % 5 == 0;
        juce::String label;

        if (tick == 0)
            label = juce::String::fromUTF8("-\xE2\x88\x9E");
        else if (tick == 40)
            label = "+36";
        else if (major)
            label = juce::String(juce::roundToInt(db));

        scale.addMark({ position, major, label, db, 16.0f });
    }

    return scale;
}
