#include "trainingsesion.h"

TrainingSesion::TrainingSesion() {}

SesionReport TrainingSesion::getReport() const
{
    return report;
}

void TrainingSesion::setReport(const SesionReport &newReport)
{
    report = newReport;
}
