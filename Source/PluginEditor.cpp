#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Graphics/Frame/PluginFrame.h"
#include "Metrics/Layout/LayoutMetrics.h"
#include "Metrics/Meter/MeterMetrics.h"
#include "Metrics/BassMono/BassMonoMetrics.h"
#include "Metrics/CorrelationMetrics/CorrelationMetrics.h"

AutoGainButton::AutoGainButton()
    : juce::Button("Auto Gain")
{
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
}

void AutoGainButton::setDisplayState(DisplayState newState)
{
    if (displayState == newState)
        return;

    displayState = newState;
    repaint();
}

void AutoGainButton::paintButton(
    juce::Graphics& g,
    bool isMouseOverButton,
    bool isButtonDown)
{
    const auto bounds = getLocalBounds().toFloat();
    const float diameter = juce::jmin(bounds.getWidth(), bounds.getHeight());
    const float radius = diameter * 0.36f;
    const juce::Point<float> centre = bounds.getCentre();

    juce::Colour accent = NeonColours::cyan;
    if (displayState == DisplayState::listening)
        accent = juce::Colour(0xffffbd3f);
    else if (displayState == DisplayState::applying)
        accent = juce::Colours::white;
    else if (displayState == DisplayState::noSignal)
        accent = juce::Colour(0xffff5b68);

    if (isMouseOverButton || isButtonDown)
        g.setColour(accent.withAlpha(isButtonDown ? 0.18f : 0.09f));
    else
        g.setColour(juce::Colours::transparentBlack);

    g.fillEllipse(centre.x - radius, centre.y - radius,
                  radius * 2.0f, radius * 2.0f);

    g.setColour(accent.withAlpha(isEnabled() ? 1.0f : 0.45f));
    g.drawEllipse(centre.x - radius, centre.y - radius,
                  radius * 2.0f, radius * 2.0f, 1.7f);

    const float sideY = centre.y;
    const float sideStart = radius + diameter * 0.09f;
    const float sideEnd = sideStart + diameter * 0.13f;
    g.drawLine(centre.x - sideEnd, sideY, centre.x - sideStart, sideY, 1.4f);
    g.drawLine(centre.x + sideStart, sideY, centre.x + sideEnd, sideY, 1.4f);

    g.setFont(juce::Font(diameter * 0.42f, juce::Font::bold));
    g.drawText("A", getLocalBounds(), juce::Justification::centred, false);
}

StereoSwapButton::StereoSwapButton()
    : juce::TextButton("SWAP")
{
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
}

void StereoSwapButton::paintButton(
    juce::Graphics& g,
    bool isMouseOverButton,
    bool isButtonDown)
{
    const auto bounds = getLocalBounds().toFloat().reduced(1.0f);
    const bool active = getToggleState();
    const auto accent = active ? NeonColours::orange : NeonColours::grey;

    g.setColour(juce::Colour(0xff151a22));
    g.fillRoundedRectangle(bounds, 5.0f);

    g.setColour(accent.withAlpha(isButtonDown ? 1.0f : (isMouseOverButton ? 0.9f : 0.7f)));
    g.drawRoundedRectangle(bounds, 5.0f, 1.2f);

    g.setColour(active ? NeonColours::orange : NeonColours::grey);
    g.setFont(juce::Font(11.0f, juce::Font::bold));
    g.drawText(getButtonText(), getLocalBounds(), juce::Justification::centred, false);
}

AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor
(AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p),
      processorRef(p), bassMono(p)
{

    addAndMakeVisible(inputMeter);
    addAndMakeVisible(outputMeter);

    inputMeter.setLabel("INPUT");
    outputMeter.setLabel("OUTPUT");
    inputMeter.setTooltip("Input level. Click the readout to reset peak hold.");
    outputMeter.setTooltip("Output level. Click the readout to reset peak hold.");

    inputMeter.setScaleOnRight(false);
    outputMeter.setScaleOnRight(true);

    addAndMakeVisible(correlationMeter);
    correlationMeter.setTooltip("Stereo correlation: +1 in phase, -1 out of phase.");

    addAndMakeVisible(bassMono);

    addAndMakeVisible(gainKnob);
    gainKnob.setTooltip("Gain: -100 to +36 dB. Double-click to edit.");


    gainAttachment =
        std::make_unique<
            juce::AudioProcessorValueTreeState::SliderAttachment>(
                processorRef.getAPVTS(),
                "GAIN",
                gainKnob.getSlider());

    gainKnob.setMode(NeonLookAndFeel::KnobMode::gain);

    addAndMakeVisible(outputTrimTitle);
    outputTrimTitle.setText("OUTPUT TRIM", juce::dontSendNotification);
    outputTrimTitle.setJustificationType(juce::Justification::centredLeft);
    outputTrimTitle.setFont(juce::Font(10.0f, juce::Font::bold));
    outputTrimTitle.setColour(juce::Label::textColourId, NeonColours::orange);
    outputTrimTitle.setTooltip("Final level after processing.");

    addAndMakeVisible(outputTrimSlider);
    outputTrimSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    outputTrimSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    outputTrimSlider.setColour(juce::Slider::trackColourId,
                               NeonColours::orange.withAlpha(0.95f));
    outputTrimSlider.setColour(juce::Slider::backgroundColourId,
                               juce::Colour(0xff48515d));
    outputTrimSlider.setColour(juce::Slider::thumbColourId,
                               NeonColours::orange);
    outputTrimSlider.setTooltip("Output trim: -36 to +12 dB.");

    addAndMakeVisible(outputTrimValue);
    outputTrimValue.setJustificationType(juce::Justification::centred);
    outputTrimValue.setFont(juce::Font(11.0f, juce::Font::bold));
    outputTrimValue.setColour(juce::Label::textColourId, juce::Colours::white);

    outputTrimSlider.onValueChange = [this]
    {
        const double value = outputTrimSlider.getValue();
        const auto text = (value > 0.0 ? "+" : "")
            + juce::String(value, 1) + " dB";
        outputTrimValue.setText(text, juce::dontSendNotification);
    };

    outputTrimAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processorRef.getAPVTS(),
            "OUTPUT_TRIM",
            outputTrimSlider);
    outputTrimSlider.onValueChange();

    addAndMakeVisible(stereoSwapButton);
    stereoSwapButton.setClickingTogglesState(true);
    stereoSwapButton.setTooltip("Swap left and right channels.");
    stereoSwapButton.onClick = [this]
    {
        processorRef.setStereoSwapParameter(stereoSwapButton.getToggleState());
    };
    stereoSwapButton.setToggleState(
        processorRef.getStereoSwapParameter(),
        juce::dontSendNotification);

    addAndMakeVisible(autoGainButton);
    autoGainButton.setTooltip(
        "Analyze 1 second and set Gain to -18 dBFS RMS. Click again to cancel.");
    autoGainButton.onClick = [this]
    {
        if (autoGainAnimationFrames > 0)
            return;

        if (autoGainCapturing)
        {
            autoGainCapturing = false;
            processorRef.cancelAutoGainCapture();
            autoGainButton.setDisplayState(AutoGainButton::DisplayState::idle);
            return;
        }

        autoGainCapturing = true;
        autoGainAnimationFrames = 0;
        autoGainFeedbackFrames = 0;
        autoGainWaitFrames = 180;
        processorRef.requestAutoGainCapture();
        autoGainButton.setDisplayState(AutoGainButton::DisplayState::listening);
    };

    //==========================================================
    // Bypass Button
    //==========================================================

    //==========================================================
    // Buttons
    //==========================================================

    addAndMakeVisible(
        bypassButton);

    bypassButton.setIconType(
        IconType::bypass);

    bypassButton.setLabel(
        "BYPASS");

    bypassButton.setColour(
        NeonColours::neonGreen);
    bypassButton.setTooltip("Pass audio without processing.");



    //==========================================================
    // Events
    //==========================================================

    bypassButton.onClick = [this]
    {
        processorRef.setBypassParameter(
            bypassButton.isActive());
    };



    //----------------------------------------------------------

    addAndMakeVisible(
        monoButton);

    monoButton.setIconType(
        IconType::mono);

    monoButton.setLabel(
        "MONO");

    monoButton.setColour(
        NeonColours::magenta);
    monoButton.setTooltip("Sum left and right to mono.");



    //==========================================================
    // Events
    //==========================================================

    monoButton.onClick = [this]
    {
        processorRef.setMonoParameter(
            monoButton.isActive());

        refreshButtons();
    };
    //----------------------------------------------------------

    addAndMakeVisible(
        phaseButton);

    phaseButton.setIconType(
        IconType::phase);

    phaseButton.setLabel(
        "PHASE");

    phaseButton.setColour(
        NeonColours::orange);
    phaseButton.setTooltip("Invert signal polarity.");



    //==========================================================
    // Events
    //==========================================================

    phaseButton.onClick = [this]
    {
        processorRef.setPhaseParameter(
     phaseButton.isActive());
    };

    //----------------------------------------------------------

    addAndMakeVisible(
        muteButton);

    muteButton.setIconType(
        IconType::mute);

    muteButton.setLabel(
        "MUTE");

    muteButton.setColour(
        NeonColours::azul);
    muteButton.setTooltip("Mute the output.");



    //==========================================================
    // Events
    //==========================================================

    muteButton.onClick = [this]
    {
        processorRef.setMuteParameter(
      muteButton.isActive());
    };

    //----------------------------------------------------------

    addAndMakeVisible(
        abButton);

    abButton.setIconType(
        IconType::ab);

    abButton.setLabel(
        "A/B");

    abButton.setColour(
        NeonColours::violet);
    abButton.setTooltip("Switch between A and B settings.");

    //==========================================================
    // Control Mode Button
    //==========================================================

    addAndMakeVisible(
        controlModeButton);
    controlModeButton.setTooltip("Select Gain, Pan, or Width.");

    controlModeButton.setMode(
        ControlMode::gain);

    controlModeButton.onModeChanged =
[this](ControlMode mode)
{
    switch (mode)
    {
    case ControlMode::gain:

        autoGainButton.setVisible(true);
        autoGainButton.setEnabled(true);

        gainKnob.setTitle("GAIN");
        gainKnob.setTooltip("Gain: -100 to +36 dB. Double-click to edit.");

        gainKnob.getLookAndFeel().setKnobMode(
            NeonLookAndFeel::KnobMode::gain);

        gainKnob.getSlider().setRange(
           -100.0,
             36.0,
            0.01);
        gainKnob.getSlider().setSkewFactorFromMidPoint(0.0);
        gainKnob.setMode(NeonLookAndFeel::KnobMode::gain);

        break;


    case ControlMode::pan:

        autoGainButton.setVisible(false);
        processorRef.cancelAutoGainCapture();
        autoGainCapturing = false;
        autoGainWaitFrames = 0;
        autoGainAnimationFrames = 0;
        if (autoGainGestureActive)
        {
            gainKnob.getSlider().setValue(
                autoGainTargetDb,
                juce::sendNotificationSync);
            processorRef.endGainParameterGesture();
            autoGainGestureActive = false;
        }
        autoGainButton.setDisplayState(AutoGainButton::DisplayState::idle);

        gainKnob.setTitle("PAN");
        gainKnob.setTooltip("Pan between left and right. Double-click the value to edit.");

        gainKnob.getLookAndFeel().setKnobMode(
            NeonLookAndFeel::KnobMode::pan);

        gainKnob.getSlider().setRange(
            -100.0,
            100.0,
            0.01);
        gainKnob.setMode(NeonLookAndFeel::KnobMode::pan);

        break;


    case ControlMode::width:

        autoGainButton.setVisible(false);
        processorRef.cancelAutoGainCapture();
        autoGainCapturing = false;
        autoGainWaitFrames = 0;
        autoGainAnimationFrames = 0;
        if (autoGainGestureActive)
        {
            gainKnob.getSlider().setValue(
                autoGainTargetDb,
                juce::sendNotificationSync);
            processorRef.endGainParameterGesture();
            autoGainGestureActive = false;
        }
        autoGainButton.setDisplayState(AutoGainButton::DisplayState::idle);

        gainKnob.setTitle("WIDTH");
        gainKnob.setTooltip("Width: 0% is mono; 100% is original. Double-click to edit.");

        gainKnob.getLookAndFeel().setKnobMode(
            NeonLookAndFeel::KnobMode::width);

        gainKnob.getSlider().setRange(
            0.0,
            400.0,
            0.01);
        gainKnob.setMode(NeonLookAndFeel::KnobMode::width);

        break;
    }

    updateKnobAttachment(mode);

    gainKnob.getSlider().repaint();
};




    //==========================================================
    // Events
    //==========================================================

    abButton.onClick = [this]
    {
        processorRef.toggleCompareState();

        refreshButtons();
    };


    startTimerHz(60);

    setSize(
    LayoutMetrics::pluginWidth,
    LayoutMetrics::pluginHeight);

    refreshButtons();
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    processorRef.cancelAutoGainCapture();
    if (autoGainGestureActive)
    {
        gainKnob.getSlider().setValue(
            autoGainTargetDb,
            juce::sendNotificationSync);
        processorRef.endGainParameterGesture();
    }
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));


    PluginFrame::draw(
      g,
      getLocalBounds(),
      animator);

    const int totalButtonWidth =
        (LayoutMetrics::buttonWidth * 5)
        + (LayoutMetrics::buttonSpacing * 4);
    const int buttonStartX = (getWidth() - totalButtonWidth) / 2;
    const float separatorTop = static_cast<float>(LayoutMetrics::buttonY + 8);
    const float separatorBottom = static_cast<float>(
        LayoutMetrics::buttonY + LayoutMetrics::buttonHeight - 8);

    g.setColour(NeonColours::border.withAlpha(0.9f));
    for (const int groupBoundary : { 0, 3 })
    {
        const float separatorX = static_cast<float>(
            buttonStartX
            + ((groupBoundary + 1) * LayoutMetrics::buttonWidth)
            + (groupBoundary * LayoutMetrics::buttonSpacing)
            + (LayoutMetrics::buttonSpacing * 0.5f));
        g.drawLine(separatorX, separatorTop, separatorX, separatorBottom, 1.0f);
    }

    const auto trimPanel = juce::Rectangle<float>(
        static_cast<float>(LayoutMetrics::outputTrimPanelX),
        static_cast<float>(LayoutMetrics::outputTrimPanelY),
        static_cast<float>(LayoutMetrics::outputTrimPanelWidth),
        static_cast<float>(LayoutMetrics::outputTrimPanelHeight));
    g.setColour(juce::Colour(0xff151a22));
    g.fillRoundedRectangle(trimPanel, 6.0f);
    g.setColour(NeonColours::orange.withAlpha(0.5f));
    g.drawRoundedRectangle(trimPanel, 6.0f, 1.0f);

}
void AudioPluginAudioProcessorEditor::resized()
{

    //==========================================================
    // Input Meter
    //==========================================================

    inputMeter.setBounds(
      MeterMetrics::inputMeterX,
      MeterMetrics::inputMeterY,
      MeterMetrics::componentWidth,
      MeterMetrics::componentHeight);


    outputMeter.setBounds(
     MeterMetrics::outputMeterX,
     MeterMetrics::outputMeterY,
     MeterMetrics::componentWidth,
     MeterMetrics::componentHeight);

    //==========================================================
    // Correlation Meter
    //==========================================================

    correlationMeter.setBounds(
    CorrelationMetrics::x,
    CorrelationMetrics::y,
    CorrelationMetrics::width,
    CorrelationMetrics::height);

    //==========================================================
    // Bass Mono
    //==========================================================

    bassMono.setBounds(
        BassMonoMetrics::panelX,
        BassMonoMetrics::panelY,
        BassMonoMetrics::panelWidth,
        BassMonoMetrics::panelHeight);

    //==========================================================
    // Knob
    //==========================================================

    auto knobBounds =
       getLocalBounds()
           .withSizeKeepingCentre(
               LayoutMetrics::knobWidth,
               LayoutMetrics::knobHeight);

    knobBounds.translate(
    LayoutMetrics::knobOffsetX,
    LayoutMetrics::knobOffsetY);

    gainKnob.setBounds(
    knobBounds);

    const int trimY = LayoutMetrics::outputTrimPanelY;
    outputTrimTitle.setBounds(105, trimY + 1, 74, 32);
    outputTrimSlider.setBounds(179, trimY + 8, 132, 18);
    outputTrimValue.setBounds(311, trimY + 1, 60, 32);
    stereoSwapButton.setBounds(377, trimY + 3, 57, 28);
    // Mirror the Gain mode control across the knob's centerline.
    // Its 40 px bounds end before the output meter begins at x=441.
    autoGainButton.setBounds(396, 50, 40, 40);


    //==========================================================
    // Buttons icons
    //==========================================================

    setButtonPosition(
       bypassButton,
       0);

    setButtonPosition(
        monoButton,
        1);

    setButtonPosition(
        phaseButton,
        2);

    setButtonPosition(
        muteButton,
        3);

    setButtonPosition(
        abButton,
        4);

    //==========================================================
    // Control Mode Button
    //==========================================================

    controlModeButton.setBounds(
        LayoutMetrics::controlModeButtonX,
        LayoutMetrics::controlModeButtonY,
        LayoutMetrics::controlModeButtonSize,
        LayoutMetrics::controlModeButtonSize);

}

void AudioPluginAudioProcessorEditor::timerCallback()
{
    animator.update();

    const int autoGainState = processorRef.getAutoGainCaptureState();
    if (autoGainCapturing && autoGainState == 2)
    {
        autoGainCapturing = false;
        autoGainWaitFrames = 0;
        autoGainButton.setDisplayState(AutoGainButton::DisplayState::applying);
        autoGainButton.setEnabled(false);
        autoGainStartDb = static_cast<float>(gainKnob.getSlider().getValue());
        autoGainTargetDb = processorRef.getAutoGainResultDb();
        autoGainAnimationFrames = 1;
        autoGainGestureActive = true;
        processorRef.beginGainParameterGesture();
    }
    else if (autoGainCapturing && autoGainState == 3)
    {
        autoGainCapturing = false;
        autoGainWaitFrames = 0;
        autoGainButton.setDisplayState(AutoGainButton::DisplayState::noSignal);
        autoGainFeedbackFrames = 90;
    }
    else if (autoGainCapturing && --autoGainWaitFrames <= 0)
    {
        autoGainCapturing = false;
        processorRef.cancelAutoGainCapture();
        autoGainButton.setDisplayState(AutoGainButton::DisplayState::noSignal);
        autoGainFeedbackFrames = 90;
    }

    if (autoGainAnimationFrames > 0)
    {
        constexpr int animationFrameCount = 36;
        const float progress = juce::jmin(
            1.0f,
            static_cast<float>(autoGainAnimationFrames)
                / static_cast<float>(animationFrameCount));
        const float easedProgress = progress * progress * (3.0f - 2.0f * progress);
        const float animatedDb = autoGainStartDb
            + (autoGainTargetDb - autoGainStartDb) * easedProgress;

        gainKnob.getSlider().setValue(
            animatedDb,
            juce::sendNotificationSync);

        if (autoGainAnimationFrames >= animationFrameCount)
        {
            autoGainAnimationFrames = 0;
            if (autoGainGestureActive)
            {
                processorRef.endGainParameterGesture();
                autoGainGestureActive = false;
            }
            processorRef.cancelAutoGainCapture();
            autoGainButton.setDisplayState(AutoGainButton::DisplayState::idle);
            autoGainButton.setEnabled(true);
        }
        else
            ++autoGainAnimationFrames;
    }

    if (autoGainFeedbackFrames > 0 && --autoGainFeedbackFrames == 0)
    {
        processorRef.cancelAutoGainCapture();
        autoGainButton.setDisplayState(AutoGainButton::DisplayState::idle);
    }

    refreshButtons();

    bassMono.refreshState();

    inputMeter.setLevels(
     processorRef.getInputLevelL(),
     processorRef.getInputLevelR(),
     processorRef.getInputRmsL(),
     processorRef.getInputRmsR());

    outputMeter.setLevels(
        processorRef.getOutputLevelL(),
        processorRef.getOutputLevelR(),
        processorRef.getOutputRmsL(),
        processorRef.getOutputRmsR());

    correlationMeter.setCorrelation(
    processorRef.getOutputCorrelation());

    repaint();
}

void AudioPluginAudioProcessorEditor::setButtonPosition(
   IconToggleButton& button,
    int index)
{
    const int buttonWidth =
        LayoutMetrics::buttonWidth;

    const int totalWidth =
        (buttonWidth * 5) +
        (LayoutMetrics::buttonSpacing * 4);

    const int startX =
        (getWidth() - totalWidth) / 2;

    const int buttonY =
     LayoutMetrics::buttonY;

    button.setBounds(
        startX +
            (buttonWidth + LayoutMetrics::buttonSpacing) * index,
        buttonY,
        buttonWidth,
        LayoutMetrics::buttonHeight);
}

void AudioPluginAudioProcessorEditor::refreshButtons()
{

    abButton.setStateA(
    processorRef.isUsingStateA());

    stereoSwapButton.setToggleState(
        processorRef.getStereoSwapParameter(),
        juce::dontSendNotification);

    bypassButton.setActive(
        processorRef.getBypassParameter());

    monoButton.setActive(
        processorRef.getMonoParameter());

    if (controlModeButton.getMode() == ControlMode::width
        && gainKnob.getSlider().isEnabled()
            == processorRef.getMonoParameter())
    {
        updateKnobAttachment(ControlMode::width);
    }

    phaseButton.setActive(
        processorRef.getPhaseParameter());

    muteButton.setActive(
        processorRef.getMuteParameter());
}

//==========================================================
// Update Knob Attachment
//==========================================================

void AudioPluginAudioProcessorEditor::updateKnobAttachment(
    ControlMode mode)
{
    // Desconectamos el attachment actual.
    gainAttachment.reset();
    panAttachment.reset();
    widthAttachment.reset();
    gainKnob.getSlider().setEnabled(true);


    //==========================================================
    // GAIN
    //==========================================================

    if (mode == ControlMode::gain)
    {
        gainAttachment =
            std::make_unique<
                juce::AudioProcessorValueTreeState::SliderAttachment>(
                    processorRef.getAPVTS(),
                    "GAIN",
                    gainKnob.getSlider());
    }


    //==========================================================
    // PAN
    //==========================================================

    else if (mode == ControlMode::pan)
    {
        panAttachment =
            std::make_unique<
                juce::AudioProcessorValueTreeState::SliderAttachment>(
                    processorRef.getAPVTS(),
                    "PAN",
                    gainKnob.getSlider());
    }


    //==========================================================
    // WIDTH
    //==========================================================

    else if (mode == ControlMode::width)
    {
        if (processorRef.getMonoParameter())
        {
            // Keep the saved WIDTH parameter untouched while Mono makes its
            // value inaudible; reattaching restores the previous width later.
            gainKnob.getSlider().setValue(0.0, juce::dontSendNotification);
            gainKnob.setMode(NeonLookAndFeel::KnobMode::width);
            gainKnob.getSlider().setEnabled(false);
            gainKnob.setTooltip(
                "Mono locks Width at 0%. Your setting returns when Mono is off.");
        }
        else
        {
            widthAttachment =
                std::make_unique<
                    juce::AudioProcessorValueTreeState::SliderAttachment>(
                        processorRef.getAPVTS(),
                        "WIDTH",
                        gainKnob.getSlider());
            gainKnob.setTooltip(
                "Width: 0% is mono; 100% is original. Double-click to edit.");
        }
    }
}
