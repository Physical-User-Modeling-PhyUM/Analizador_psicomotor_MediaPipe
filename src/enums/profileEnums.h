#ifndef PROFILEENUM_H
#define PROFILEENUM_H

#include <QString>
#include <QStringList>
#include <QPair>
#include <QHash>

/**
 * @file profileEnums.h
 * @brief Define enumeraciones relacionadas con el perfil del cliente y funciones auxiliares para su conversión y manipulación.
 *
 * Este archivo contiene las definiciones de los distintos campos de datos utilizados para describir un perfil de cliente en el contexto de una aplicación de entrenamiento físico o rehabilitación.
 * Se incluyen enumeraciones para aspectos generales, funcionales, morfológicos, médicos, de estilo de vida y de rango de movimiento (ROM).
 * También se proporcionan funciones para convertir entre enumeraciones y sus representaciones en texto, así como para obtener límites estándar asociados a cada campo.
 */

/// Enum que representa las distintas categorías de datos de perfil del cliente.
enum class ClientProfileData {
    General,        ///< Datos generales (edad, peso, altura, etc.).
    Functional,     ///< Datos funcionales (movilidad, equilibrio, fuerza).
    Morfological,   ///< Datos morfológicos (índices corporales, longitudes).
    Medical,        ///< Historial médico relevante.
    LifeStyle,      ///< Estilo de vida (hábitos deportivos, consumo de tabaco).
    Additional,     ///< Información adicional no clasificada.
    ROM,            ///< Datos sobre el rango de movimiento articular.
    All             ///< Selección para todos los campos.
};

/**
 * @brief Convierte un valor de ClientProfileData en una cadena legible.
 * @param field Valor del enum a convertir.
 * @return Representación en texto del campo.
 */
QString ClientProfileDataToString(ClientProfileData field);

/**
 * @brief Convierte una cadena en un valor de ClientProfileData.
 * @param str Cadena con el nombre del campo.
 * @return Valor del enum correspondiente.
 */
ClientProfileData ClientProfileDataFromString(const QString& str);


// === GeneralDataField ===

/**
 * @brief Enum para campos generales del perfil del cliente.
 */
enum class GeneralDataField {
    Qualification,  ///< Nivel educativo o formación.
    Level,          ///< Nivel de experiencia física.
    Language,       ///< Idioma preferido.
    Notifications,  ///< Preferencias de notificaciones.
    Units,          ///< Unidades de medida preferidas.
    Age,            ///< Edad del cliente.
    Gender,         ///< Género.
    Weight,         ///< Peso corporal.
    Height,         ///< Estatura.
    Plan,           ///< Tipo de plan de entrenamiento asociado.
    Unknown         ///< Valor desconocido o sin clasificar.
};

QString GeneralDataFieldToString(GeneralDataField field);
GeneralDataField GeneralDataFieldFromString(const QString& str);

/**
 * @brief Devuelve los límites recomendados para un campo general.
 * @param field Campo general para el cual obtener los límites.
 * @return Par de valores mínimo y máximo aceptables.
 */
QPair<double, double> getGeneralDataFieldLimit(GeneralDataField field);


// === FunctionalField ===

/**
 * @brief Enum para campos funcionales de evaluación física.
 */
enum class FunctionalField {
    StaticPosture,              ///< Tipo de postura estática.
    HipMobility,                ///< Movilidad de cadera.
    AnkleMobility,              ///< Movilidad de tobillo.
    ShoulderMobilityLevel,     ///< Movilidad de hombros.
    SquatStrength,             ///< Evaluación del patrón de sentadilla.
    BalanceTest,               ///< Prueba de equilibrio.
    CardioLevel,               ///< Nivel cardiovascular estimado.
    Unknown                    ///< Campo desconocido o sin clasificar.
};

QString FunctionalFieldToString(FunctionalField field);
FunctionalField FunctionalFieldFromString(const QString& str);
QPair<double, double> getFunctionalFieldLimit(FunctionalField field);


// === MorfologicalFields ===

/**
 * @brief Enum para campos morfológicos del cliente.
 */
enum class MorfologicalFields {
    FatPercentage,         ///< Porcentaje de grasa corporal.
    MuscleMass,            ///< Masa muscular estimada.
    BMI,                   ///< Índice de masa corporal.
    VO2Max,                ///< Capacidad aeróbica máxima.
    HeartRate,             ///< Frecuencia cardíaca en reposo.
    TorsoLength,           ///< Longitud del torso.
    LegLength,             ///< Longitud de las piernas.
    ArmLength,             ///< Longitud de los brazos.
    ForearmLength,         ///< Longitud de los antebrazos.
    ThighCircumference,    ///< Circunferencia del muslo.
    WaistCircumference,    ///< Circunferencia de la cintura.
    Unknown                ///< Campo desconocido o sin clasificar.
};

QString MorfologicalFieldsToString(MorfologicalFields field);
MorfologicalFields MorfologicalFieldsFromString(const QString& str);
QPair<double, double> getMorfologicalLimit(MorfologicalFields field);


// === LifeStyleFields ===

/**
 * @brief Enum para campos relacionados con el estilo de vida del cliente.
 */
enum class LifeStyleFields {
    TrainingFrequency,     ///< Frecuencia de entrenamiento.
    TypeOfTraining,        ///< Tipo de entrenamiento realizado.
    PreviousSports,        ///< Experiencia deportiva previa.
    SmokingHabits,         ///< Hábitos de consumo de tabaco.
    Unknown                ///< Campo desconocido o sin clasificar.
};

QString LifeStyleFieldsToString(LifeStyleFields field);
LifeStyleFields LifeStyleFieldsFromString(const QString& str);
QPair<double, double> getLifeStyleFieldsLimit(LifeStyleFields field);


// === MedicalRecordsFields ===

/**
 * @brief Enum para campos médicos relevantes en el perfil del cliente.
 */
enum class MedicalRecordsFields {
    PreviousInjuries,       ///< Lesiones anteriores.
    MedicalLimitations,     ///< Limitaciones médicas o contraindicaciones.
    PainZones,              ///< Zonas del cuerpo con dolor.
    Unknown                 ///< Campo desconocido o sin clasificar.
};

QString MedicalRecordsFieldsToString(MedicalRecordsFields field);
MedicalRecordsFields MedicalRecordsFieldsFromString(const QString& str);
QPair<double, double> getMedicalRecordsFieldsLimit(MedicalRecordsFields field);


// === ROMJoint ===

/**
 * @brief Enum que define las articulaciones y movimientos incluidos en la evaluación de rango de movimiento (ROM).
 */
enum class ROMJoint {
    NeckFlexion,
    NeckExtension,
    NeckRotation,
    NeckLateralFlexion,
    ShoulderFlexion,
    ShoulderExtension,
    ShoulderAbduction,
    ShoulderAdduction,
    ShoulderInternalRotation,
    ShoulderExternalRotation,
    ElbowFlexion,
    ElbowExtension,
    ForearmSupination,
    ForearmPronation,
    WristFlexion,
    WristExtension,
    WristRadialDeviation,
    WristUlnarDeviation,
    FingerFlexion,
    ThumbOpposition,
    ThoracicRotation,
    ThoracicLateralFlexion,
    ThoracicExtension,
    LumbarFlexion,
    LumbarExtension,
    LumbarLateralFlexion,
    LumbarRotation,
    HipFlexion,
    HipExtension,
    HipAbduction,
    HipAdduction,
    HipInternalRotation,
    HipExternalRotation,
    KneeFlexion,
    KneeExtension,
    AnkleDorsiflexion,
    AnklePlantarflexion,
    FootInversion,
    FootEversion,
    ToeFlexion,
    ToeExtension,
    Unknown
};

QString ROMJointToString(ROMJoint field);
ROMJoint ROMJointFromString(const QString& str);
QPair<double, double> getROMLimit(ROMJoint field);


// === Funciones auxiliares ===

/**
 * @brief Devuelve los nombres de los campos de datos generales.
 * @return Lista de nombres de campos.
 */
QStringList GeneralDataFieldNames();

/**
 * @brief Devuelve los nombres de los campos funcionales.
 * @return Lista de nombres de campos.
 */
QStringList FunctionalFieldNames();

/**
 * @brief Devuelve los nombres de los campos morfológicos.
 * @return Lista de nombres de campos.
 */
QStringList MorfologicalFieldNames();

/**
 * @brief Devuelve los nombres de las articulaciones y movimientos ROM.
 * @return Lista de nombres de articulaciones.
 */
QStringList ROMJointNames();

/**
 * @brief Devuelve los nombres de los campos del historial médico.
 * @return Lista de nombres de campos.
 */
QStringList MedicalRecordsFieldNames();

/**
 * @brief Devuelve los nombres de los campos del estilo de vida.
 * @return Lista de nombres de campos.
 */
QStringList LifeStyleFieldNames();


#endif // PROFILEENUM_H
