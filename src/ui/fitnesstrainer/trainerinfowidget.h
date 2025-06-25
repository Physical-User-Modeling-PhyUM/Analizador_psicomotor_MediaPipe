/**
 * @file trainerinfowidget.h
 * @brief Declaración de la clase TrainerInfoWidget, interfaz para que el entrenador visualice y actualice su perfil personal.
 *
 * Este widget permite al usuario con rol de entrenador ver y editar su información personal básica,
 * incluyendo nombre de usuario, correo electrónico, experiencia, especialidad y fotografía de perfil.
 * Proporciona un formulario con campos actualizables y control de carga de imagen desde archivo.
 */

#ifndef TRAINERINFOWIDGET_H
#define TRAINERINFOWIDGET_H

#include <QWidget>
#include <QSharedPointer>
#include "core/appcontroller.h"
#include "profiles/fitnesstrainer.h"
#include "ui_trainerInfoWidget.h"

/**
 * @class TrainerInfoWidget
 * @brief Widget que permite al entrenador consultar y modificar su información personal.
 *
 * Esta clase proporciona al entrenador una interfaz para ver su nombre, correo, especialidad, años de experiencia
 * y fotografía de perfil. El usuario puede actualizar manualmente la imagen desde el sistema de archivos.
 *
 * Los datos se obtienen del objeto `FitnessTrainer` asociado, y se gestionan mediante el `AppController`.
 */
class TrainerInfoWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor del widget.
     * @param controller Controlador principal de la aplicación, que permite acceder al modelo de dominio.
     * @param parent Widget padre (opcional).
     */
    explicit TrainerInfoWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~TrainerInfoWidget();

    /**
     * @brief Establece el entrenador que se visualizará y editará en el formulario.
     * @param trainer Puntero compartido al objeto `FitnessTrainer` autenticado.
     */
    void setTrainer(QSharedPointer<FitnessTrainer> trainer);

    /**
     * @brief Actualiza los campos de la interfaz con los datos del entrenador.
     */
    void updateTrainerInfo();

public slots:
    /**
     * @brief Abre un cuadro de diálogo para seleccionar una nueva imagen de perfil.
     *
     * Si se selecciona y carga correctamente una imagen, se actualiza en el modelo de dominio
     * y se notifica al `AppController` para persistencia.
     */
    void on_newPhoto_clicked();

private:
    Ui_TrainerInfoWidget *ui; ///< Interfaz gráfica del widget.
    QSharedPointer<AppController> controller; ///< Controlador principal que gestiona el modelo de aplicación.
    QSharedPointer<FitnessTrainer> trainer; ///< Entrenador autenticado actualmente.
};

#endif // TRAINERINFOWIDGET_H
