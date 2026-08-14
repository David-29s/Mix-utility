#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

class MeteringProcessor
{
public:

    void processInput(const juce::AudioBuffer<float>& buffer);
    void processOutput(const juce::AudioBuffer<float>& buffer);

    float getInputLevelL() const;
    float getInputLevelR() const;

    float getOutputLevelL() const;
    float getOutputLevelR() const;

    float getOutputCorrelation() const;

private:

    std::atomic<float> inputLevelL { -60.0f };
    std::atomic<float> inputLevelR { -60.0f };

    std::atomic<float> outputLevelL { -60.0f };
    std::atomic<float> outputLevelR { -60.0f };

    std::atomic<float> outputCorrelation { 0.0f };


};