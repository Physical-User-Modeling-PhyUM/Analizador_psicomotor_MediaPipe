#ifndef FITNESSTRAINER_H
#define FITNESSTRAINER_H

#include "user.h"
#include "enums/GeneralDataEnum.h"

class FitnessTrainer : public User {
public:
    explicit FitnessTrainer(QMap<UserField, QVariant> dataMap, QObject *parent = nullptr);


    QString getQualifications() const;
    void setQualifications(const QString &newQualifications);

    QString getSpecialtyArea() const;
    void setSpecialtyArea(const QString &area);

    int getYearsOfExperience() const;
    void setYearsOfExperience(int years);

    QString getResume() const;
    void setResume(const QString &newResume);

private:
    QString qualifications;
    QString specialtyArea;
    int experienceYears;
    QString resume;
};

#endif // FITNESSTRAINER_H
