#include "KnobScale.h"

void KnobScale::addMark(const ScaleMark& mark)
{
    marks.push_back(mark);
}

const std::vector<ScaleMark>& KnobScale::getMarks() const
{
    return marks;
}