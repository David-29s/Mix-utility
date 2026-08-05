#pragma once
// #include "juce_audio_basics/buffers/juce_AudioSampleBuffer.h"
#include <juce_audio_basics/juce_audio_basics.h>

class GainProcessor
{
public:
    GainProcessor();
    ~GainProcessor();

    void setGain(float newGain);

    float getGain() const;

    void process(juce::AudioBuffer<float>& buffer);

private:
    float gain = 1.0f;
};