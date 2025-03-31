#include "clientprofile.h"

ClientProfile::ClientProfile()
    : idClient(-1) // Inicializa el ID con valor inválido por defecto
{
}

// Getters y setters (puedes extenderlos según lo necesites)
void ClientProfile::setIdClient(int id) {
    idClient = id;
}

int ClientProfile::getIdClient() const {
    return idClient;
}



void ClientProfile::setFunctionalRecord(FunctionalField field, const QVariant& value) {
    FunctionalRecords[field] = value;
}

QVariant ClientProfile::getFunctionalRecord(FunctionalField field) const {
    return FunctionalRecords.value(field, QVariant());
}

void ClientProfile::setMorfologicalRecord(MorfologicalFields field, double value) {
    MorfologicalRecords[field] = value;
}

double ClientProfile::getMorfologicalRecord(MorfologicalFields field) const {
    return MorfologicalRecords.value(field, -1.0);
}

void ClientProfile::setROM(ROMJoint joint, double value) {
    RangeOfMovements[joint] = value;
}

double ClientProfile::getROM(ROMJoint joint) const {
    return RangeOfMovements.value(joint, -1.0);
}

void ClientProfile::addMedicalRecord(MedicalRecordsFields field, const QString& entry) {
    MedicalRecords[field].append(entry);
}

QString ClientProfile::getMedicalRecord(MedicalRecordsFields field) const {
    return MedicalRecords.value(field);
}

QMap<MedicalRecordsFields, QString> ClientProfile::getMedicalRecords() const
{
    return MedicalRecords;
}

void ClientProfile::setLifestyleRecord(LifeStyleFields field, const QString& value) {
    lifeStyleRecords[field] = value;
}

QString ClientProfile::getLifestyleRecord(LifeStyleFields field) const {
    return lifeStyleRecords.value(field);
}

void ClientProfile::setAdditionalData(const QString& key, const QString& value) {
    aditionalData[key] = value;
}

QString ClientProfile::getAdditionalData(const QString& key) const {
    return aditionalData.value(key);
}

QStringList ClientProfile::getAllAdditionalKeys() const {
    return aditionalData.keys();
}

QMap<FunctionalField, QVariant> ClientProfile::getFunctionalRecords() const
{
    return FunctionalRecords;
}

void ClientProfile::setFunctionalRecords(const QMap<FunctionalField, QVariant> &newFunctionalRecords)
{
    FunctionalRecords = newFunctionalRecords;
}

QMap<MorfologicalFields, double> ClientProfile::getMorfologicalRecords() const
{
    return MorfologicalRecords;
}

void ClientProfile::setMorfologicalRecords(const QMap<MorfologicalFields, double> &newMorfologicalRecords)
{
    MorfologicalRecords = newMorfologicalRecords;
}

QMap<ROMJoint, double> ClientProfile::getRangeOfMovements() const
{
    return RangeOfMovements;
}

void ClientProfile::setRangeOfMovements(const QMap<ROMJoint, double> &newRangeOfMovements)
{
    RangeOfMovements = newRangeOfMovements;
}

void ClientProfile::setMedicalRecords(const QMap<MedicalRecordsFields, QString > &newMedicalRecords)
{
    MedicalRecords = newMedicalRecords;
}

QMap<QString, QString> ClientProfile::getAditionalData() const
{
    return aditionalData;
}

void ClientProfile::setAditionalData(const QMap<QString, QString> &newAditionalData)
{
    aditionalData = newAditionalData;
}

QMap<GeneralDataField, QVariant> ClientProfile::getGeneralData() const
{
    return generalData;
}

void ClientProfile::setGeneralData(const QMap<GeneralDataField, QVariant> &newGeneralData)
{
    generalData = newGeneralData;
}

QMap<LifeStyleFields, QString> ClientProfile::getLifeStyleRecords() const
{
    return lifeStyleRecords;
}

void ClientProfile::setLifeStyleRecords(const QMap<LifeStyleFields, QString> &newLifeStyleRecords)
{
    lifeStyleRecords = newLifeStyleRecords;
}
