/**
 * @file validationmanager.h
 * @brief Declaración de la clase ValidationManager para validación de campos en formularios.
 *
 * Esta clase proporciona métodos para validar datos de entrada del usuario según distintos campos
 * definidos en los enumeradores `UserField`, `ExEspecField` y `WorkoutField`. Emite señales
 * para notificar a la interfaz de usuario sobre errores detectados durante la validación.
 */

#ifndef VALIDATIONMANAGER_H
#define VALIDATIONMANAGER_H

#include <QObject>
#include <QVariant>
#include <QRegularExpression>
#include "enums/UserFieldEnum.h"
#include "enums/ExEspecFieldEnum.h"
#include "enums/WorkoutFieldEnum.h"

/**
 * @class ValidationManager
 * @brief Clase que se encarga de la validación de entradas en formularios del sistema.
 *
 * Utiliza reglas predefinidas para asegurar que los valores ingresados por el usuario cumplan con
 * los formatos requeridos (email válido, campos no vacíos, contraseñas seguras, números positivos, etc.).
 * En caso de error, se emite una señal para que la interfaz pueda notificar al usuario.
 */
class ValidationManager : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructor del validador.
     * @param parent Objeto padre de Qt (opcional).
     */
    explicit ValidationManager(QObject* parent = nullptr);

    /**
     * @brief Valida un campo de tipo UserField.
     * @param field Campo de usuario a validar.
     * @param value Valor asociado al campo.
     * @return true si es válido, false si falla.
     */
    bool validate(UserField field, const QVariant& value);

    /**
     * @brief Valida un campo de tipo ExEspecField (especificaciones de ejercicio).
     * @param field Campo a validar.
     * @param value Valor a comprobar.
     * @return true si el valor es válido.
     */
    bool validate(ExEspecField field, const QVariant& value);

    /**
     * @brief Valida un campo de tipo WorkoutField (entrenamientos).
     * @param field Campo del entrenamiento.
     * @param value Valor del campo.
     * @return true si la validación fue exitosa.
     */
    bool validate(WorkoutField field, const QVariant& value);

signals:
    /**
     * @brief Señal emitida cuando se detecta un error de validación.
     * @param msg Mensaje a mostrar en la interfaz.
     * @param type Tipo de mensaje (por defecto advertencia).
     */
    void ui_errorMessage(const QString& msg, QtMsgType type = QtWarningMsg);
};

#endif // VALIDATIONMANAGER_H
