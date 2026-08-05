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

    inline const juce::Colour frameBorder      { 0xff252B34 };
    inline const juce::Colour frameHighlight   { 0xff4A515E };
    inline const juce::Colour frameShadow      { 0xff3A2617 };
    inline const juce::Colour frameInnerBorder { 0xff8C6542 };

    //==========================================================
    // Accent
    //==========================================================

    inline const juce::Colour cyan       { 0xff00E5FF };
    inline const juce::Colour magenta    { 0xffFF38F5 };

    //==========================================================
    // Glow
    //==========================================================

    inline const juce::Colour cyanGlow   { 0x8014D8FF };
    inline const juce::Colour whiteGlow  { 0x40FFFFFF };
}