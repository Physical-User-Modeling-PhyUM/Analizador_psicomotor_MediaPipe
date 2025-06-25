/// @file assignworkoutdialog.h
/// @brief Declaración de la clase AssignWorkoutDialog, que permite asignar un workout a un cliente en una fecha concreta.

#ifndef ASSIGNWORKOUTDIALOG_H
#define ASSIGNWORKOUTDIALOG_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include "core/appcontroller.h"
#include "ui/main/uitablemodel.h"

namespace Ui {
class AssignWorkoutDialog;
}

/// @class AssignWorkoutDialog
/// @brief Diálogo que permite asignar un plan de entrenamiento (`Workout`) a un cliente en una fecha seleccionada.
///
/// Permite seleccionar un cliente de una tabla con filtro, elegir una fecha en un calendario y confirmar la asignación del workout.
class AssignWorkoutDialog : public QDialog
{
    Q_OBJECT

public:
    /// @brief Constructor.
    /// @param controller Controlador principal de la aplicación.
    /// @param idWorkout ID del workout a asignar.
    /// @param parent Widget padre opcional.
    explicit AssignWorkoutDialog(QSharedPointer<AppController> controller, int idWorkout, QWidget *parent = nullptr);

    /// @brief Destructor.
    ~AssignWorkoutDialog();

    /// @brief Devuelve el ID del cliente actualmente seleccionado en la tabla.
    int getSelectedClientId() const;

private slots:
    /// @brief Maneja la acción de clic en el botón "Asignar".
    void onAssignButtonClicked();

    /// @brief Maneja la acción de clic en el botón "Cancelar".
    void onCancelButtonClicked();

    /// @brief Evento de clic sobre una celda de la tabla de clientes.
    void on_clientsTable_clicked(const QModelIndex &index);

    /// @brief Muestra un filtro emergente al hacer clic en un encabezado de columna.
    void onHeaderClickedClients(int logicalIndex);

private:
    Ui::AssignWorkoutDialog *ui; ///< Interfaz gráfica generada por Qt Designer.
    QSharedPointer<AppController> controller; ///< Controlador principal.
    UiTableModel* clientModel; ///< Modelo de datos para los clientes.
    QSortFilterProxyModel* proxyModel; ///< Modelo proxy para aplicar filtros.
    int idWorkout; ///< ID del workout a asignar.

    /// @brief Configura la tabla de clientes cargando los datos y activando filtros.
    void setupClientsTable();
};

#endif // ASSIGNWORKOUTDIALOG_H
