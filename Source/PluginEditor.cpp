#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Graphics/Frame/PluginFrame.h"


//==============================================================================
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
        NeonColours::cyan);

    bypassButton.setActive(
        false);

    //----------------------------------------------------------

    addAndMakeVisible(
        monoButton);

    monoButton.setIconType(
        IconType::mono);

    monoButton.setLabel(
        "MONO");

    monoButton.setColour(
        NeonColours::magenta);

    monoButton.setActive(
        false);

    //----------------------------------------------------------

    addAndMakeVisible(
        phaseButton);

    phaseButton.setIconType(
        IconType::phase);

    phaseButton.setLabel(
        "PHASE");

    phaseButton.setColour(
        NeonColours::orange);

    phaseButton.setActive(
        false);

    //----------------------------------------------------------

    addAndMakeVisible(
        muteButton);

    muteButton.setIconType(
        IconType::mute);

    muteButton.setLabel(
        "MUTE");

    muteButton.setColour(
        NeonColours::green);

    muteButton.setActive(
        false);

    //----------------------------------------------------------

    addAndMakeVisible(
        abButton);

    abButton.setIconType(
        IconType::ab);

    abButton.setLabel(
        "A/B");

    abButton.setColour(
        NeonColours::violet);

    abButton.setActive(
        false);
    startTimerHz(60);

    setSize(300, 320);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("", getLocalBounds(), juce::Justification::centred, 1);

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

    constexpr int knobWidth = 280;
    constexpr int knobHeight = 360;

    //----------------------------------------------------------
    // Position
    //----------------------------------------------------------

    constexpr int knobOffsetX = 0;
    constexpr int knobOffsetY = -15;

    //----------------------------------------------------------
    // Bounds
    //----------------------------------------------------------

    auto knobBounds =
        getLocalBounds()
            .withSizeKeepingCentre(
                knobWidth,
                knobHeight);

    knobBounds.translate(
        knobOffsetX,
        knobOffsetY);

    gainKnob.setBounds(
        knobBounds);

    //==========================================================
    // Buttons iconos
    //==========================================================

    constexpr int buttonSize = 80;

    constexpr int spacing = 50;

    constexpr int startX = 14;

    constexpr int y = 230;

    bypassButton.setBounds(
    startX,
    y,
    buttonSize,
    buttonSize);

    monoButton.setBounds(
        startX + spacing,
        y,
        buttonSize,
        buttonSize);

    phaseButton.setBounds(
        startX + spacing * 2,
        y,
        buttonSize,
        buttonSize);

    muteButton.setBounds(
        startX + spacing * 3,
        y,
        buttonSize,
        buttonSize);

    abButton.setBounds(
        startX + spacing * 4,
        y,
        buttonSize,
        buttonSize);

}

void AudioPluginAudioProcessorEditor::timerCallback()
{
    animator.update();

    repaint();
}