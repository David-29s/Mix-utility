#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Graphics/Frame/PluginFrame.h"
#include "Metrics/Layout/LayoutMetrics.h"
#include "Metrics/Meter/MeterMetrics.h"
#include "Metrics/BassMono/BassMonoMetrics.h"
#include "Metrics/CorrelationMetrics/CorrelationMetrics.h"

AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor
(AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p),
      processorRef(p), bassMono(p)
{

    addAndMakeVisible(inputMeter);
    addAndMakeVisible(outputMeter);

    inputMeter.setLabel("INPUT");
    outputMeter.setLabel("OUTPUT");
    inputMeter.setTooltip("Real-time input level. Click the top readout to reset the peak hold.");
    outputMeter.setTooltip("Real-time output level. Click the top readout to reset the peak hold.");

    inputMeter.setScaleOnRight(false);
    outputMeter.setScaleOnRight(true);

    addAndMakeVisible(correlationMeter);
    correlationMeter.setTooltip("Stereo output correlation: +1 is in phase, while -1 is out of phase.");

    addAndMakeVisible(bassMono);

    addAndMakeVisible(gainKnob);
    gainKnob.setTooltip("Input gain: -100 dB (digital silence) to +36 dB. Double-click the readout to enter a value.");


    gainAttachment =
        std::make_unique<
            juce::AudioProcessorValueTreeState::SliderAttachment>(
                processorRef.getAPVTS(),
                "GAIN",
                gainKnob.getSlider());

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
    bypassButton.setTooltip("Enable or disable plugin processing bypass.");



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
    monoButton.setTooltip("Sum the stereo signal to mono.");



    //==========================================================
    // Events
    //==========================================================

    monoButton.onClick = [this]
    {
        processorRef.setMonoParameter(
            monoButton.isActive());
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
    phaseButton.setTooltip("Invert the signal phase.");



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
    muteButton.setTooltip("Mute the plugin output.");



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
    abButton.setTooltip("Switch between states A and B to compare settings.");

    //==========================================================
    // Control Mode Button
    //==========================================================

    addAndMakeVisible(
        controlModeButton);
    controlModeButton.setTooltip("Switch the main control between Gain, Pan, and Width.");

    controlModeButton.setMode(
        ControlMode::gain);

    controlModeButton.onModeChanged =
[this](ControlMode mode)
{
    switch (mode)
    {
    case ControlMode::gain:

        gainKnob.setTitle("GAIN");
        gainKnob.setTooltip("Input gain: -100 dB (digital silence) to +36 dB. Double-click the readout to enter a value.");

        gainKnob.getLookAndFeel().setKnobMode(
            NeonLookAndFeel::KnobMode::gain);

        gainKnob.getSlider().setRange(
           -100.0,
             36.0,
            0.01);
        gainKnob.getSlider().setSkewFactorFromMidPoint(0.0);

        break;


    case ControlMode::pan:

        gainKnob.setTitle("PAN");
        gainKnob.setTooltip("Stereo pan: move the signal between left (L) and right (R). Double-click the readout to enter a value.");

        gainKnob.getLookAndFeel().setKnobMode(
            NeonLookAndFeel::KnobMode::pan);

        gainKnob.getSlider().setRange(
            -100.0,
            100.0,
            0.01);

        break;


    case ControlMode::width:

        gainKnob.setTitle("WIDTH");
        gainKnob.setTooltip("Stereo width: 0% is mono and 100% keeps the original width. Double-click the readout to enter a value.");

        gainKnob.getLookAndFeel().setKnobMode(
            NeonLookAndFeel::KnobMode::width);

        gainKnob.getSlider().setRange(
            0.0,
            400.0,
            0.01);

        break;
    }

    updateKnobAttachment(mode);

    switch (mode)
    {
    case ControlMode::gain:
        gainKnob.setUnit("dB");
        break;

    case ControlMode::pan:
        gainKnob.setUnit("%");
        break;

    case ControlMode::width:
        gainKnob.setUnit("%");
        break;
    }

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

    const int knobTop =
        (getHeight() - LayoutMetrics::knobHeight) / 2;

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

    bypassButton.setActive(
        processorRef.getBypassParameter());

    monoButton.setActive(
        processorRef.getMonoParameter());

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
        widthAttachment =
            std::make_unique<
                juce::AudioProcessorValueTreeState::SliderAttachment>(
                    processorRef.getAPVTS(),
                    "WIDTH",
                    gainKnob.getSlider());
    }
}
