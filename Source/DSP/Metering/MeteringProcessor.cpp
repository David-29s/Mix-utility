#include "MeteringProcessor.h"
#include <cmath>

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

float MeteringProcessor::getOutputLevelL() const
{
    return outputLevelL.load();
}

float MeteringProcessor::getOutputLevelR() const
{
    return outputLevelR.load();
}
//--------------------------------------------------

//==========================================================
// Output metering
//==========================================================

void MeteringProcessor::processOutput(
    const juce::AudioBuffer<float>& buffer)
{

    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    if (buffer.getNumChannels() == 0 ||
        buffer.getNumSamples() == 0)
        return;

    const float leftPeak =
        buffer.getMagnitude(0, 0, buffer.getNumSamples());

    const float rightPeak =
        buffer.getNumChannels() > 1
            ? buffer.getMagnitude(1, 0, buffer.getNumSamples())
            : leftPeak;

    outputLevelL.store(
        juce::Decibels::gainToDecibels(
            leftPeak,
            -60.0f));

    outputLevelR.store(
        juce::Decibels::gainToDecibels(
            rightPeak,
            -60.0f));

    //==========================================================
    // Output Correlation
    //==========================================================

    if (numChannels > 1)
    {
        const float* left =
            buffer.getReadPointer(0);

        const float* right =
            buffer.getReadPointer(1);

        double sumLR = 0.0;
        double sumLL = 0.0;
        double sumRR = 0.0;

        for (int sample = 0;
             sample < numSamples;
             ++sample)
        {
            const double l = left[sample];
            const double r = right[sample];

            sumLR += l * r;
            sumLL += l * l;
            sumRR += r * r;
        }

        const double denominator =
            std::sqrt(sumLL * sumRR);

        float correlation = 0.0f;

        if (denominator > 1.0e-12)
        {
            correlation =
                static_cast<float>(
                    sumLR / denominator);
        }

        correlation =
            juce::jlimit(
                -1.0f,
                1.0f,
                correlation);

        outputCorrelation.store(
            correlation);
    }
    else
    {
        outputCorrelation.store(1.0f);
    }
}



//==========================================================
// Output correlation getter
//==========================================================

float MeteringProcessor::getOutputCorrelation() const
{
    return outputCorrelation.load();
}