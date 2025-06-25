/// @file trainerboardwidget.h
/// @brief Declaración de la clase TrainerBoardWidget, la interfaz principal para el usuario con rol de entrenador físico.

#ifndef TRAINERBOARDWIDGET_H
#define TRAINERBOARDWIDGET_H

#include <QWidget>
#include <QSharedPointer>
#include <QPointer>
#include <QLoggingCategory>
#include <QQueue>

#include "core/appcontroller.h"
#include "profiles/fitnesstrainer.h"
#include "ui/fitnesstrainer/trainerinfowidget.h"
#include "ui/fitnesstrainer/trainerclientinfowidget.h"
#include "ui/fitnesstrainer/trainerworkoutdesignwidget.h"
#include "ui/fitnesstrainer/trainerworkoutassignwidget.h"
#include "ui/fitnesstrainer/trainermetricsmainwidget.h"
#include "ui_trainerboardwidget.h"

Q_DECLARE_LOGGING_CATEGORY(TrainerBoard)

/// @class TrainerBoardWidget
/// @brief Widget principal que permite al entrenador gestionar clientes, métricas y entrenamientos.
///
/// Contiene varias secciones organizadas en un `QStackedWidget` accesibles mediante un menú lateral.
/// Gestiona además un buffer de clientes en memoria para optimizar el acceso.
class TrainerBoardWidget : public QWidget
{
    Q_OBJECT

public:
    /// @brief Constructor.
    explicit TrainerBoardWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);

    /// @brief Destructor.
    ~TrainerBoardWidget();

    /// @brief Asigna el entrenador actual al widget.
    void setUser(QSharedPointer<FitnessTrainer> user);

    /// @brief Verifica si un cliente ya está en el buffer.
    bool isInBuffer(QSharedPointer<Client> client) const;

    /// @brief Verifica si un cliente con el ID dado está en el buffer.
    bool isInBuffer(int id) const;

    /// @brief Obtiene un cliente del buffer a partir de su ID.
    QSharedPointer<Client> getFromBuffer(int id) const;

    /// @brief Agrega un cliente al buffer, respetando el tamaño máximo.
    void addToBuffer(QSharedPointer<Client> client);

private slots:
    /// @brief Cambia de sección del menú lateral.
    void sectionChanged(int index);

    /// @brief Muestra mensajes de error o advertencia desde el sistema.
    void onUiErrorMessage(const QString& msg, QtMsgType type);

    /// @brief Método reservado para poblar la sección de métricas (no implementado).
    void populateMetricsTable();

signals:
    /// @brief Señal emitida cuando el usuario selecciona cerrar sesión.
    void logoutRequested();

private:
    Ui_TrainerBoardWidget *ui; ///< Interfaz gráfica generada por Qt Designer.
    QSharedPointer<AppController> controller; ///< Controlador principal de la aplicación.
    QSharedPointer<FitnessTrainer> currentTrainer; ///< Entrenador autenticado actualmente.

    QPointer<TrainerInfoWidget> infoWidget;
    QPointer<TrainerClientInfoWidget> infoClientWidget;
    QPointer<TrainerMetricsMainWidget> metricsWidget;
    QPointer<TrainerWorkoutDesignWidget> workoutDesignWidget;
    QPointer<TrainerWorkoutAssignWidget> assignWidget;

    QHash<int, QSharedPointer<Client>> clientBuffer; ///< Buffer de clientes.
    QQueue<int> bufferOrder; ///< Orden de inserción de los clientes en el buffer.

    /// @brief Actualiza los campos del encabezado con la información del entrenador.
    void updateTrainerInfo();
};

#endif // TRAINERBOARDWIDGET_H
