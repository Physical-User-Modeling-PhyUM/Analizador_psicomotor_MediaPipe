/// @file usersadminwidget.h
/// @brief Declaración de la clase UsersAdminWidget, encargada de la gestión de usuarios en la interfaz de administrador.

#ifndef USERSADMINWIDGET_H
#define USERSADMINWIDGET_H

#include <QWidget>
#include <QSharedPointer>
#include <QTableView>
#include <QPushButton>
#include <QMap>
#include "ui_usersadminwidget.h"
#include "core/appcontroller.h"
#include "ui/main/uitablemodel.h"
#include "enums/UserFieldEnum.h"

namespace Ui {
class UsersAdminWidget;
}

/// @class UsersAdminWidget
/// @brief Widget para gestionar usuarios desde la interfaz de administración.
///
/// Permite listar, crear, editar y eliminar usuarios de cualquier tipo (Administrador, Entrenador, Cliente).
/// Utiliza dos tablas: una para listar usuarios existentes y otra para editar los detalles del usuario seleccionado.
class UsersAdminWidget : public QWidget {
    Q_OBJECT

public:
    /// @brief Constructor.
    /// @param controller Puntero al controlador principal de la aplicación.
    /// @param parent Widget padre opcional.
    explicit UsersAdminWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);

    /// @brief Destructor.
    ~UsersAdminWidget();

    /// @brief Reacciona al cambio de tipo de usuario en la tabla de edición.
    /// @param typeStr Tipo de usuario seleccionado ("Admin", "Trainer", "Client").
    void onUserTypeChanged(const QString &typeStr);

private slots:
    /// @brief Maneja la selección de un usuario desde la tabla de usuarios.
    /// @param index Índice del usuario seleccionado.
    void onUserSelected(const QModelIndex& index);

    /// @brief Guarda los cambios realizados en el usuario actualmente en edición.
    void onSaveClicked();

    /// @brief Elimina el usuario actualmente seleccionado.
    void onDeleteClicked();

    /// @brief Inicia la creación de un nuevo usuario.
    void onNewClicked();

    /// @brief Detecta cambios en los datos de la tabla de edición.
    void onEditTableDataChanged(const QModelIndex& m1, const QModelIndex& m2);

private:
    Ui_UsersAdminWidget *ui; ///< Interfaz gráfica generada por Qt Designer.
    QSharedPointer<AppController> controller; ///< Controlador principal de la aplicación.
    UiTableModel* userTableModel; ///< Modelo de datos para la tabla de usuarios.
    UiTableModel* editTableModel; ///< Modelo de datos para la tabla de edición.
    int selectedUserId = -1; ///< ID del usuario actualmente seleccionado o en edición.
    QSharedPointer<User> editUser; ///< Puntero al usuario actualmente en edición.

    /// @brief Carga todos los usuarios desde el sistema y los muestra en la tabla.
    void loadUsers();

    /// @brief Rellena la tabla de edición con los datos del usuario dado.
    /// @param user Usuario cuyos datos se van a editar.
    void populateEditTable(QSharedPointer<User> user);

    /// @brief Extrae los datos modificados por el usuario desde la tabla de edición.
    /// @return Mapa con los campos modificados y sus nuevos valores.
    QHash<UserField, QVariant> collectEditTableData();

    /// @brief Limpia el contenido de la tabla de edición.
    void clearEditTable();
};

#endif // USERSADMINWIDGET_H
