#include "fitnesstrainer.h"

FitnessTrainer::FitnessTrainer(int id, const QString &userName, const QString &email, const QString &password, const QString &qualifications, QObject *parent)
    : User(id, userName, email, password, parent), qualifications(qualifications) {}

void FitnessTrainer::assignPlan(int planId, const QDateTime &startDate, const QDateTime &endDate) {
    // Implement plan assignment logic
}

QString FitnessTrainer::getQualifications() const {
    return qualifications;
}

void FitnessTrainer::setQualifications(const QString &newQualifications) {
    qualifications = newQualifications;
}
