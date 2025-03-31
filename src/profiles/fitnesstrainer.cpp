#include "fitnesstrainer.h"
#include <QDebug>

FitnessTrainer::FitnessTrainer(QMap<UserField, QVariant> dataMap, QObject *parent)
    : User(
          dataMap.value(UserField::IdUser).toInt(),
          dataMap.value(UserField::UserName).toString(),
          dataMap.value(UserField::Email).toString(),
          dataMap.value(UserField::Pass).toString(),
          parent
          ),
    qualifications(dataMap.value(UserField::Qualification).toString()),
    specialtyArea(dataMap.value(UserField::Language).toString()),
    experienceYears(dataMap.value(UserField::Level).toInt()),
    resume(dataMap.value(UserField::Plan).toString())
{
    if (!dataMap.contains(UserField::IdUser)
        || !dataMap.contains(UserField::UserName)
        || !dataMap.contains(UserField::Email)
        || !dataMap.contains(UserField::Pass)) {
        qWarning() << "Error: Faltan campos requeridos para crear un FitnessTrainer";
        return;
    }
}

void FitnessTrainer::assignPlan(int, const QDateTime &, const QDateTime &) {
    // Aquí puedes implementar lógica de asignación de planes a usuarios
}

QString FitnessTrainer::getQualifications() const {
    return qualifications;
}

void FitnessTrainer::setQualifications(const QString &newQualifications) {
    qualifications = newQualifications;
}

QString FitnessTrainer::getSpecialtyArea() const {
    return specialtyArea;
}

void FitnessTrainer::setSpecialtyArea(const QString &area) {
    specialtyArea = area;
}

int FitnessTrainer::getYearsOfExperience() const {
    return experienceYears;
}

void FitnessTrainer::setYearsOfExperience(int years) {
    experienceYears = years;
}

QString FitnessTrainer::getResume() const {
    return resume;
}

void FitnessTrainer::setResume(const QString &newResume) {
    resume = newResume;
}
