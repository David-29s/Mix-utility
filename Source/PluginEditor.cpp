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


    startTimerHz(60);


    setSize(350, 400);
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
    constexpr int knobWidth  = 280;
    constexpr int knobHeight = 360;

    gainKnob.setBounds(
        getLocalBounds()
            .withSizeKeepingCentre(
                knobWidth,
                knobHeight));
}

void AudioPluginAudioProcessorEditor::timerCallback()
{
    animator.update();

    repaint();
}