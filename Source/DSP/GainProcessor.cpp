#include "GainProcessor.h"
#include <cmath>
#include <utility>

//==========================================================
// Constructor / Destructor
//==========================================================

GainProcessor::GainProcessor()
{

}

GainProcessor::~GainProcessor()
{

}

void GainProcessor::prepare(
    double newSampleRate,
    int samplesPerBlock)
{
    sampleRate = newSampleRate;

    //==========================================================
    // ProcessSpec
    //==========================================================

    juce::dsp::ProcessSpec spec;

    spec.sampleRate = sampleRate;
    spec.maximumBlockSize =
        static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = 1;

    //==========================================================
    // Smoothing
    //==========================================================

    smoothedGain.reset(sampleRate, 0.02);
    smoothedGain.setCurrentAndTargetValue(1.0f);

    smoothedOutputTrim.reset(sampleRate, 0.02);
    smoothedOutputTrim.setCurrentAndTargetValue(1.0f);

    smoothedStereoSwap.reset(sampleRate, 0.01);
    smoothedStereoSwap.setCurrentAndTargetValue(0.0f);

    smoothedPan.reset(sampleRate, 0.02);
    smoothedPan.setCurrentAndTargetValue(0.0f);

    smoothedWidth.reset(sampleRate, 0.02);
    smoothedWidth.setCurrentAndTargetValue(1.0f);

    smoothedBypassMix.reset(sampleRate, 0.015);
    smoothedBypassMix.setCurrentAndTargetValue(0.0f);
    smoothedMonoMix.reset(sampleRate, 0.015);
    smoothedMonoMix.setCurrentAndTargetValue(0.0f);
    smoothedPhaseMix.reset(sampleRate, 0.015);
    smoothedPhaseMix.setCurrentAndTargetValue(0.0f);
    smoothedMuteGain.reset(sampleRate, 0.015);
    smoothedMuteGain.setCurrentAndTargetValue(1.0f);

    smoothedBassMonoMix.reset(sampleRate, 0.025);
    smoothedBassMonoMix.setCurrentAndTargetValue(0.0f);
    smoothedBassMonoListenMix.reset(sampleRate, 0.015);
    smoothedBassMonoListenMix.setCurrentAndTargetValue(0.0f);

    // Preallocate dry audio for click-free bypass crossfades on the audio thread.
    bypassBuffer.setSize(2, juce::jmax(samplesPerBlock, 8192), false, true, true);

    smoothedBassMonoCrossover.reset(
        sampleRate,
        0.02);

    smoothedBassMonoCrossover.setCurrentAndTargetValue(
        bassMonoCrossover);

    //==========================================================
    // Bass Mono Filters
    //==========================================================

    bassMonoFilterL.prepare(spec);
    bassMonoFilterR.prepare(spec);

    bassMonoFilterL.setCutoffFrequency(
        bassMonoCrossover);

    bassMonoFilterR.setCutoffFrequency(
        bassMonoCrossover);
}

//==========================================================
// Parameters
//==========================================================

// Establece la ganancia lineal que se aplicará al audio.
void GainProcessor::setGain(float newGain)
{
    newGain = juce::jlimit(
        0.0f,
        63.0957344f, // +36 dB
        newGain);

    smoothedGain.setTargetValue(newGain);
}

void GainProcessor::setOutputTrim(float newGain)
{
    smoothedOutputTrim.setTargetValue(
        juce::jlimit(0.0f, 4.0f, newGain));
}

void GainProcessor::setStereoSwap(bool shouldSwap)
{
    smoothedStereoSwap.setTargetValue(shouldSwap ? 1.0f : 0.0f);
}

void GainProcessor::requestAutoGainCapture()
{
    autoGainCancelled.store(false);
    autoGainCaptureState.store(1);
    autoGainRequested.store(true);
}

void GainProcessor::cancelAutoGainCapture()
{
    autoGainCancelled.store(true);
    autoGainRequested.store(false);
    autoGainCaptureState.store(0);
}

int GainProcessor::getAutoGainCaptureState() const
{
    return autoGainCaptureState.load();
}

float GainProcessor::getAutoGainResultDb() const
{
    return autoGainResultDb.load();
}

void GainProcessor::processAutoGainCapture(
    const juce::AudioBuffer<float>& buffer)
{
    if (autoGainCancelled.exchange(false))
    {
        autoGainIsCapturing = false;
        autoGainPowerSum = 0.0;
        autoGainFramesCollected = 0;
        return;
    }

    if (autoGainRequested.exchange(false))
    {
        autoGainIsCapturing = true;
        autoGainPowerSum = 0.0;
        autoGainFramesCollected = 0;
        autoGainCaptureState.store(1);
    }

    if (!autoGainIsCapturing)
        return;

    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();
    if (numChannels <= 0 || numSamples <= 0)
        return;

    const auto requiredFrames = static_cast<std::int64_t>(juce::roundToInt(sampleRate));
    const auto framesToCapture = static_cast<int>(juce::jmin(
        static_cast<std::int64_t>(numSamples),
        requiredFrames - autoGainFramesCollected));

    // Average channel energy per frame, so mono and stereo use the same
    // exact one-second analysis window, independent of block size.
    for (int sample = 0; sample < framesToCapture; ++sample)
    {
        double frameEnergy = 0.0;
        for (int channel = 0; channel < numChannels; ++channel)
        {
            const double value = buffer.getReadPointer(channel)[sample];
            frameEnergy += value * value;
        }
        autoGainPowerSum += frameEnergy / static_cast<double>(numChannels);
    }

    autoGainFramesCollected += framesToCapture;
    if (autoGainFramesCollected < requiredFrames)
        return;

    autoGainIsCapturing = false;
    const double meanPower = autoGainPowerSum
        / static_cast<double>(autoGainFramesCollected);
    const float measuredRms = static_cast<float>(std::sqrt(meanPower));
    const float minimumRms = juce::Decibels::decibelsToGain(-55.0f);

    if (measuredRms <= minimumRms)
    {
        autoGainCaptureState.store(3);
        return;
    }

    constexpr float targetRmsDb = -18.0f;
    const float measuredRmsDb = juce::Decibels::gainToDecibels(measuredRms);
    const float outputTrimDb = juce::Decibels::gainToDecibels(
        smoothedOutputTrim.getTargetValue(), -36.0f);
    const float requiredGainDb = juce::jlimit(
        -100.0f,
        36.0f,
        targetRmsDb - measuredRmsDb - outputTrimDb);

    autoGainResultDb.store(requiredGainDb);
    autoGainCaptureState.store(2);
}

// Devuelve la ganancia actual.
float GainProcessor::getGain() const
{
    return smoothedGain.getCurrentValue();
}

//==========================================================
// Pan
//==========================================================

// Establece el paneo.
// El valor recibido está en el rango -1.0 a +1.0.
void GainProcessor::setPan(
    float newPan)
{
    newPan = juce::jlimit(
        -1.0f,
        1.0f,
        newPan);

    smoothedPan.setTargetValue(newPan);
}


// Devuelve el paneo actual.
float GainProcessor::getPan() const
{
    return smoothedPan.getCurrentValue();
}

//==========================================================
// Width
//==========================================================

void GainProcessor::setWidth(
    float newWidth)
{
    newWidth = juce::jlimit(
        0.0f,
        4.0f,
        newWidth);

    smoothedWidth.setTargetValue(newWidth);
}


float GainProcessor::getWidth() const
{
    return smoothedWidth.getCurrentValue();
}

//==========================================================
// Bass Mono
//==========================================================

void GainProcessor::setBassMono(
    bool shouldBeBassMono)
{
    bassMono = shouldBeBassMono;
    smoothedBassMonoMix.setTargetValue(shouldBeBassMono ? 1.0f : 0.0f);
}

bool GainProcessor::isBassMonoEnabled() const
{
    return bassMono;
}

void GainProcessor::setBassMonoCrossover(
    float newCrossover)
{
    newCrossover = juce::jlimit(
        20.0f,
        500.0f,
        newCrossover);

    bassMonoCrossover = newCrossover;

    smoothedBassMonoCrossover.setTargetValue(
        newCrossover);
}

float GainProcessor::getBassMonoCrossover() const
{
    return bassMonoCrossover;
}

//==========================================================
// States
//==========================================================

// Activa o desactiva el bypass del procesador.
void GainProcessor::setBypass(
    bool shouldBypass)
{
    bypass = shouldBypass;
    smoothedBypassMix.setTargetValue(shouldBypass ? 1.0f : 0.0f);
}

// Devuelve el estado actual del bypass.
bool GainProcessor::isBypassed() const
{
    return bypass;
}

//----------------------------------------------------------

// Activa o desactiva el modo Mono.
void GainProcessor::setMono(
    bool shouldBeMono)
{
    mono = shouldBeMono;
    smoothedMonoMix.setTargetValue(shouldBeMono ? 1.0f : 0.0f);
}

// Devuelve el estado actual del modo Mono.
bool GainProcessor::isMono() const
{
    return mono;
}

//----------------------------------------------------------

// Activa o desactiva la inversión de fase.
void GainProcessor::setPhase(
    bool shouldBePhase)
{
    phase = shouldBePhase;
    smoothedPhaseMix.setTargetValue(shouldBePhase ? 1.0f : 0.0f);
}

// Devuelve el estado actual de la fase.
bool GainProcessor::isPhase() const
{
    return phase;
}

//----------------------------------------------------------

// Activa o desactiva el mute.
void GainProcessor::setMute(
    bool shouldBeMute)
{
    mute = shouldBeMute;
    smoothedMuteGain.setTargetValue(shouldBeMute ? 0.0f : 1.0f);
}

// Devuelve el estado actual del mute.
bool GainProcessor::isMuted() const
{
    return mute;
}
//==========================================================
// Processing
//==========================================================

// Procesa el bloque completo de audio.
void GainProcessor::process(
    juce::AudioBuffer<float>& buffer)
{
    processAutoGainCapture(buffer);

    const int numChannels = juce::jmin(buffer.getNumChannels(), 2);
    const int numSamples = buffer.getNumSamples();
    const int drySamples = juce::jmin(numSamples, bypassBuffer.getNumSamples());

    for (int channel = 0; channel < numChannels; ++channel)
        bypassBuffer.copyFrom(channel, 0, buffer, channel, 0, drySamples);

    //==========================================================
    // Processing Stages
    //==========================================================

    //==========================================================
    // Bass Mono / Listen
    //==========================================================

    processBassMono(buffer);

    //==========================================================
    // Other Processing
    //==========================================================

    processMono(buffer);

    processPhase(buffer);

    processPan(buffer);

    processWidth(buffer);

    processGain(buffer);

    processStereoSwap(buffer);

    processOutputTrim(buffer);

    processMute(buffer);

    // Blend processed audio with the dry input when bypassing. The dry buffer
    // is prepared in prepare(), so this path never allocates on the audio thread.
    for (int sample = 0; sample < numSamples; ++sample)
    {
        const float bypassMix = smoothedBypassMix.getNextValue();
        if (sample >= drySamples)
            continue;

        const float wetMix = 1.0f - bypassMix;
        for (int channel = 0; channel < numChannels; ++channel)
        {
            const float dry = bypassBuffer.getSample(channel, sample);
            float& wet = buffer.getWritePointer(channel)[sample];
            wet = wet * wetMix + dry * bypassMix;
        }
    }
}

//==========================================================
// Processing Stages
//==========================================================

// Procesa el estado de mute con una transición de ganancia suavizada.
void GainProcessor::processMute(juce::AudioBuffer<float>& buffer)
{
    const int numChannels = juce::jmin(buffer.getNumChannels(), 2);
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        const float gain = smoothedMuteGain.getNextValue();
        for (int channel = 0; channel < numChannels; ++channel)
            buffer.getWritePointer(channel)[sample] *= gain;
    }
}

//----------------------------------------------------------
// Aplica el procesamiento Mono.
void GainProcessor::processMono(
    juce::AudioBuffer<float>& buffer)
{
    if (buffer.getNumChannels() < 2)
        return;

    auto* left = buffer.getWritePointer(0);
    auto* right = buffer.getWritePointer(1);

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        const float inputLeft = left[sample];
        const float inputRight = right[sample];
        const float monoSample = (inputLeft + inputRight) * 0.5f;
        const float monoMix = smoothedMonoMix.getNextValue();
        left[sample] = inputLeft + (monoSample - inputLeft) * monoMix;
        right[sample] = inputRight + (monoSample - inputRight) * monoMix;
    }
}

//----------------------------------------------------------
// Aplica la inversión de fase.
void GainProcessor::processPhase(
    juce::AudioBuffer<float>& buffer)
{
    const int numChannels = juce::jmin(buffer.getNumChannels(), 2);
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        const float phaseMix = smoothedPhaseMix.getNextValue();
        const float polarity = 1.0f - 2.0f * phaseMix;
        for (int channel = 0; channel < numChannels; ++channel)
            buffer.getWritePointer(channel)[sample] *= polarity;
    }
}

//----------------------------------------------------------

// Aplica la ganancia al bloque de audio.
void GainProcessor::processGain(
    juce::AudioBuffer<float>& buffer)
{
    if (!smoothedGain.isSmoothing())
    {
        buffer.applyGain(smoothedGain.getCurrentValue());
        return;
    }

    const int numChannels =
        buffer.getNumChannels();

    const int numSamples =
        buffer.getNumSamples();

    for (int sample = 0;
         sample < numSamples;
         ++sample)
    {
        const float currentGain =
            smoothedGain.getNextValue();

        for (int channel = 0;
             channel < numChannels;
             ++channel)
        {
            buffer.getWritePointer(channel)[sample]
                *= currentGain;
        }
    }
}

void GainProcessor::processOutputTrim(
    juce::AudioBuffer<float>& buffer)
{
    if (!smoothedOutputTrim.isSmoothing())
    {
        buffer.applyGain(smoothedOutputTrim.getCurrentValue());
        return;
    }

    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    for (int sample = 0; sample < numSamples; ++sample)
    {
        const float trim = smoothedOutputTrim.getNextValue();
        for (int channel = 0; channel < numChannels; ++channel)
            buffer.getWritePointer(channel)[sample] *= trim;
    }
}

void GainProcessor::processStereoSwap(
    juce::AudioBuffer<float>& buffer)
{
    if (buffer.getNumChannels() < 2)
        return;

    auto* left = buffer.getWritePointer(0);
    auto* right = buffer.getWritePointer(1);
    const int numSamples = buffer.getNumSamples();

    if (!smoothedStereoSwap.isSmoothing())
    {
        if (smoothedStereoSwap.getCurrentValue() < 0.5f)
            return;

        for (int sample = 0; sample < numSamples; ++sample)
            std::swap(left[sample], right[sample]);

        return;
    }

    for (int sample = 0; sample < numSamples; ++sample)
    {
        const float mix = smoothedStereoSwap.getNextValue();
        const float originalLeft = left[sample];
        const float originalRight = right[sample];
        left[sample] = originalLeft + (originalRight - originalLeft) * mix;
        right[sample] = originalRight + (originalLeft - originalRight) * mix;
    }
}

//==========================================================
// Pan
//==========================================================

// Aplica el paneo al bloque estéreo.
void GainProcessor::processPan(
    juce::AudioBuffer<float>& buffer)
{
    if (buffer.getNumChannels() < 2)
        return;

    const int numSamples =
        buffer.getNumSamples();

    auto* left =
        buffer.getWritePointer(0);

    auto* right =
        buffer.getWritePointer(1);

    if (!smoothedPan.isSmoothing())
    {
        const float currentPan = smoothedPan.getCurrentValue();
        if (currentPan == 0.0f)
            return;

        const float panAmount = std::abs(currentPan);
        const float curvedPan = std::pow(panAmount, 0.75f);
        const float leftGain = currentPan > 0.0f ? 1.0f - curvedPan : 1.0f;
        const float rightGain = currentPan < 0.0f ? 1.0f - curvedPan : 1.0f;

        juce::FloatVectorOperations::multiply(left, leftGain, numSamples);
        juce::FloatVectorOperations::multiply(right, rightGain, numSamples);
        return;
    }

    for (int sample = 0;
         sample < numSamples;
         ++sample)
    {
        const float currentPan =
            smoothedPan.getNextValue();

        const float panAmount =
            std::abs(currentPan);

        // Curva híbrida:
        // conserva unity en el centro,
        // pero hace más progresivo el movimiento.
        const float curvedPan =
            std::pow(panAmount, 0.75f);

        float leftGain = 1.0f;
        float rightGain = 1.0f;

        if (currentPan < 0.0f)
        {
            rightGain =
                1.0f - curvedPan;
        }
        else if (currentPan > 0.0f)
        {
            leftGain =
                1.0f - curvedPan;
        }

        left[sample] *= leftGain;
        right[sample] *= rightGain;
    }
}

//==========================================================
// Width
//==========================================================

void GainProcessor::processWidth(
    juce::AudioBuffer<float>& buffer)
{
    // WIDTH necesita como mínimo dos canales.
    if (buffer.getNumChannels() < 2)
        return;

    if (!smoothedWidth.isSmoothing()
        && smoothedWidth.getCurrentValue() == 1.0f)
        return;

    const int numSamples =
        buffer.getNumSamples();

    auto* left =
        buffer.getWritePointer(0);

    auto* right =
        buffer.getWritePointer(1);

    //==========================================================
    // Mid / Side
    //==========================================================

    for (int sample = 0;
         sample < numSamples;
         ++sample)
    {
        const float inputLeft =
            left[sample];

        const float inputRight =
            right[sample];

        // Centro de la señal.
        const float mid =
            (inputLeft + inputRight) * 0.5f;

        // Información estéreo.
        const float side =
            (inputLeft - inputRight) * 0.5f;

        //======================================================
        // Aplicar WIDTH
        //======================================================

        const float currentWidth =
    smoothedWidth.getNextValue();

        const float newSide =
            side * currentWidth;

        //======================================================
        // Mid / Side → Left / Right
        //======================================================

        left[sample] =
            mid + newSide;

        right[sample] =
            mid - newSide;
    }
}

//==========================================================
// Bass Mono
//==========================================================

void GainProcessor::processBassMono(
    juce::AudioBuffer<float>& buffer)
{
    if (buffer.getNumChannels() < 2)
        return;

    const int numSamples =
        buffer.getNumSamples();

    auto* left =
        buffer.getWritePointer(0);

    auto* right =
        buffer.getWritePointer(1);

    constexpr int filterUpdateInterval = 16;
    float currentCrossover =
        smoothedBassMonoCrossover.getCurrentValue();

    for (int chunkStart = 0;
         chunkStart < numSamples;
         chunkStart += filterUpdateInterval)
    {
        bassMonoFilterL.setCutoffFrequency(
            currentCrossover);

        bassMonoFilterR.setCutoffFrequency(
            currentCrossover);

        const int chunkEnd = juce::jmin(
            chunkStart + filterUpdateInterval,
            numSamples);

        for (int sample = chunkStart;
             sample < chunkEnd;
             ++sample)
        {
            const float inputLeft = left[sample];
            const float inputRight = right[sample];

            float lowLeft = 0.0f;
            float highLeft = 0.0f;
            bassMonoFilterL.processSample(0, inputLeft, lowLeft, highLeft);

            float lowRight = 0.0f;
            float highRight = 0.0f;
            bassMonoFilterR.processSample(0, inputRight, lowRight, highRight);

            const float monoLow = (lowLeft + lowRight) * 0.5f;
            const float bassMix = smoothedBassMonoMix.getNextValue();
            const float listenMix = smoothedBassMonoListenMix.getNextValue();

            const float enabledLeft = monoLow + highLeft;
            const float enabledRight = monoLow + highRight;
            const float normalLeft = inputLeft + (enabledLeft - inputLeft) * bassMix;
            const float normalRight = inputRight + (enabledRight - inputRight) * bassMix;

            const float previewLeft = lowLeft + (monoLow - lowLeft) * bassMix;
            const float previewRight = lowRight + (monoLow - lowRight) * bassMix;

            left[sample] = normalLeft + (previewLeft - normalLeft) * listenMix;
            right[sample] = normalRight + (previewRight - normalRight) * listenMix;
        }

        currentCrossover =
            smoothedBassMonoCrossover.skip(chunkEnd - chunkStart);
    }
}

void GainProcessor::setBassMonoListen(bool shouldListen)
{
    bassMonoListen = shouldListen;
    smoothedBassMonoListenMix.setTargetValue(shouldListen ? 1.0f : 0.0f);
}

bool GainProcessor::isBassMonoListen() const
{
    return bassMonoListen;
}
