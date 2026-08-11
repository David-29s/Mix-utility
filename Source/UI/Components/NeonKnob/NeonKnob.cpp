#include "NeonKnob.h"

NeonKnob::NeonKnob(const juce::String& title)
    : knobTitle(title)
{
    addAndMakeVisible(slider);

    //==========================================================
    // Slider auxiliar para mouse wheel
    //==========================================================

    wheelHelper.setSliderStyle(
        juce::Slider::RotaryHorizontalVerticalDrag);

    wheelHelper.setTextBoxStyle(
        juce::Slider::TextBoxBelow,
        false,
        80,
        25);

    wheelHelper.setRange(
        0.0,
        100.0,
        1.0);

    wheelHelper.setValue(
        0.0,
        juce::dontSendNotification);

    addAndMakeVisible(wheelHelper);

    // Invisible, pero conserva la interacción del Slider de JUCE
    wheelHelper.setAlpha(0.0f);

    wheelHelper.onWheel =
    [this](const juce::MouseEvent& event,
           const juce::MouseWheelDetails& wheel)
    {
        const bool fineControl =
            event.mods.isShiftDown();

        const double range =
            slider.getMaximum() -
            slider.getMinimum();

        // Shift + Scroll = 0.005
        // Scroll normal = 0.10

        const double sensitivity =
            fineControl ? 0.006 : 0.15;

        const double delta =
            wheel.deltaY * range * sensitivity;

        slider.setValue(
            slider.getValue() + delta,
            juce::sendNotificationSync);
    };


    //==========================================================
    // Slider principal
    //==========================================================

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
        if (knobUnit == "%")
        {
            valueLabel.setText(
                juce::String(
                    juce::roundToInt(slider.getValue()))
                + " %",
                juce::dontSendNotification);
        }
        else
        {
            valueLabel.setText(
                juce::String(slider.getValue(), 1)
                + " dB",
                juce::dontSendNotification);
        }
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

    wheelHelper.setBounds(
     slider.getBounds().reduced(100));

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

    if (newUnit == "%")
    {
        valueLabel.setText(
            juce::String(
                juce::roundToInt(slider.getValue()))
            + " %",
            juce::dontSendNotification);
    }
    else
    {
        valueLabel.setText(
            juce::String(slider.getValue(), 1)
            + " dB",
            juce::dontSendNotification);
    }

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

