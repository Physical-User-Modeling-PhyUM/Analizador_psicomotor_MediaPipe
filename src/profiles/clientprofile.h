#ifndef CLIENTPROFILE_H
#define CLIENTPROFILE_H

#include <QMap>
#include <QVariant>
#include <QString>
#include <QStringList>
#include "enums/enums.h"

class ClientProfile
{
public:
    ClientProfile();

    // ID
    void setIdClient(int id);
    int getIdClient() const;

    //GeneralData

    QMap<GeneralDataField, QVariant> getGeneralData() const;
    void setGeneralData(const QMap<GeneralDataField, QVariant> &newGeneralData);
    void setGeneralRecord(GeneralDataField, const QVariant& value);

    // Functional Records
    void setFunctionalRecord(FunctionalField field, const QVariant& value);
    QVariant getFunctionalRecord(FunctionalField field) const;
    QMap<FunctionalField, QVariant> getFunctionalRecords() const;
    void setFunctionalRecords(const QMap<FunctionalField, QVariant> &newFunctionalRecords);


    // Morfological Records
    void setMorfologicalRecord(MorfologicalFields field, double value);
    double getMorfologicalRecord(MorfologicalFields field) const;
    QMap<MorfologicalFields, double> getMorfologicalRecords() const;
    void setMorfologicalRecords(const QMap<MorfologicalFields, double> &newMorfologicalRecords);

    // Range of Movements
    void setROM(ROMJoint joint, double value);
    double getROM(ROMJoint joint) const;
    QMap<ROMJoint, double> getRangeOfMovements() const;
    void setRangeOfMovements(const QMap<ROMJoint, double> &newRangeOfMovements);

    // Medical Records
    void addMedicalRecord(MedicalRecordsFields field, const QString& entry);
    QString getMedicalRecord(MedicalRecordsFields field) const;
    QMap<MedicalRecordsFields, QString> getMedicalRecords() const;
    void setMedicalRecords(const QMap<MedicalRecordsFields, QString > &newMedicalRecords);

    // Lifestyle Records
    void setLifestyleRecord(LifeStyleFields field, const QString& value);
    QString getLifestyleRecord(LifeStyleFields field) const;
    QMap<LifeStyleFields, QString> getLifeStyleRecords() const;
    void setLifeStyleRecords(const QMap<LifeStyleFields, QString> &newLifeStyleRecords);

    // Additional Data
    void setAdditionalData(const QString& key, const QString& value);
    QString getAdditionalData(const QString& key) const;
    QStringList getAllAdditionalKeys() const;
    QMap<QString, QString> getAditionalData() const;
    void setAditionalData(const QMap<QString, QString> &newAditionalData);


private:
    int idClient;
    QMap<GeneralDataField, QVariant> generalData;
    QMap<FunctionalField, QVariant> FunctionalRecords;
    QMap<MorfologicalFields, double> MorfologicalRecords;
    QMap<ROMJoint, double> RangeOfMovements;
    QMap<MedicalRecordsFields, QString> MedicalRecords;
    QMap<LifeStyleFields, QString> lifeStyleRecords;
    QMap<QString, QString> aditionalData;
};

#endif // CLIENTPROFILE_H
