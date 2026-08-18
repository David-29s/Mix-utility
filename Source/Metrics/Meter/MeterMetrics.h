#pragma once

namespace MeterMetrics
{
    //==========================================================
    // Meter range
    //==========================================================

    inline constexpr float minDb = -60.0f;
    inline constexpr float maxDb = 12.0f;

    //==========================================================
    // Meter dimensions
    //==========================================================

    inline constexpr int meterWidth = 23;
    inline constexpr int meterHeight = 270;

    //==========================================================
    // Stereo channels
    //==========================================================

    inline constexpr int channelGap = 5;


    //==========================================================
    // Peak Hold
    //==========================================================

    inline constexpr float peakHoldTimeMs = 500.0f; // el pico permanecera 500 ms antes de empezar a bajar
    inline constexpr float peakDecayDbPerSecond = 90.0f; //significa que después del hold la línea bajará


    inline constexpr float labelRiseDbPerSecond = 10.0f;
    inline constexpr float labelFallDbPerSecond = 60.0f;
    //==========================================================
    // Meter frame
    //==========================================================

    inline constexpr int meterPadding = 5;
    inline constexpr int scaleWidth = 28;
    inline constexpr int labelGap = 6;

    inline constexpr int componentWidth =
      scaleWidth +
      labelGap +
      (meterWidth * 2) +
      channelGap;

    //==========================================================
    // Input Meter Position
    //==========================================================

    inline constexpr int inputMeterX = 9;
    inline constexpr int inputMeterY = 25;

    //==========================================================
    // Output Meter Position
    //==========================================================

    inline constexpr int outputMeterX = 445;
    inline constexpr int outputMeterY = 25;

    //==========================================================
    // Meter labels
    //==========================================================

    inline constexpr int labelTopHeight = 42;
    inline constexpr int labelBottomHeight = 20;

    inline constexpr int componentHeight =
        labelTopHeight +
        meterHeight +
        labelBottomHeight;

    inline constexpr int inputLabelWidth = 50;


    inline constexpr int inputReadoutOffsetX = -2;
    inline constexpr int outputReadoutOffsetX = 1;

    //Input / output Text
    inline constexpr int inputLabelOffsetX = 0;
    inline constexpr int outputLabelOffsetX = 0;

    inline constexpr int inputLabelOffsetY = -4;
    inline constexpr int outputLabelOffsetY = -4;




}