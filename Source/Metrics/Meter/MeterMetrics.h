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

    inline constexpr int meterWidth = 18;
    inline constexpr int meterHeight = 270;

    //==========================================================
    // Stereo channels
    //==========================================================

    inline constexpr int channelGap = 5;


    //==========================================================
    // Peak Hold
    //==========================================================

    inline constexpr float peakHoldTimeMs = 500.0f; // el pico permanecera 500 ms antes de empezar a bajar
    inline constexpr float peakDecayDbPerSecond = 100.0f; //significa que después del hold la línea bajará

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

    inline constexpr int inputMeterX = 5;
    inline constexpr int inputMeterY = 25;

    //==========================================================
    // Output Meter Position
    //==========================================================

    inline constexpr int outputMeterX = 455;
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
    
}