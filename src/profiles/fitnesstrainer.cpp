/*!
 * \file fitnesstrainer.cpp
 * \brief Implementación de la clase FitnessTrainer.
 *
 * Esta implementación incluye la inicialización del objeto a partir de un mapa de campos,
 * así como el acceso y modificación de atributos propios del rol de preparador físico.
 */
#include "fitnesstrainer.h"
#include <QDebug>
/*!
 * \brief Constructor de FitnessTrainer.
 *
 * Este constructor valida que los campos obligatorios del usuario estén presentes en el `dataMap`.
 * Si la validación es satisfactoria, se procede a establecer los datos básicos del usuario y
 * los atributos específicos del entrenador.
 *
 * Además, se carga la imagen de perfil si está disponible y se aplica una validación
 * adicional sobre los años de experiencia (0 a 80 años).
 *
 * \param dataMap Mapa con datos clave del entrenador, incluyendo nombre, correo, contraseña, etc.
 * \param parent Objeto padre (por defecto nullptr).
 */
FitnessTrainer::FitnessTrainer(QHash<UserField, QVariant> dataMap, QObject *parent)
    : User(-1, "", "", "", parent),  qualifications(""),specialtyArea(""),experienceYears(0),resume("")
{
    //Comprobamos que disponga de los campos suficientes para la clase usuario
    if (!dataMap.contains(UserField::IdUser)
        || !dataMap.contains(UserField::UserName)
        || !dataMap.contains(UserField::Email)
        || !dataMap.contains(UserField::Pass)) {
        qWarning() << "Error: Faltan campos requeridos para crear un FitnessTrainer";
        return;
    }

    // Comprobamos que el valor sea un valor numérico válido, si es un QString, se convertirá a cero
    int id = dataMap.value(UserField::IdUser).toInt();
    if (id <-1) {
        qWarning() << "Constructor>>ID no válido";
        return;
    }

   //editamos los campos necesarios y nos aseguramos que no se encuentran vacios
    QString userName = dataMap.value(UserField::UserName).toString().trimmed();
    QString email = dataMap.value(UserField::Email).toString().trimmed();
    QString password = dataMap.value(UserField::Pass).toString();

    if (userName.isEmpty() || email.isEmpty() || password.isEmpty()) {
        qWarning() << "Campos obligatorios vacíos al crear un FitnessTrainer";
        return;
    }


    idUser = id;
    this->valid = true;
    this->userName = userName;
    this->email = email;
    this->password = password;
    this->userType=UserType::Trainer;
    QVariant imageData = dataMap.value(UserField::Picture);
    QImage image;
    if (imageData.canConvert<QByteArray>()) {
        qDebug()<<"imagen convertida a QbyteArray";
        image.loadFromData(imageData.toByteArray());
    }

    if (image.isNull()) {
        image = QImage(":/images/images/blank-profile.jpg");
        qWarning() << "No se pudo cargar ninguna imagen del perfil. Usando imagen en blanco.,,"
                   <<"null"<<imageData.isNull()<<"valid"<<imageData.isValid();
    }

    this->profile_Picture = image;
    //
    qualifications = dataMap.value(UserField::Qualification,"info not available").toString().trimmed();
    specialtyArea = dataMap.value(UserField::specialtyArea,"info not available").toString().trimmed();
    experienceYears = dataMap.value(UserField::experienceYears,0).toInt();
    resume = dataMap.value(UserField::resume,"info not available").toString().trimmed();

    // Validación para loa añ os de experiencia
    if (experienceYears < 0 || experienceYears > 80) {
        qWarning() << "Años de experiencia inválidos para FitnessTrainer";
        experienceYears = 0;
    }

}


/*!
 * \brief Devuelve las calificaciones del entrenador.
 * \return Texto con las certificaciones o estudios relevantes.
 */
QString FitnessTrainer::getQualifications() const {
    return qualifications;
}
/*!
 * \brief Asigna nuevas calificaciones al entrenador.
 * \param newQualifications Texto a establecer.
 */
void FitnessTrainer::setQualifications(const QString &newQualifications) {
    qualifications = newQualifications;
}
/*!
 * \brief Devuelve el área de especialidad del entrenador.
 * \return Texto descriptivo.
 */
QString FitnessTrainer::getSpecialtyArea() const {
    return specialtyArea;
}
/*!
 * \brief Asigna el área de especialidad del entrenador.
 * \param area Texto a establecer.
 */
void FitnessTrainer::setSpecialtyArea(const QString &area) {
    specialtyArea = area;
}
/*!
 * \brief Devuelve los años de experiencia.
 * \return Número entero entre 0 y 80.
 */
int FitnessTrainer::getYearsOfExperience() const {
    return experienceYears;
}
/*!
 * \brief Establece los años de experiencia.
 * \param years Valor entero a asignar.
 */
void FitnessTrainer::setYearsOfExperience(int years) {
    experienceYears = years;
}
/*!
 * \brief Devuelve el resumen profesional del entrenador.
 * \return Texto con información profesional adicional.
 */
QString FitnessTrainer::getResume() const {
    return resume;
}
/*!
 * \brief Establece el resumen profesional del entrenador.
 * \param newResume Texto del currículum.
 */
void FitnessTrainer::setResume(const QString &newResume) {
   this->resume = newResume;
}
