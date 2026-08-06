#include "GainScale.h"

KnobScale createGainScale()
{
    KnobScale scale;

     scale.addMark({ 0.000f, true, "-24", -24.0f, 16.0f });

    scale.addMark({ 0.025f, false, "", 0.0f });
    scale.addMark({ 0.050f, false, "", 0.0f });
    scale.addMark({ 0.075f, false, "", 0.0f });
    scale.addMark({ 0.100f, false, "", 0.0f });

   // scale.addMark({ 0.125f, true, "-18", -18.0f, 16.0f });
    scale.addMark({ 0.125f, true, "", -18.0f, 16.0f });

    scale.addMark({ 0.150f, false, "", 0.0f });
    scale.addMark({ 0.175f, false, "", 0.0f });
    scale.addMark({ 0.200f, false, "", 0.0f });
    scale.addMark({ 0.225f, false, "", 0.0f });

    scale.addMark({ 0.250f, true, "-12", -12.0f, 16.0f });
    // scale.addMark({ 0.250f, true, "", -12.0f, 16.0f });

    scale.addMark({ 0.275f, false, "", 0.0f });
    scale.addMark({ 0.300f, false, "", 0.0f });
    scale.addMark({ 0.325f, false, "", 0.0f });
    scale.addMark({ 0.350f, false, "", 0.0f });

    //scale.addMark({ 0.375f, true, "-6", -6.0f, 14.0f });
    scale.addMark({ 0.375f, true, "", -6.0f, 14.0f });

    scale.addMark({ 0.400f, false, "", 0.0f });
    scale.addMark({ 0.425f, false, "", 0.0f });
    scale.addMark({ 0.450f, false, "", 0.0f });
    scale.addMark({ 0.475f, false, "", 0.0f });

    scale.addMark({ 0.500f, true, "0db", 0.0f, 13.0f });

    scale.addMark({ 0.525f, false, "", 0.0f });
    scale.addMark({ 0.550f, false, "", 0.0f });
    scale.addMark({ 0.575f, false, "", 0.0f });
    scale.addMark({ 0.600f, false, "", 0.0f });

    //scale.addMark({ 0.625f, true, "+6", 6.0f, 14.0f });
    scale.addMark({ 0.625f, true, "", 6.0f, 14.0f });

    scale.addMark({ 0.650f, false, "", 0.0f });
    scale.addMark({ 0.675f, false, "", 0.0f });
    scale.addMark({ 0.700f, false, "", 0.0f });
    scale.addMark({ 0.725f, false, "", 0.0f });

    scale.addMark({ 0.750f, true, "+12", 12.0f, 16.0f });
    // scale.addMark({ 0.750f, true, "", 12.0f, 16.0f });

    scale.addMark({ 0.775f, false, "", 0.0f });
    scale.addMark({ 0.800f, false, "", 0.0f });
    scale.addMark({ 0.825f, false, "", 0.0f });
    scale.addMark({ 0.850f, false, "", 0.0f });

    //scale.addMark({ 0.875f, true, "+18", 18.0f, 16.0f });
    scale.addMark({ 0.875f, true, "", 18.0f, 16.0f });

    scale.addMark({ 0.900f, false, "", 0.0f });
    scale.addMark({ 0.925f, false, "", 0.0f });
    scale.addMark({ 0.950f, false, "", 0.0f });
    scale.addMark({ 0.975f, false, "", 0.0f });

    scale.addMark({ 1.000f, true, "+24", 24.0f, 16.0f });

    return scale;
}