#ifndef TRAININGSESION_H
#define TRAININGSESION_H

#include <QDateTime>
#include <QElapsedTimer>
#include "pose/sesionreport.h"
#include "exercise.h"
#include "exerciseespec.h"

class TrainingSesion
{
public:
    TrainingSesion();
    SesionReport getReport() const;
    void setReport(const SesionReport &newReport);

private:
    int IdSesion;
    int idClient;
    ExerciseEspec espec;
    Exercise ex;
    QDateTime date;
    qint64 Duration;
    SesionReport report;

};

#endif // TRAININGSESION_H
