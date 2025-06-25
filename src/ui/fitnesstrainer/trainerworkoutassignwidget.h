/**
 * @file trainerworkoutassignwidget.h
 * @brief Declaración de la clase TrainerWorkoutAssignWidget, interfaz para asignar y desasignar entrenamientos a clientes.
 *
 * Este widget permite al entrenador gestionar la asignación de workouts a sus clientes.
 * Ofrece tres tablas filtrables: una con los clientes disponibles, otra con los entrenamientos asignados y otra con los no asignados.
 * Permite asignar workouts a fechas específicas o retirarlos de un cliente concreto.
 */

#ifndef TRAINERWORKOUTASSIGNWIDGET_H
#define TRAINERWORKOUTASSIGNWIDGET_H

#include <QWidget>
#include <QHash>
#include <QSharedPointer>
#include <QSortFilterProxyModel>

#include "core/appcontroller.h"
#include "profiles/fitnesstrainer.h"
#include "profiles/client.h"
#include "ui/main/uitablemodel.h"
#include "ui_trainerWorkoutAssignWidget.h"

/**
 * @class TrainerWorkoutAssignWidget
 * @brief Widget que permite al entrenador asignar o desasignar entrenamientos a sus clientes.
 *
 * Esta clase proporciona una interfaz para visualizar clientes y sus entrenamientos actuales,
 * así como asignar nuevos workouts o quitar asignaciones existentes.
 * La interfaz está organizada en tres secciones principales:
 * - Tabla de clientes
 * - Tabla de entrenamientos asignados
 * - Tabla de entrenamientos disponibles
 *
 * Utiliza modelos de tabla personalizados con soporte para filtrado dinámico.
 */
class TrainerWorkoutAssignWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor del widget.
     * @param controller Controlador principal de la aplicación.
     * @param parent Widget padre opcional.
     */
    explicit TrainerWorkoutAssignWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~TrainerWorkoutAssignWidget();

    /**
     * @brief Asigna el entrenador actual a esta vista.
     * @param trainer Entrenador autenticado.
     */
    void setTrainer(QSharedPointer<FitnessTrainer> trainer);

private slots:
    /**
     * @brief Acción al hacer clic sobre un cliente en la tabla.
     * @param index Índice de celda seleccionada.
     */
    void on_clientsTable_clicked(const QModelIndex &index);

    /**
     * @brief Acción al pulsar el botón de asignación de workout.
     */
    void on_assignButton_clicked();

    /**
     * @brief Acción al pulsar el botón para desasignar un workout.
     */
    void on_unassignButton_clicked();

    /**
     * @brief Muestra un popup de filtrado sobre la tabla de clientes.
     * @param logicalIndex Índice de columna clicado.
     */
    void onHeaderClickedClients(int logicalIndex);

    /**
     * @brief Muestra un popup de filtrado sobre la tabla de workouts asignados.
     * @param logicalIndex Índice de columna clicado.
     */
    void onHeaderClickedAssignedWorkouts(int logicalIndex);

    /**
     * @brief Muestra un popup de filtrado sobre la tabla de workouts disponibles.
     * @param logicalIndex Índice de columna clicado.
     */
    void onHeaderClickedAvailableWorkouts(int logicalIndex);

private:
    Ui_TrainerWorkoutAssignWidget *ui; ///< Interfaz gráfica generada por Qt Designer.
    QSharedPointer<AppController> controller; ///< Controlador de aplicación.
    QSharedPointer<FitnessTrainer> trainer = nullptr; ///< Entrenador actualmente autenticado.
    QSharedPointer<Client> currentClient = nullptr; ///< Cliente actualmente seleccionado.

    QPointer<UiTableModel> clientModel; ///< Modelo de clientes.
    QPointer<UiTableModel> assignedWorkoutModel; ///< Modelo de workouts ya asignados al cliente.
    QPointer<UiTableModel> availableWorkoutModel; ///< Modelo de workouts disponibles para asignar.

    QSortFilterProxyModel* proxyModel; ///< Filtro para la tabla de clientes.
    QSortFilterProxyModel* assignedProxyModel; ///< Filtro para la tabla de workouts asignados.
    QSortFilterProxyModel* availableProxyModel; ///< Filtro para la tabla de workouts disponibles.

    /**
     * @brief Configura la tabla de clientes, aplicando modelo y filtros.
     */
    void setupClientsTable();

    /**
     * @brief Carga los workouts asignados y no asignados del cliente actual.
     */
    void populateWorkoutTables();
};

#endif // TRAINERWORKOUTASSIGNWIDGET_H
