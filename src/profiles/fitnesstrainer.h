#ifndef FITNESSTRAINER_H
#define FITNESSTRAINER_H

#include "user.h"

class FitnessTrainer : public User {
public:
    explicit FitnessTrainer(int id, const QString &userName, const QString &email, const QString &password, const QString &qualifications, QObject *parent = nullptr);
    void assignPlan(int planId, const QDateTime &startDate, const QDateTime &endDate) override;

    QString getQualifications() const;
    void setQualifications(const QString &newQualifications);

private:
    QString qualifications;
};

#endif // FITNESSTRAINER_H
