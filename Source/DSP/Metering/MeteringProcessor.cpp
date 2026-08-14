#include "MeteringProcessor.h"

void MeteringProcessor::processInput(
    const juce::AudioBuffer<float>& buffer)
{
    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    if (numSamples <= 0)
        return;

    // Left channel
    if (numChannels > 0)
    {
        const float peakL = buffer.getMagnitude(
            0,
            0,
            numSamples);

        const float dbL = juce::Decibels::gainToDecibels(
            peakL,
            -60.0f);

        inputLevelL.store(dbL);
    }

    // Right channel
    if (numChannels > 1)
    {
        const float peakR = buffer.getMagnitude(
            1,
            0,
            numSamples);

        const float dbR = juce::Decibels::gainToDecibels(
            peakR,
            -60.0f);

        inputLevelR.store(dbR);
    }
    else
    {
        inputLevelR.store(inputLevelL.load());
    }
}

float MeteringProcessor::getInputLevelL() const
{
    return inputLevelL.load();
}

float MeteringProcessor::getInputLevelR() const
{
    return inputLevelR.load();
}