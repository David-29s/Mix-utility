#include "NeonKnob.h"

NeonKnob::NeonKnob(const juce::String& title)
    : knobTitle(title)
{
    addAndMakeVisible(slider);

    slider.setSliderStyle(
      juce::Slider::RotaryHorizontalVerticalDrag);



    slider.setTextBoxStyle(
        juce::Slider::NoTextBox,
        false,
        0,
        0);

    slider.setLookAndFeel(&lookAndFeel);
}

NeonKnob::~NeonKnob()
{
    slider.setLookAndFeel(nullptr);
}

void NeonKnob::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::white);

    g.setFont(
        juce::Font(22.0f, juce::Font::bold));

    auto titleBounds =
        getLocalBounds()
            .removeFromTop(25);

    titleBounds.translate(0, 56); //Modificar esta linea para ajustar el texto (Gain)

    g.drawText(
        knobTitle,
        titleBounds,
        juce::Justification::centred);
}

void NeonKnob::resized()
{
    slider.setBounds(
        getLocalBounds()
            .reduced(2)
            .withTrimmedTop(25));
}

juce::Slider& NeonKnob::getSlider()
{
    return slider;
}