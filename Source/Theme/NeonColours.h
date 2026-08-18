#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace NeonColours
{
    //==========================================================
    // Base
    //==========================================================

    inline const juce::Colour background { 0xff15181d };
    inline const juce::Colour body       { 0xff20242b };
    inline const juce::Colour border     { 0xff353b44 };

    //==========================================================
    // Frame
    //==========================================================

    inline const juce::Colour frameBorder      { 0xff252B34 }; //Color del fondo de la ventana original
    inline const juce::Colour frameHighlight   { 0xff4A515E };
    inline const juce::Colour frameShadow      { 0xff3A2617 };
    inline const juce::Colour frameInnerBorder { 0xff8C6542 };

    //==========================================================
    // Accent
    //==========================================================

    inline const juce::Colour cyan       { 0xff00E5FF };
    inline const juce::Colour magenta    { 0xffFF38F5 };
    inline const juce::Colour orange     { 0xffffa500 };
    inline const juce::Colour purple     { 0xff800080 };
    inline const juce::Colour azul      { 0xff0080ff };
    inline const juce::Colour grey       { 0xff808080 };
    inline const juce::Colour violet     { 0xff9419bd};
    inline const juce::Colour neonGreen { 0xff00FF88 };

    //==========================================================
    // Glow
    //==========================================================

    inline const juce::Colour cyanGlow   { 0x8014D8FF };
    inline const juce::Colour whiteGlow  { 0x40FFFFFF };

    //==========================================================
    // Control Modes
    //==========================================================

    inline const juce::Colour gainNeon  { 0xff20E878 };
    inline const juce::Colour panNeon   { 0xffffe600 };
    inline const juce::Colour widthNeon { 0xff3D7BFF };

    // otros colores para el fondo
    //inline const juce::Colour frameBorder { 0xff1B1D29 };
    //inline const juce::Colour frameBorder { 0xff21272f};

    //==========================================================
    // Bass Mono
    //==========================================================

    inline const juce::Colour bassMonoPanel       { 0xff0C141C };
    inline const juce::Colour bassMonoBorder      { 0xff006EFF };

    inline const juce::Colour bassMonoControl     { 0xff232D32 };
    inline const juce::Colour bassMonoControlOff  { 0xff505A5F };

    inline const juce::Colour bassMonoActive      { 0xff00F078 };
    inline const juce::Colour bassMonoInactive    { 0xffD3D7DA };

    inline const juce::Colour bassMonoText        { 0xffD3D7DA }; //
    inline const juce::Colour bassMonoBorderSoft  { 0x4DFFFFFF };

    inline const juce::Colour bassMonoListenGlow  { 0x2E00E5FF };

    inline const juce::Colour electricBlue { 0xff006EFF };

    //==========================================================
    // Correlation Meter
    //==========================================================

    inline const juce::Colour correlationPanel { 0xff0C141C };

    inline const juce::Colour correlationBorder { 0xff353B44 };

    inline const juce::Colour correlationText { 0xffD3D7DA };

    inline const juce::Colour correlationTextSecondary { 0xff8E969E };

    inline const juce::Colour correlationMarker { 0xffffffff };

    inline const juce::Colour correlationBarBackground  { 0xff15181d };


    //----------------------------------------------------------
    // Correlation Zones
    //----------------------------------------------------------

    inline const juce::Colour correlationOutOfPhase { 0xffff3030 };

    inline const juce::Colour correlationLow { 0xffffe600 };

    inline const juce::Colour correlationInPhase { 0xff00C96A };


    //----------------------------------------------------------
    // Correlation Gradient
    //----------------------------------------------------------

    inline const juce::Colour correlationGradientRed { 0xffff3030 };

    inline const juce::Colour correlationGradientOrange { 0xffffa500 };

    inline const juce::Colour correlationGradientYellow { 0xffffe600 };

    inline const juce::Colour correlationGradientGreen { 0xff00C96A };




}