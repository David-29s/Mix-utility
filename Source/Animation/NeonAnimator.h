#pragma once

class NeonAnimator
{
public:

    void update();

    float getPulse() const;

private:

    float phase = 0.0f;

    float pulse = 1.0f;
};