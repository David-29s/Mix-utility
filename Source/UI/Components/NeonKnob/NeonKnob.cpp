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
    slider.setTooltip("Adjust the current mode value. Double-click the readout to enter a value.");

    //Etiqueta del centro del knob

    addAndMakeVisible(valueLabel);

    valueLabel.addMouseListener(this, true);

    valueLabel.setMouseCursor(
    juce::MouseCursor::PointingHandCursor);

    valueLabel.setText("0.0 dB", juce::dontSendNotification);
    valueLabel.setTooltip("Current value. Double-click to edit it numerically.");

    valueLabel.setJustificationType(
        juce::Justification::centred);

    valueLabel.setFont(
        juce::Font(23.0f, juce::Font::plain));

    valueLabel.setColour(
    juce::Label::textColourId,
    juce::Colours::white.withAlpha(0.85f));

    addAndMakeVisible(modeLabel);
    modeLabel.setJustificationType(juce::Justification::centred);
    modeLabel.setFont(juce::Font(12.0f, juce::Font::bold));
    modeLabel.setColour(juce::Label::textColourId,
                        juce::Colour(0xff91A1B3).withAlpha(0.9f));
    modeLabel.setInterceptsMouseClicks(false, false);

    slider.onValueChange = [this]()
    {
        updateReadout();
    };

    addAndMakeVisible(valueEditor);

    valueEditor.setMouseCursor(
    juce::MouseCursor::IBeamCursor);

    valueEditor.setVisible(false);

    valueEditor.setPopupMenuEnabled(false);
    valueEditor.setTooltip("Enter a control value and press Enter to apply it.");

    valueEditor.setJustification(
        juce::Justification::centred);

    valueEditor.setFont(
        juce::Font(23.0f, juce::Font::plain));

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
    slider.getMinimum(),
    slider.getMaximum(),
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
    g.setColour(
      juce::Colours::white.withAlpha(0.85f));

    g.setFont(
        juce::Font(25.0f, juce::Font::plain));

    auto titleBounds =
        getLocalBounds()
            .removeFromTop(25);

    titleBounds.translate(0, 32); //Modificar esta linea para ajustar el texto (Gain)

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

    wheelHelper.setBounds(0, 0, 0, 0);

    // Colocar el valor en el centro del knob
    auto sliderBounds = slider.getBounds();

    valueLabel.setBounds(sliderBounds.getCentreX() - 82,
                         sliderBounds.getCentreY() - 25,
                         164,
                         28);

    modeLabel.setBounds(sliderBounds.getCentreX() - 60,
                        sliderBounds.getCentreY() + 5,
                        120,
                        18);

    valueEditor.setBounds(
        sliderBounds.getCentreX() - 70,
        sliderBounds.getCentreY() - 18,
        140,
        28);
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

void NeonKnob::setMode(NeonLookAndFeel::KnobMode newMode)
{
    readoutMode = newMode;
    updateReadout();
}

void NeonKnob::updateReadout()
{
    const double value = slider.getValue();

    if (readoutMode == NeonLookAndFeel::KnobMode::gain)
    {
        const bool isMinimumGain = value <= slider.getMinimum() + 0.005;
        const auto valueText = isMinimumGain
            ? juce::String::fromUTF8("-\xE2\x88\x9E dB")
            : (value > 0.0 ? "+" : "") + juce::String(value, 1) + " dB";

        valueLabel.setFont(juce::Font(23.0f, juce::Font::plain));
        valueLabel.setText(valueText, juce::dontSendNotification);
        modeLabel.setText(isMinimumGain ? "MIN"
                          : value < -0.05 ? "CUT"
                          : value > 0.05 ? "BOOST"
                                         : "UNITY",
                          juce::dontSendNotification);
    }
    else if (readoutMode == NeonLookAndFeel::KnobMode::pan)
    {
        const auto magnitude = juce::roundToInt(std::abs(value));
        const auto valueText = magnitude == 0
            ? juce::String("0 %")
            : juce::String(magnitude) + "% " + (value < 0.0 ? "L" : "R");

        valueLabel.setFont(juce::Font(23.0f, juce::Font::plain));
        valueLabel.setText(valueText, juce::dontSendNotification);
        modeLabel.setText(value < -0.5 ? "LEFT"
                          : value > 0.5 ? "RIGHT"
                                        : "CENTER",
                          juce::dontSendNotification);
    }
    else
    {
        valueLabel.setFont(juce::Font(23.0f, juce::Font::plain));
        valueLabel.setText(juce::String(juce::roundToInt(value)) + " %",
                           juce::dontSendNotification);
        modeLabel.setText(value < 0.5 ? "MONO"
                          : std::abs(value - 100.0) < 0.5 ? "STEREO"
                          : value < 100.0 ? "NARROW"
                                          : "WIDE",
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

