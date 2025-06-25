/*!
 * \file fitnesstrainer.h
 * \brief Declaración de la clase FitnessTrainer.
 *
 * La clase FitnessTrainer representa a un usuario del sistema con rol de preparador físico.
 * Hereda de User e incorpora atributos adicionales como calificaciones, área de especialidad,
 * años de experiencia y un resumen profesional.
 */

#ifndef FITNESSTRAINER_H
#define FITNESSTRAINER_H

#include "user.h"
#include "enums/profileEnums.h"

/*!
 * \class FitnessTrainer
 * \brief Clase que representa a un preparador físico dentro del sistema.
 *
 * Esta clase extiende la funcionalidad de User para incorporar información profesional
 * específica de los entrenadores, como sus calificaciones, especialidad, experiencia y currículum.
 */
class FitnessTrainer : public User {
public:
    /*!
     * \brief Constructor que inicializa un entrenador con los datos proporcionados.
     * \param dataMap Mapa de campos clave-valor que contienen la información del entrenador.
     * \param parent Objeto padre dentro de la jerarquía de Qt.
     */
    explicit FitnessTrainer(QHash<UserField, QVariant> dataMap, QObject *parent = nullptr);

    /*!
     * \brief Devuelve las calificaciones académicas o profesionales del entrenador.
     * \return Texto con las calificaciones.
     */
    QString getQualifications() const;

    /*!
     * \brief Establece las calificaciones académicas o profesionales del entrenador.
     * \param newQualifications Texto descriptivo.
     */
    void setQualifications(const QString& newQualifications);

    /*!
     * \brief Devuelve el área de especialidad del entrenador (por ejemplo: fuerza, movilidad, etc.).
     * \return Texto con el área de especialidad.
     */
    QString getSpecialtyArea() const;

    /*!
     * \brief Establece el área de especialidad del entrenador.
     * \param area Texto descriptivo del área.
     */
    void setSpecialtyArea(const QString &area);

    /*!
     * \brief Devuelve los años de experiencia como entrenador.
     * \return Número entero de años.
     */
    int getYearsOfExperience() const;

    /*!
     * \brief Establece el número de años de experiencia.
     * \param years Valor entero con los años.
     */
    void setYearsOfExperience(int years);

    /*!
     * \brief Devuelve el resumen profesional o currículum del entrenador.
     * \return Texto con el resumen.
     */
    QString getResume() const;

    /*!
     * \brief Establece el resumen profesional del entrenador.
     * \param newResume Texto con la información del currículum.
     */
    void setResume(const QString &newResume);

private:
    QString qualifications;  //!< Calificaciones académicas o profesionales.
    QString specialtyArea;   //!< Área de especialidad del entrenador.
    int experienceYears;     //!< Años de experiencia laboral.
    QString resume;          //!< Currículum o resumen profesional.
};

#endif // FITNESSTRAINER_H
