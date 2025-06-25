/**
 * @file validationmanager.cpp
 * @brief Implementación de la clase ValidationManager para validación de entradas.
 */
#include "validationmanager.h"
/**
 * @brief Constructor de la clase ValidationManager.
 */
ValidationManager::ValidationManager(QObject* parent) : QObject(parent) {}
/**
 * @brief Valida un campo del tipo UserField.
 *
 * Reglas:
 * - Email: debe tener formato válido.
 * - UserName, Name, FamilyName: no pueden estar vacíos.
 * - Pass: mínimo 6 caracteres.
 *
 * @param field Campo a validar.
 * @param value Valor ingresado.
 * @return true si es válido, false si hay error.
 */
bool ValidationManager::validate(UserField field, const QVariant& value) {

    switch (field) {

        case UserField::Email: {
            QString email = value.toString().trimmed();
            QRegularExpression re(R"((\w+)(\.\w+)*@(\w+\.)+\w+)");
            if (!re.match(email).hasMatch()) {
                emit ui_errorMessage("Formato de email no válido.");
                return false;
            }
            return true;
        }
        case UserField::UserName:
        case UserField::Name:
        case UserField::FamilyName: {
            QString s = value.toString().trimmed();
            if (s.isEmpty()) {
                emit ui_errorMessage("Este campo no puede estar vacío.");
                return false;
            }
            return true;
        }
        case UserField::Pass: {
            QString pass = value.toString();
            if (pass.length() < 6) {
                emit ui_errorMessage("La contraseña debe tener al menos 6 caracteres.");
                return false;
            }
            return true;
        }
        case UserField::Plan:
        case UserField::last_login:
        case UserField::join_up_date:
        case UserField::Qualification:
        case UserField::specialtyArea:
        case UserField::experienceYears:
        case UserField::resume:
        case UserField::permissionLevel:
        case UserField::roleDescription:
        case UserField::canManageUsers:
        case UserField::canModifyPlans:

        default:
            return true;
        }
}
/**
 * @brief Valida un campo del tipo ExEspecField (especificación de ejercicio).
 *
 * Reglas:
 * - Name y Description: no pueden estar vacíos.
 * - Valores numéricos (Series, Weight, etc.): deben ser positivos.
 *
 * @param field Campo a validar.
 * @param value Valor proporcionado.
 * @return true si es válido.
 */
bool ValidationManager::validate(ExEspecField field, const QVariant& value) {
    switch (field) {
        case ExEspecField::Name:
        case ExEspecField::Description: {
            if (value.toString().trimmed().isEmpty()) {
                emit ui_errorMessage("El campo no puede estar vacío.");
                return false;
            }
            return true;
        }
        case ExEspecField::ExersiseType:
        case ExEspecField::TargetMuscle:
        case ExEspecField::Equipment:
        case ExEspecField::Series:
        case ExEspecField::Repetitions:
        case ExEspecField::Weight: {
            bool ok;
            double num = value.toDouble(&ok);
            if (!ok || num < 0) {
                emit ui_errorMessage("El valor debe ser numérico y positivo.");
                return false;
            }
            return true;
        }
        case ExEspecField::RestTime:
        default:
            return true;
        }
    }
/**
 * @brief Valida un campo del tipo WorkoutField (entrenamiento).
 *
 * Reglas:
 * - Name y Description: no deben estar vacíos.
 * - Duración: debe ser numérica y positiva.
 *
 * @param field Campo a validar.
 * @param value Valor recibido.
 * @return true si pasa la validación.
 */
bool ValidationManager::validate(WorkoutField field, const QVariant& value) {
    switch (field) {
        case WorkoutField::Name:
        case WorkoutField::Description: {
            if (value.toString().trimmed().isEmpty()) {
                emit ui_errorMessage("Este campo no puede estar vacío.");
                return false;
            }
            return true;
        }
        case WorkoutField::duration: {
            bool ok;
            int dur = value.toInt(&ok);
            if (!ok || dur <= 0) {
                emit ui_errorMessage("La duración debe ser un número positivo.");
                return false;
            }
            return true;
        }
        default:
            return true;
        }
}
