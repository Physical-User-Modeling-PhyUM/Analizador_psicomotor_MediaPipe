/**
 * @file uiutils.h
 * @brief Declaración de la clase UiUtils con funciones auxiliares para poblar y extraer datos de modelos de tablas en la interfaz gráfica.
 *
 * Esta clase contiene funciones estáticas utilizadas para conectar datos de dominio con elementos visuales,
 * en particular `UiTableModel`, proporcionando herramientas para transformar estructuras internas en representaciones editables por el usuario.
 */

#ifndef UIUTILS_H
#define UIUTILS_H

#include "profiles/user.h"
#include "ui/main/uitablemodel.h"
#include "db/dbtable.h"
#include "workouts/workoutsummary.h"
#include "workouts/exerciseespec.h"
#include "workouts/trainingworkout.h"
#include "pose/state.h"
#include "pose/angleconstraint.h"
#include <QSortFilterProxyModel>
#include <QHeaderView>
#include <QTableView>
#include <QMenu>
#include <QWidgetAction>
#include <QLineEdit>
#include <QCursor>
#include "ui/main/comboboxdelegate.h"
#include <QStandardItem>
#include "enums/PoseViewEnum.h"

/**
 * @class UiUtils
 * @brief Clase de utilidades para convertir datos entre estructuras internas y modelos de tabla en la UI.
 *
 * Esta clase sirve como puente entre el modelo de datos y la interfaz de usuario. Facilita la carga de datos en tablas (`UiTableModel`)
 * y la conversión de los datos modificados por el usuario de vuelta a las estructuras originales. También permite mostrar filtros,
 * poblar tablas con enums, y gestionar transiciones de estados en ejercicios.
 */
class UiUtils {
public:
    // Conversión desde datos de dominio a tablas
    static void fromWorkoutSummaries(const QList<WorkoutSummary>& list, UiTableModel* model);
    static void fromExerciseSummaries(const QList<ExerciseSummary>& list, UiTableModel* model);
    static void fromWorkoutExercises(const QSharedPointer<TrainingWorkout>& workout, UiTableModel* model);
    static void fromExerciseStates(const QSharedPointer<ExerciseEspec>& exercise, UiTableModel* model);
    static void fromStateConstraints(const QHash<QString, AngleConstraint>& constraints, UiTableModel* model);
    static void fromStateTransitions(const QHash<QPair<int, int>, QSet<Condition>>& transitions, UiTableModel* model);
    static void fromUserList(const QList<QSharedPointer<User>>& list, UiTableModel* model);
    static void fromWorkoutCalendar(const QMap<QDateTime, int>& calendar, UiTableModel* model);
    static void fromSessionSummaries(const QMap<int, QDateTime>& sesiones, UiTableModel* model);

    // Conversión desde tablas a estructuras
    static QHash<QPair<int, int>, QSet<Condition>> toStateTransitions(UiTableModel* model);
    static QSet<State> toExerciseStates(QSet<State> originalStates, UiTableModel* model);
    static void updateExerciseStates(QSet<State>& states, UiTableModel* model);
    static void updateExerciseEspecFromTable(UiTableModel* model, QSharedPointer<ExerciseEspec> ex);
    static void updateWorkoutFromTable(UiTableModel* model, QSharedPointer<TrainingWorkout> workout);
    static QHash<QString, AngleConstraint> toStateConstraints(UiTableModel* model);

    // Utilidades adicionales
    static void showFilterPopup(QWidget *parent, QTableView *table, QSortFilterProxyModel *proxy, int logicalIndex);
    static void populateEnumTable(QTableView* table, const QVariantMap& data, const QStringList& allFields);
    static QVariantMap tableToVariantMap(QTableView* table);
};

#endif // UIUTILS_H
