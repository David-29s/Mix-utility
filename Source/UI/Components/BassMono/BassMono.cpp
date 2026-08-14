#include "BassMono.h"
#include "../../../Metrics/BassMono/BassMonoMetrics.h"
#include "../../../PluginProcessor.h"


BassMono::BassMono(
    AudioPluginAudioProcessor& processor)
    : processorRef(processor)
{
    //==========================================================
    // Enabled
    //==========================================================

    enabledButton.setButtonText("");

    enabledButton.setClickingTogglesState(true);

    enabledButton.setToggleState(
        false,
        juce::dontSendNotification);

    enabledButton.onClick = [this]
    {
        processorRef.setBassMonoParameter(
            enabledButton.getToggleState());

        repaint();
    };

    addAndMakeVisible(enabledButton);


    //==========================================================
    // Crossover Slider
    //==========================================================

    crossoverSlider.setSliderStyle(
        juce::Slider::LinearHorizontal);

    crossoverSlider.setTextBoxStyle(
        juce::Slider::NoTextBox,
        false,
        0,
        0);

    crossoverSlider.setRange(
        20.0,
        500.0,
        1.0);

    crossoverSlider.setValue(
        150.0,
        juce::dontSendNotification);

    crossoverSlider.setColour(
        juce::Slider::trackColourId,
        juce::Colour::fromRGB(0, 210, 240));

    crossoverSlider.setColour(
        juce::Slider::thumbColourId,
        juce::Colours::white);

    crossoverSlider.onValueChange = [this]
    {
        const auto frequency =
            static_cast<float>(
                crossoverSlider.getValue());

        processorRef.setBassMonoCrossoverParameter(
            frequency);

        crossoverLabel.setText(
            juce::String(
                frequency,
                0)
                + " Hz",
            juce::dontSendNotification);

        repaint();
    };

    addAndMakeVisible(crossoverSlider);


    //==========================================================
    // Crossover Value
    //==========================================================

    crossoverLabel.setText(
        "150 Hz",
        juce::dontSendNotification);

    crossoverLabel.setJustificationType(
        juce::Justification::centred);

    crossoverLabel.setColour(
        juce::Label::textColourId,
        juce::Colours::white);

    addAndMakeVisible(crossoverLabel);
}

void BassMono::paint(juce::Graphics& g)
{
    //==========================================================
    // Panel
    //==========================================================

    auto bounds = getLocalBounds().toFloat();

    g.setColour(
        juce::Colour::fromRGB(12, 20, 28));

    g.fillRoundedRectangle(
        bounds,
        12.0f);


    //==========================================================
    // Panel Border
    //==========================================================

    g.setColour(
        juce::Colour::fromRGB(0, 190, 220));

    g.drawRoundedRectangle(
        bounds.reduced(1.0f),
        12.0f,
        1.5f);


    //==========================================================
    // BASS MONO
    //==========================================================

    g.setColour(
        juce::Colour::fromRGB(0, 230, 255));

    g.setFont(
        16.0f);

    g.drawText(
        "BASS MONO",
        BassMonoMetrics::titleX,
        BassMonoMetrics::titleY,
        BassMonoMetrics::titleWidth,
        BassMonoMetrics::titleHeight,
        juce::Justification::centredLeft);

    //==========================================================
    // Listen Icon
    //==========================================================

    const float listenX =
       static_cast<float>(BassMonoMetrics::listenIconX);

    const float listenY =
        static_cast<float>(BassMonoMetrics::listenIconY);

    const float listenWidth =
        static_cast<float>(BassMonoMetrics::listenIconWidth);

    const float listenHeight =
        static_cast<float>(BassMonoMetrics::listenIconHeight);

    auto listenBounds =
        juce::Rectangle<float>(
            listenX,
            listenY,
            listenWidth,
            listenHeight);

    // Background
    g.setColour(
     listenEnabled
         ? juce::Colour::fromRGB(0, 230, 255).withAlpha(0.18f)
         : juce::Colour::fromRGB(35, 45, 50));

    g.fillRoundedRectangle(
        listenBounds,
        6.0f);

    // Border
    g.setColour(
     listenEnabled
         ? juce::Colour::fromRGB(0, 230, 255)
         : juce::Colour::fromRGB(80, 90, 95));

    g.drawRoundedRectangle(
        listenBounds,
        6.0f,
        1.2f);

    // Headphones
    g.setColour(
     listenEnabled
         ? juce::Colour::fromRGB(0, 230, 255)
         : juce::Colours::lightgrey);

    juce::Path headphones;

    headphones.startNewSubPath(
        listenX + 4.0f,
        listenY + 11.0f);

    headphones.cubicTo(
        listenX + 4.0f,
        listenY + 2.0f,
        listenX + 20.0f,
        listenY + 2.0f,
        listenX + 20.0f,
        listenY + 11.0f);

    g.strokePath(
        headphones,
        juce::PathStrokeType(1.0f));

    // Left ear cup
    g.drawRoundedRectangle(
        listenX + 3.0f,
        listenY + 10.0f,
        4.0f,
        7.0f,
        1.5f,
        1.5f);

    // Right ear cup
    g.drawRoundedRectangle(
        listenX + 17.0f,
        listenY + 10.0f,
        4.0f,
        7.0f,
        1.5f,
        1.5f);


    //==========================================================
    // ENABLED
    //==========================================================

    g.setColour(
        juce::Colour::fromRGB(0, 230, 255));

    g.setFont(
        12.0f);

    g.drawText(
        "ENABLED",
        BassMonoMetrics::enabledLabelX,
        BassMonoMetrics::enabledLabelY,
        BassMonoMetrics::enabledLabelWidth,
        BassMonoMetrics::enabledLabelHeight,
        juce::Justification::centred);


    //==========================================================
    // Switch
    //==========================================================

    const auto switchX =
        static_cast<float>(BassMonoMetrics::toggleX);

    const auto switchY =
        static_cast<float>(BassMonoMetrics::toggleY);

    const auto switchWidth =
        static_cast<float>(BassMonoMetrics::toggleWidth);

    const auto switchHeight =
        static_cast<float>(BassMonoMetrics::toggleHeight);

    const auto switchBounds =
        juce::Rectangle<float>(
            switchX,
            switchY,
            switchWidth,
            switchHeight);


    // Background
    g.setColour(
        juce::Colour::fromRGB(35, 45, 50));

    g.fillRoundedRectangle(
        switchBounds,
        switchHeight * 0.5f);


    // State
    if (enabledButton.getToggleState())
    {
        g.setColour(
            juce::Colour::fromRGB(0, 240, 120));

        g.fillRoundedRectangle(
            switchBounds.reduced(2.0f),
            (switchHeight - 4.0f) * 0.5f);

        // Knob
        g.setColour(
            juce::Colours::white);

        g.fillEllipse(
            switchBounds.getRight() - switchHeight + 3.0f,
            switchBounds.getY() + 3.0f,
            switchHeight - 6.0f,
            switchHeight - 6.0f);
    }
    else
    {
        g.setColour(
            juce::Colour::fromRGB(80, 90, 95));

        g.fillRoundedRectangle(
            switchBounds.reduced(2.0f),
            (switchHeight - 4.0f) * 0.5f);

        g.setColour(
            juce::Colours::lightgrey);

        g.fillEllipse(
            switchBounds.getX() + 3.0f,
            switchBounds.getY() + 3.0f,
            switchHeight - 6.0f,
            switchHeight - 6.0f);
    }


    //==========================================================
    // Bass Mono Icon
    //==========================================================

    const float iconCentreX = 35.0f;
    const float iconCentreY = 62.0f;
    const float iconRadius = 24.0f;

    g.setColour(
        juce::Colour::fromRGB(0, 230, 255));

    g.drawEllipse(
        iconCentreX - iconRadius,
        iconCentreY - iconRadius,
        iconRadius * 2.0f,
        iconRadius * 2.0f,
        2.0f);


    // Waveform
    juce::Path wave;

    wave.startNewSubPath(
        iconCentreX - 16.0f,
        iconCentreY);

    wave.cubicTo(
        iconCentreX - 11.0f,
        iconCentreY - 15.0f,
        iconCentreX - 7.0f,
        iconCentreY - 15.0f,
        iconCentreX - 3.0f,
        iconCentreY);

    wave.cubicTo(
        iconCentreX + 2.0f,
        iconCentreY + 15.0f,
        iconCentreX + 7.0f,
        iconCentreY + 15.0f,
        iconCentreX + 12.0f,
        iconCentreY);

    wave.cubicTo(
        iconCentreX + 15.0f,
        iconCentreY - 8.0f,
        iconCentreX + 17.0f,
        iconCentreY - 8.0f,
        iconCentreX + 18.0f,
        iconCentreY);

    g.strokePath(
        wave,
        juce::PathStrokeType(2.5f));


    //==========================================================
    // CROSSOVER
    //==========================================================

    g.setColour(
        juce::Colours::lightgrey);

    g.setFont(
        12.0f);

    g.drawText(
        "CROSSOVER",
        BassMonoMetrics::crossoverLabelX,
        BassMonoMetrics::crossoverLabelY,
        BassMonoMetrics::crossoverLabelWidth,
        BassMonoMetrics::crossoverLabelHeight,
        juce::Justification::centred);


    //==========================================================
    // Frequency Labels
    //==========================================================

    g.setFont(
        11.0f);

    g.drawText(
        "20 Hz",
        BassMonoMetrics::minLabelX,
        BassMonoMetrics::minLabelY,
        40,
        16,
        juce::Justification::centred);

    g.drawText(
        "500 Hz",
        BassMonoMetrics::maxLabelX,
        BassMonoMetrics::maxLabelY,
        45,
        16,
        juce::Justification::centred);
}

void BassMono::resized()
{
    enabledButton.setBounds(
        BassMonoMetrics::toggleX,
        BassMonoMetrics::toggleY,
        BassMonoMetrics::toggleWidth,
        BassMonoMetrics::toggleHeight);


    crossoverLabel.setBounds(
        BassMonoMetrics::crossoverValueX,
        BassMonoMetrics::crossoverValueY,
        BassMonoMetrics::crossoverValueWidth,
        BassMonoMetrics::crossoverValueHeight);


    crossoverSlider.setBounds(
        BassMonoMetrics::sliderX,
        BassMonoMetrics::sliderY,
        BassMonoMetrics::sliderWidth,
        BassMonoMetrics::sliderHeight);
}

void BassMono::mouseDown(const juce::MouseEvent& event)
{
    const auto listenBounds =
        juce::Rectangle<float>(
            static_cast<float>(BassMonoMetrics::listenIconX),
            static_cast<float>(BassMonoMetrics::listenIconY),
            static_cast<float>(BassMonoMetrics::listenIconWidth),
            static_cast<float>(BassMonoMetrics::listenIconHeight));

    if (listenBounds.contains(event.position))
    {
        listenEnabled = !listenEnabled;

        repaint();
    }
}