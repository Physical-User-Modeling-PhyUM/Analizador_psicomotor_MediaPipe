/*!
 * \file clientprofile.cpp
 * \brief Implementación de la clase ClientProfile.
 *
 * Esta clase gestiona los datos físicos y funcionales asociados a un cliente. Permite registrar y consultar
 * valores organizados por categorías, validarlos y almacenarlos de forma estructurada. Las categorías incluyen:
 * - Datos Generales
 * - Datos Funcionales
 * - Datos Morfológicos
 * - Rango de Movimiento (ROM)
 * - Registros Médicos
 * - Estilo de Vida
 * - Datos Adicionales personalizados
 */

#include "clientprofile.h"

/*!
 * \brief Constructor por defecto.
 *
 * Inicializa la estructura interna del perfil de cliente. Todos los campos se dejan vacíos
 * hasta ser asignados explícitamente mediante los setters.
 */

ClientProfile::ClientProfile() : idClient(-1) {}
/*!
 * \brief Establece el ID del cliente asociado a este perfil.
 * \param id Identificador único del cliente.
 */
void ClientProfile::setIdClient(int id) { idClient = id; }
/*!
 * \brief Devuelve el ID del cliente asociado.
 * \return Valor entero que identifica al cliente.
 */
int ClientProfile::getIdClient() const { return idClient; }
/*!
 * \brief Obtiene un valor del registro general especificado.
 * \param field Campo de tipo GeneralDataField.
 * \return Valor del campo o QVariant vacío si no está definido.
 */
QVariant ClientProfile::getRecord(GeneralDataField field) const {
    return generalData.value(field, QVariant());
}

/*!
 * \brief Obtiene un valor del registro funcional especificado.
 * \param field Campo de tipo FunctionalField.
 * \return Valor del campo o QVariant vacío si no está definido.
 */
QVariant ClientProfile::getRecord(FunctionalField field) const {
    return FunctionalRecords.value(field, QVariant());
}
/*!
 * \brief Obtiene un valor del registro morfológico.
 * \param field Campo de tipo MorfologicalFields.
 * \return Valor como QVariant (double) o vacío si no está definido.
 */
QVariant ClientProfile::getRecord(MorfologicalFields field) const {
    return MorfologicalRecords.value(field, -1.0);
}
/*!
 * \brief Obtiene el valor de amplitud de movimiento (ROM) de una articulación.
 * \param field Articulación (ROMJoint).
 * \return Valor como QVariant (double) o vacío si no está definido.
 */
QVariant ClientProfile::getRecord(ROMJoint field) const {
    return RangeOfMovements.value(field, -1.0);
}
/*!
 * \brief Obtiene un campo del historial médico.
 * \param field Campo de tipo MedicalRecordsFields.
 * \return Valor como QString encapsulado en QVariant.
 */
QVariant ClientProfile::getRecord(MedicalRecordsFields field) const {
    return MedicalRecords.value(field);
}
/*!
 * \brief Obtiene un campo del estilo de vida.
 * \param field Campo de tipo LifeStyleFields.
 * \return Valor como QString encapsulado en QVariant.
 */
QVariant ClientProfile::getRecord(LifeStyleFields field) const {
    return lifeStyleRecords.value(field);
}
/*!
 * \brief Obtiene un dato adicional personalizado por su clave textual.
 * \param key Cadena de texto clave.
 * \return Valor asociado o cadena vacía si no existe.
 */
QVariant ClientProfile::getRecord(const QString& key) const {
    return aditionalData.value(key);
}
/*!
 * \brief Establece un valor en los datos generales, con validación interna.
 */
bool ClientProfile::setRecord(GeneralDataField field, const QVariant& value) {
    return setGeneralRecord(field, value);
}

/*!
 * \brief Establece un valor funcional, con validación interna.
 */
bool ClientProfile::setRecord(FunctionalField field, const QVariant& value) {
    return setFunctionalRecord(field, value);
}

/*!
 * \brief Establece un valor morfológico (double) si es válido.
 */
bool ClientProfile::setRecord(MorfologicalFields field, const QVariant& value) {
    return value.canConvert<double>() ? setMorfologicalRecord(field, value.toDouble()) : false;
}

/*!
 * \brief Establece un valor ROM si es convertible a double.
 */
bool ClientProfile::setRecord(ROMJoint field, const QVariant& value) {
    return value.canConvert<double>() ? setROM(field, value.toDouble()) : false;
}

/*!
 * \brief Establece un valor en los registros médicos.
 */
bool ClientProfile::setRecord(MedicalRecordsFields field, const QVariant& value) {
    if (!value.canConvert<QString>()) return false;
    MedicalRecords[field] = value.toString();
    return true;
}

/*!
 * \brief Establece un valor en los registros de estilo de vida.
 */
bool ClientProfile::setRecord(LifeStyleFields field, const QVariant& value) {
    if (!value.canConvert<QString>()) return false;
    lifeStyleRecords[field] = value.toString();
    return true;
}

/*!
 * \brief Establece un dato adicional por clave textual.
 */
bool ClientProfile::setRecord(const QString& key, const QVariant& value) {
    if (!value.canConvert<QString>()) return false;
    aditionalData[key] = value.toString();
    return true;
}

/*!
 * \brief Devuelve todos los registros de una categoría específica del perfil.
 */
QVariantMap ClientProfile::getAllRecords(ClientProfileData type) const {

    QVariantMap  map;

    switch (type) {
        //TEnemos que recorrer los QMaps para agregarlo a QVariantMap
        //Seria mejor guardarlos en QVariantMap?
    case ClientProfileData::General:
        for (auto it = generalData.begin(); it != generalData.end(); ++it)
            map.insert(GeneralDataFieldToString(it.key()), it.value());
        break;
    case ClientProfileData::Functional:
        for (auto it = FunctionalRecords.begin(); it != FunctionalRecords.end(); ++it)
            map.insert(FunctionalFieldToString(it.key()), it.value());
        break;

    case ClientProfileData::Morfological:
        for (auto it = MorfologicalRecords.begin(); it != MorfologicalRecords.end(); ++it)
            map.insert(MorfologicalFieldsToString(it.key()), it.value());
        break;
    case ClientProfileData::Medical:
        for (auto it = MedicalRecords.begin(); it != MedicalRecords.end(); ++it)
            map.insert(MedicalRecordsFieldsToString(it.key()), it.value());
        break;
    case ClientProfileData::LifeStyle:
        for (auto it = lifeStyleRecords.begin(); it != lifeStyleRecords.end(); ++it)
            map.insert(LifeStyleFieldsToString(it.key()), it.value());
        break;

    case ClientProfileData::Additional:
        for (auto it = aditionalData.begin(); it != aditionalData.end(); ++it)
            map.insert(it.key(), it.value());

        break;
    default:
        break;
    }
    return map;
}
/*!
 * \brief Establece múltiples campos a la vez para una categoría del perfil.
 */
bool ClientProfile::setAllRecords(ClientProfileData type, const QVariantMap& map) {
    bool allOk = true;

    for (auto it = map.begin(); it != map.end(); ++it) {
        const QString& key = it.key();
        const QVariant& value = it.value();
        bool ok = false;

        if (type == ClientProfileData::General) {
            GeneralDataField field = GeneralDataFieldFromString(key);
            if (field != GeneralDataField::Unknown)
                ok = setRecord(field, value);
        }
        else if (type == ClientProfileData::Functional) {
            FunctionalField field = FunctionalFieldFromString(key);
            if (field != FunctionalField::Unknown)
                ok = setRecord(field, value);
        }
        else if (type == ClientProfileData::Morfological) {
            MorfologicalFields field = MorfologicalFieldsFromString(key);
            if (field != MorfologicalFields::Unknown)
                ok = setRecord(field, value);
        }
        else if (type == ClientProfileData::ROM) {
            ROMJoint field = ROMJointFromString(key);
            if (field != ROMJoint::Unknown)
                ok = setRecord(field, value);
        }
        else if (type == ClientProfileData::Medical) {
            MedicalRecordsFields field = MedicalRecordsFieldsFromString(key);
            if (field != MedicalRecordsFields::Unknown)
                ok = setRecord(field, value);
        }
        else if (type == ClientProfileData::LifeStyle) {
            LifeStyleFields field = LifeStyleFieldsFromString(key);
            if (field != LifeStyleFields::Unknown)
                ok = setRecord(field, value);
        }
        else if (type == ClientProfileData::Additional) {
            ok = setRecord(key, value);
        }

        if (!ok) {
            allOk = false;
            // Opcional: puedes registrar el fallo si quieres
            // qWarning() << "Fallo al asignar campo" << key << "con valor" << value << "en tipo" << ClientProfileDataToString(type);
        }
    }

    return allOk;
}


// con estos métodos validamos que los valores se encuentran denro de los límites
/*!
 * \brief Valida y asigna un campo general.
 */
bool ClientProfile::setGeneralRecord(GeneralDataField field, QVariant value) {
    auto range = getGeneralDataFieldLimit(field);
    if (range.first != -1 && range.second != -1) {
        double val = value.toDouble();
        if (val < range.first || val > range.second) return false;
    }
    generalData[field] = value;
    return true;
}
/*!
 * \brief Valida y asigna un campo funcional.
 */
bool ClientProfile::setFunctionalRecord(FunctionalField field, QVariant value) {
    auto range = getFunctionalFieldLimit(field);
    if (range.first != -1 && range.second != -1) {
        double val = value.toDouble();
        if (val < range.first || val > range.second) return false;
    }
    FunctionalRecords[field] = value;
    return true;
}
/*!
 * \brief Valida y asigna un campo morfológico.
 */
bool ClientProfile::setMorfologicalRecord(MorfologicalFields field, double value) {
    auto range = getMorfologicalLimit(field);
    if (range.first != -1 && range.second != -1) {
        if (value < range.first || value > range.second) return false;
    }
    MorfologicalRecords[field] = value;
    return true;
}
/*!
 * \brief Valida y asigna el ROM de una articulación.
 */
bool ClientProfile::setROM(ROMJoint joint, double value) {
    auto range = getROMLimit(joint);
    if (range.first != -1 && range.second != -1) {
        if (value < range.first || value > range.second) return false;
    }
    RangeOfMovements[joint] = value;
    return true;
}
