#include "exercise.h"

Exercise::Exercise() {}

int Exercise::getRepetitions() const
{
    return repetitions;
}

int Exercise::getDuration() const
{
    return duration;
}

int Exercise::getRestTime() const
{
    return restTime;
}

int Exercise::getSeries() const
{
    return series;
}
