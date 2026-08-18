#include "BassMono.h"
#include "../../../Metrics/BassMono/BassMonoMetrics.h"
#include "../../../PluginProcessor.h"
#include "../../../Theme/NeonColours.h"


BassMono::BassMono(
    AudioPluginAudioProcessor& processor)
    : processorRef(processor)
{

    listenEnabled =
    processorRef.getBassMonoListenParameter();
    //==========================================================
    // Enabled
    //==========================================================

    enabledButton.setButtonText("");

    enabledButton.setColour(
        juce::ToggleButton::textColourId,
        juce::Colours::transparentBlack);

    enabledButton.setColour(
        juce::ToggleButton::tickColourId,
        juce::Colours::transparentBlack);

    enabledButton.setColour(
        juce::ToggleButton::tickDisabledColourId,
        juce::Colours::transparentBlack);

    enabledButton.setClickingTogglesState(true);

    enabledButton.setToggleState(
    processorRef.getBassMonoParameter(),
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
      processorRef.getBassMonoCrossoverParameter(),
      juce::dontSendNotification);

    crossoverSlider.setColour(
       juce::Slider::trackColourId,
       NeonColours::electricBlue);

    crossoverSlider.setColour(
        juce::Slider::thumbColourId,
        NeonColours::bassMonoInactive);

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
    juce::String(
        crossoverSlider.getValue(),
        0) + " Hz",
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
     NeonColours::bassMonoPanel);

    g.fillRoundedRectangle(
        bounds,
        BassMonoMetrics::panelCornerRadius);


    //==========================================================
    // Panel Border
    //==========================================================

    g.setColour(
    NeonColours::bassMonoBorder);

    g.drawRoundedRectangle(
        bounds.reduced(
            BassMonoMetrics::panelBorderInset),
        BassMonoMetrics::panelBorderCornerRadius,
        BassMonoMetrics::panelBorderThickness);


    //==========================================================
    // BASS MONO
    //==========================================================

    g.setColour(
      NeonColours::electricBlue);

    g.setFont(
        BassMonoMetrics::titleFontSize);

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
        static_cast<float>(
            BassMonoMetrics::listenIconX);

    const float listenY =
        static_cast<float>(
            BassMonoMetrics::listenIconY);

    const float listenWidth =
        static_cast<float>(
            BassMonoMetrics::listenIconWidth);

    const float listenHeight =
        static_cast<float>(
            BassMonoMetrics::listenIconHeight);

    auto listenBounds =
        juce::Rectangle<float>(
            listenX,
            listenY,
            listenWidth,
            listenHeight);


    // Background
    g.setColour(
    listenEnabled
        ? NeonColours::bassMonoListenGlow
        : NeonColours::bassMonoControl);

    // g.fillRoundedRectangle(
    //     listenBounds,
    //     BassMonoMetrics::listenCornerRadius);


    // Border
    // g.setColour(
    //  NeonColours::border);
    //
    // g.drawRoundedRectangle(
    //     listenBounds,
    //     BassMonoMetrics::listenCornerRadius,
    //     BassMonoMetrics::listenBorderThickness);


    // Headphones
    g.setColour(
      listenEnabled
          ? NeonColours::electricBlue
          : NeonColours::bassMonoInactive);


    juce::Path headphones;

    headphones.startNewSubPath(
        listenX +
            BassMonoMetrics::headphonesStartX,
        listenY +
            BassMonoMetrics::headphonesStartY);

    headphones.cubicTo(
        listenX +
            BassMonoMetrics::headphonesCurveTopX,
        listenY +
            BassMonoMetrics::headphonesCurveTopY,

        listenX +
            BassMonoMetrics::headphonesCurveControl1X,
        listenY +
            BassMonoMetrics::headphonesCurveControl1Y,

        listenX +
            BassMonoMetrics::headphonesCurveEndX,
        listenY +
            BassMonoMetrics::headphonesCurveEndY);

    g.strokePath(
        headphones,
        juce::PathStrokeType(
            BassMonoMetrics::headphonesStrokeWidth));


    // Left ear cup
    g.drawRoundedRectangle(
        listenX +
            BassMonoMetrics::leftEarX,
        listenY +
            BassMonoMetrics::leftEarY,
        BassMonoMetrics::leftEarWidth,
        BassMonoMetrics::leftEarHeight,
        BassMonoMetrics::leftEarCornerX,
        BassMonoMetrics::leftEarCornerY);


    // Right ear cup
    g.drawRoundedRectangle(
        listenX +
            BassMonoMetrics::rightEarX,
        listenY +
            BassMonoMetrics::rightEarY,
        BassMonoMetrics::rightEarWidth,
        BassMonoMetrics::rightEarHeight,
        BassMonoMetrics::rightEarCornerX,
        BassMonoMetrics::rightEarCornerY);


    //==========================================================
    // ENABLED
    //==========================================================

    g.setColour(
    NeonColours::electricBlue);

    g.setFont(
        BassMonoMetrics::enabledFontSize);

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
        static_cast<float>(
            BassMonoMetrics::toggleX);

    const auto switchY =
        static_cast<float>(
            BassMonoMetrics::toggleY);

    const auto switchWidth =
        static_cast<float>(
            BassMonoMetrics::toggleWidth);

    const auto switchHeight =
        static_cast<float>(
            BassMonoMetrics::toggleHeight);

    const auto switchBounds =
        juce::Rectangle<float>(
            switchX,
            switchY,
            switchWidth,
            switchHeight);


    // Background
    g.setColour(
     NeonColours::bassMonoControl);

    g.fillRoundedRectangle(
        switchBounds,
        switchHeight *
            BassMonoMetrics::toggleCornerRadius);


    // State
    if (enabledButton.getToggleState())
    {
        // Fondo del switch
        g.setColour(
            juce::Colour::fromRGB(35, 45, 50));

        g.fillRoundedRectangle(
            switchBounds,
            switchHeight *
                BassMonoMetrics::toggleCornerRadius);


        // Borde
        g.setColour(
                NeonColours::bassMonoBorderSoft);

        g.drawRoundedRectangle(
            switchBounds,
            switchHeight *
                BassMonoMetrics::toggleCornerRadius,
                BassMonoMetrics::toggleBorderThickness);


        // Círculo ON
        g.setColour(
                NeonColours::bassMonoActive);

        g.fillEllipse(
            switchBounds.getRight()
                - switchHeight
                + BassMonoMetrics::toggleCircleInset,

            switchBounds.getY()
                + BassMonoMetrics::toggleCircleInset,

            switchHeight
                - BassMonoMetrics::toggleCircleSizeReduction,

            switchHeight
                - BassMonoMetrics::toggleCircleSizeReduction);
    }
    else
    {
            g.setColour(
                NeonColours::bassMonoControlOff);

        g.fillRoundedRectangle(
            switchBounds.reduced(
                BassMonoMetrics::toggleInnerInset),
            (switchHeight
                - BassMonoMetrics::toggleInnerHeightReduction)
                * 0.5f);


        // Borde
        g.setColour(
            juce::Colours::white.withAlpha(0.3f));

        g.drawRoundedRectangle(
            switchBounds,
            switchHeight *
                BassMonoMetrics::toggleCornerRadius,
                BassMonoMetrics::toggleBorderThickness);


        // Círculo OFF
        g.setColour(
            NeonColours::bassMonoInactive);

        g.fillEllipse(
            switchBounds.getX()
                + BassMonoMetrics::toggleCircleInset,

            switchBounds.getY()
                + BassMonoMetrics::toggleCircleInset,

            switchHeight
                - BassMonoMetrics::toggleCircleSizeReduction,

            switchHeight
                - BassMonoMetrics::toggleCircleSizeReduction);
    }


    //==========================================================
    // Bass Mono Icon
    //==========================================================

    const float iconCentreX =
        BassMonoMetrics::iconCentreX;

    const float iconCentreY =
        BassMonoMetrics::iconCentreY;

    const float iconRadius =
        BassMonoMetrics::iconRadius;

    g.setColour(
            NeonColours::electricBlue);

    g.drawEllipse(
        iconCentreX - iconRadius,
        iconCentreY - iconRadius,
        iconRadius * 2.0f,
        iconRadius * 2.0f,
        BassMonoMetrics::iconStrokeWidth);


    //==========================================================
    // Waveform
    //==========================================================

    juce::Path wave;

    wave.startNewSubPath(
        iconCentreX +
            BassMonoMetrics::waveStartX,

        iconCentreY +
            BassMonoMetrics::waveStartY);


    wave.cubicTo(
        iconCentreX +
            BassMonoMetrics::waveControl1X,

        iconCentreY +
            BassMonoMetrics::waveControl1Y,

        iconCentreX +
            BassMonoMetrics::waveControl2X,

        iconCentreY +
            BassMonoMetrics::waveControl2Y,

        iconCentreX +
            BassMonoMetrics::waveEnd1X,

        iconCentreY +
            BassMonoMetrics::waveEnd1Y);


    wave.cubicTo(
        iconCentreX +
            BassMonoMetrics::waveControl3X,

        iconCentreY +
            BassMonoMetrics::waveControl3Y,

        iconCentreX +
            BassMonoMetrics::waveControl4X,

        iconCentreY +
            BassMonoMetrics::waveControl4Y,

        iconCentreX +
            BassMonoMetrics::waveEnd2X,

        iconCentreY +
            BassMonoMetrics::waveEnd2Y);


    wave.cubicTo(
        iconCentreX +
            BassMonoMetrics::waveControl5X,

        iconCentreY +
            BassMonoMetrics::waveControl5Y,

        iconCentreX +
            BassMonoMetrics::waveControl6X,

        iconCentreY +
            BassMonoMetrics::waveControl6Y,

        iconCentreX +
            BassMonoMetrics::waveEnd3X,

        iconCentreY +
            BassMonoMetrics::waveEnd3Y);


    g.strokePath(
        wave,
        juce::PathStrokeType(
            BassMonoMetrics::waveStrokeWidth));


    //==========================================================
    // CROSSOVER
    //==========================================================

    g.setColour(
        NeonColours::bassMonoText);

    g.setFont(
        BassMonoMetrics::crossoverFontSize);

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
        BassMonoMetrics::frequencyFontSize);

    g.drawText(
        "20 Hz",
        BassMonoMetrics::minLabelX,
        BassMonoMetrics::minLabelY,
        BassMonoMetrics::minLabelWidth,
        BassMonoMetrics::minLabelHeight,
        juce::Justification::centred);

    g.drawText(
        "500 Hz",
        BassMonoMetrics::maxLabelX,
        BassMonoMetrics::maxLabelY,
        BassMonoMetrics::maxLabelWidth,
        BassMonoMetrics::maxLabelHeight,
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

        processorRef.setBassMonoListen(listenEnabled);

        repaint();
    }
}

void BassMono::mouseMove(const juce::MouseEvent& event)
{
    const auto listenBounds =
        juce::Rectangle<float>(
            static_cast<float>(
                BassMonoMetrics::listenIconX),
            static_cast<float>(
                BassMonoMetrics::listenIconY),
            static_cast<float>(
                BassMonoMetrics::listenIconWidth),
            static_cast<float>(
                BassMonoMetrics::listenIconHeight));

    if (listenBounds.contains(event.position))
    {
        setMouseCursor(
            juce::MouseCursor::PointingHandCursor);
    }
    else
    {
        setMouseCursor(
            juce::MouseCursor::NormalCursor);
    }
}


void BassMono::mouseExit(const juce::MouseEvent&)
{
    setMouseCursor(
        juce::MouseCursor::NormalCursor);
}