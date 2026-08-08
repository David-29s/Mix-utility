#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Graphics/Frame/PluginFrame.h"
#include "Metrics/Layout/LayoutMetrics.h"

AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor
(AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p),
      processorRef(p)
{
    addAndMakeVisible(gainKnob);


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

    //==========================================================
    // Control Mode Button
    //==========================================================

    addAndMakeVisible(
        controlModeButton);

    controlModeButton.setMode(
        ControlMode::gain);

    controlModeButton.onModeChanged =
[this](ControlMode mode)
{
    switch (mode)
    {
    case ControlMode::gain:

        gainKnob.setTitle("GAIN");
        gainKnob.setUnit("dB");

        gainKnob.getSlider().setRange(
            -24.0,
            24.0,
            0.01);

        break;


    case ControlMode::pan:

        gainKnob.setTitle("PAN");
        gainKnob.setUnit("%");

        gainKnob.getSlider().setRange(
            -100.0,
            100.0,
            0.01);

        break;


    case ControlMode::width:

        gainKnob.setTitle("WIDTH");
        gainKnob.setUnit("%");

        gainKnob.getSlider().setRange(
            0.0,
            200.0,
            0.01);

        break;
    }

    updateKnobAttachment(mode);
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

}
void AudioPluginAudioProcessorEditor::resized()
{
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
        LayoutMetrics::buttonSize,
        LayoutMetrics::buttonSize);

}

void AudioPluginAudioProcessorEditor::timerCallback()
{
    animator.update();

    refreshButtons();

    repaint();
}

void AudioPluginAudioProcessorEditor::setButtonPosition(
    IconToggleButton& button,
    int index)
{
    button.setBounds(
        LayoutMetrics::buttonStartX +
        LayoutMetrics::buttonSpacing * index,

        LayoutMetrics::buttonY,

        LayoutMetrics::buttonSize,

        LayoutMetrics::buttonSize);
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
