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

    //Etiqueta del centro del knob

    addAndMakeVisible(valueLabel);

    valueLabel.addMouseListener(this, true);

    valueLabel.setMouseCursor(
    juce::MouseCursor::PointingHandCursor);

    valueLabel.setText("0.0 dB", juce::dontSendNotification);

    valueLabel.setJustificationType(
        juce::Justification::centred);

    valueLabel.setFont(
        juce::Font(19.0f, juce::Font::bold));

    valueLabel.setColour(
        juce::Label::textColourId,
        juce::Colours::cyan);

    slider.onValueChange = [this]()
    {
        valueLabel.setText(
            juce::String(slider.getValue(), 1)
            + " "
            + knobUnit,
            juce::dontSendNotification);
    };

    addAndMakeVisible(valueEditor);

    valueEditor.setMouseCursor(
    juce::MouseCursor::IBeamCursor);

    valueEditor.setVisible(false);

    valueEditor.setPopupMenuEnabled(false);

    valueEditor.setJustification(
        juce::Justification::centred);

    valueEditor.setFont(
        juce::Font(22.0f, juce::Font::bold));

    valueEditor.setColour(
    juce::TextEditor::textColourId,
    juce::Colours::cyan);

    valueEditor.setColour(
        juce::TextEditor::backgroundColourId,
        juce::Colours::transparentBlack);

    valueEditor.setColour(
        juce::TextEditor::outlineColourId,
        juce::Colours::transparentBlack);

    valueEditor.setColour(
        juce::TextEditor::focusedOutlineColourId,
        juce::Colours::transparentBlack);

    valueEditor.setInputRestrictions(
        10,
        "0123456789.-");

    valueEditor.onReturnKey = [this]()
    {
        double newValue =
            valueEditor.getText().getDoubleValue();

        newValue = juce::jlimit(
            -24.0,
            24.0,
            newValue);

        slider.setValue(
            newValue,
            juce::sendNotificationSync);

        valueEditor.setVisible(false);

        valueLabel.setVisible(true);

        slider.grabKeyboardFocus();

        repaint();
    };

    valueEditor.onEscapeKey = [this]()
    {
        valueEditor.setVisible(false);

        valueLabel.setVisible(true);

        slider.grabKeyboardFocus();

        repaint();
    };
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
    auto bounds = getLocalBounds();

    slider.setBounds(
        bounds
            .reduced(2)
            .withTrimmedTop(25));

    // Colocar el valor en el centro del knob
    auto sliderBounds = slider.getBounds();

    valueLabel.setBounds(
        sliderBounds.getCentreX() - 50,
        sliderBounds.getCentreY() - 15,
        100,
        30);

    valueEditor.setBounds(
        sliderBounds.getCentreX() - 50,
        sliderBounds.getCentreY() - 15,
        100,
        30);
}

juce::Slider& NeonKnob::getSlider()
{
    return slider;
}

NeonLookAndFeel& NeonKnob::getLookAndFeel()
{
    return lookAndFeel;
}

void NeonKnob::setTitle(
    const juce::String& newTitle)
{
    knobTitle = newTitle;

    repaint();
}

void NeonKnob::setUnit(
    const juce::String& newUnit)
{
    knobUnit = newUnit;

    valueLabel.setText(
        juce::String(slider.getValue(), 1)
        + " "
        + knobUnit,
        juce::dontSendNotification);

    repaint();
}

void NeonKnob::mouseDoubleClick(
    const juce::MouseEvent& event)
{
    if (event.mods.isLeftButtonDown())
    {
        valueEditor.setText(
            juce::String(slider.getValue(), 1),
            false);

        valueEditor.setVisible(true);

        valueLabel.setVisible(false);

        valueEditor.grabKeyboardFocus();

        valueEditor.selectAll();
    }
}

