#include "GainProcessor.h"

GainProcessor::GainProcessor()
{

}

GainProcessor::~GainProcessor()
{

}

void GainProcessor::setGain(float newGain)
{
    gain = newGain;
}

float GainProcessor::getGain() const
{
    return gain;
}

void GainProcessor::process(juce::AudioBuffer<float>& buffer)
{
    const int numChannels = buffer.getNumChannels();

    const int numSamples = buffer.getNumSamples();

    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < numSamples; ++sample)
        {
            channelData[sample] *= gain;

        }
    }
}