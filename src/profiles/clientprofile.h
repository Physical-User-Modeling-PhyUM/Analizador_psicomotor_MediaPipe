/*!
 * \file clientprofile.h
 * \brief Declaración de la clase ClientProfile.
 *
 * Esta clase representa el perfil físico y funcional de un cliente en el sistema, incluyendo datos
 * generales, morfológicos, funcionales, médicos, estilo de vida, amplitud de movimiento (ROM) y datos adicionales.
 */

#ifndef CLIENTPROFILE_H
#define CLIENTPROFILE_H

#include <QMap>
#include <QVariant>
#include <QString>
#include <QStringList>
#include "enums/profileEnums.h"
#include "enums/enums.h"

/*!
 * \class ClientProfile
 * \brief Clase que encapsula todos los registros físicos, médicos y de estilo de vida de un cliente.
 *
 * Organiza los datos por categorías (general, funcional, morfológica, médica, ROM, estilo de vida y adicional),
 * y proporciona métodos seguros para su acceso y validación con límites definidos por tipo de campo.
 */
class ClientProfile
{
public:
    /*!
     * \brief Constructor por defecto.
     */
    ClientProfile();

    /*!
     * \brief Establece el ID del cliente asociado.
     * \param id Identificador único del cliente.
     */
    void setIdClient(int id);

    /*!
     * \brief Devuelve el ID del cliente asociado.
     * \return ID entero del cliente.
     */
    int getIdClient() const;

    // Getters para distintos tipos de campo
    QVariant getRecord(GeneralDataField field) const;
    QVariant getRecord(FunctionalField field) const;
    QVariant getRecord(MorfologicalFields field) const;
    QVariant getRecord(ROMJoint field) const;
    QVariant getRecord(MedicalRecordsFields field) const;
    QVariant getRecord(LifeStyleFields field) const;
    QVariant getRecord(const QString& key) const;

    // Setters para distintos tipos de campo
    bool setRecord(GeneralDataField field, const QVariant& value);
    bool setRecord(FunctionalField field, const QVariant& value);
    bool setRecord(MorfologicalFields field, const QVariant& value);
    bool setRecord(ROMJoint field, const QVariant& value);
    bool setRecord(MedicalRecordsFields field, const QVariant& value);
    bool setRecord(LifeStyleFields field, const QVariant& value);
    bool setRecord(const QString& key, const QVariant& value);

    /*!
     * \brief Devuelve todos los registros de una categoría concreta del perfil.
     * \param type Tipo de categoría (General, Functional, etc.).
     * \return Mapa con los valores correspondientes.
     */
    QVariantMap getAllRecords(ClientProfileData type) const;

    /*!
     * \brief Establece múltiples registros en bloque para una categoría del perfil.
     * \param type Tipo de categoría.
     * \param map Mapa de valores a establecer.
     * \return true si todos los campos se asignaron correctamente, false si alguno falló.
     */
    bool setAllRecords(ClientProfileData type, const QVariantMap& map);

    friend class TestClientProfile; //!< Clase de prueba con acceso a miembros privados.

private:
    int idClient; //!< Identificador del cliente.

    QMap<GeneralDataField, QVariant> generalData;              //!< Datos generales (edad, altura, peso...).
    QMap<FunctionalField, QVariant> FunctionalRecords;         //!< Registros funcionales (resistencia, fuerza...).
    QMap<MorfologicalFields, double> MorfologicalRecords;      //!< Datos morfológicos (IMC, masa muscular...).
    QMap<ROMJoint, double> RangeOfMovements;                   //!< Amplitud de movimiento (ROM) por articulación.
    QMap<MedicalRecordsFields, QString> MedicalRecords;        //!< Historial médico (enfermedades, intervenciones...).
    QMap<LifeStyleFields, QString> lifeStyleRecords;           //!< Datos de estilo de vida (actividad, sueño...).
    QMap<QString, QString> aditionalData;                      //!< Datos personalizados adicionales.

    // Métodos de validación internos
    bool setGeneralRecord(GeneralDataField field, QVariant value);
    bool setFunctionalRecord(FunctionalField field, QVariant value);
    bool setMorfologicalRecord(MorfologicalFields field, double value);
    bool setROM(ROMJoint joint, double value);
};

#endif // CLIENTPROFILE_H
