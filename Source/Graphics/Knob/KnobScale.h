#pragma once

#include <vector>

#include "ScaleMark.h"

class KnobScale
{
public:

    KnobScale() = default;

    void addMark(const ScaleMark& mark);

    const std::vector<ScaleMark>& getMarks() const;

private:

    std::vector<ScaleMark> marks;
};