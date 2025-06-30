
/// @brief Implementación de la clase UsersAdminWidget.

#include "usersadminwidget.h"
#include "ui_usersadminwidget.h"
#include "enums/UserFieldEnum.h"
#include "ui/main/uitablemodel.h"
#include "profiles/admin.h"
#include "profiles/fitnesstrainer.h"
#include "ui/main/comboboxdelegate.h"


/// @brief Constructor. Inicializa los modelos, conecta señales y carga los usuarios disponibles.
UsersAdminWidget::UsersAdminWidget(QSharedPointer<AppController> controller, QWidget *parent)
    : QWidget(parent), ui(new Ui::UsersAdminWidget), controller(controller) {
    ui->setupUi(this);

    userTableModel = new UiTableModel(this);
    editTableModel = new UiTableModel(this);

    ui->userTable->setModel(userTableModel);
    ui->editTable->setModel(editTableModel);

    connect(ui->userTable, &QTableView::clicked, this, &UsersAdminWidget::onUserSelected);
    connect(ui->saveButton, &QPushButton::clicked, this, &UsersAdminWidget::onSaveClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &UsersAdminWidget::onDeleteClicked);
    connect(ui->newButton, &QPushButton::clicked, this, &UsersAdminWidget::onNewClicked);

    connect(editTableModel, &QAbstractItemModel::dataChanged,this, &UsersAdminWidget::onEditTableDataChanged);


    loadUsers();
}

/// @brief Destructor.
UsersAdminWidget::~UsersAdminWidget() {
    delete ui;
}

/// @brief Carga todos los usuarios y los muestra en la tabla principal.
void UsersAdminWidget::loadUsers() {
    QList<QSharedPointer<User>> allUsers;
     allUsers = controller->getUserManager()->listUsers(UserType::Unknown);
    //allUsers += controller->getUserManager()->listUsers(UserType::Admin);
    //allUsers += controller->getUserManager()->listUsers(UserType::Trainer);
    //allUsers += controller->getUserManager()->listUsers(UserType::Client);

    QList<QList<QVariant>> rows;
    for (const auto& user : allUsers) {
        QList<QVariant> row;
        row << user->getId() << user->getUserName() << user->getEmail() << UserTypeToString(user->getUserType());
        rows << row;
    }

    userTableModel->setDataSet(rows, {"IdUser", "UserName", "Email", "UserType"});
}
/// @brief Maneja la selección de un usuario y carga sus datos en la tabla de edición.
void UsersAdminWidget::onUserSelected(const QModelIndex& index) {
    int row = index.row();
    if (row < 0 || row >= userTableModel->rowCount()) return;

    int id = userTableModel->getDataSet().at(row).at(0).toInt();
    selectedUserId = id;

    editUser= controller->getUserManager()->getUser(id);
    if (!editUser) return;
    populateEditTable(editUser);
}
/// @brief Llena la tabla de edición con los datos del usuario proporcionado.
void UsersAdminWidget::populateEditTable(QSharedPointer<User> user) {
    QList<QList<QVariant>> rows;
    rows << (QList<QVariant>() << "Type" << UserTypeToString(user->getUserType()));
    rows << (QList<QVariant>() << "IdUser" << user->getId());
    rows << (QList<QVariant>() << "UserName" << user->getUserName());
    rows << (QList<QVariant>() << "Email" << user->getEmail());
    rows << (QList<QVariant>() << "Password" << user->getPassword());
    rows << (QList<QVariant>() << "join_up_date" << user->getJoin_up_date().toString(Qt::ISODate));
    rows << (QList<QVariant>() << "last_login" << user->getLast_login().toString(Qt::ISODate));

    if (user->getUserType() == UserType::Admin) {
        auto admin = qSharedPointerCast<Admin>(user);
        rows << (QList<QVariant>() << "permissionLevel" << admin->getPermissionLevel());
        rows << (QList<QVariant>() << "roleDescription" << admin->getRoleDescription());
        rows << (QList<QVariant>() << "canManageUsers" << admin->canManage());
        rows << (QList<QVariant>() << "canModifyPlans" << admin->canModifyTrainingPlans());
    } else if (user->getUserType() == UserType::Trainer) {
        auto trainer = qSharedPointerCast<FitnessTrainer>(user);
        rows << (QList<QVariant>() << "Qualification" << trainer->getQualifications());
        rows << (QList<QVariant>() << "Specialty" << trainer->getSpecialtyArea());
        rows << (QList<QVariant>() << "ExperienceYears" << trainer->getYearsOfExperience());
        rows << (QList<QVariant>() << "Resume" << trainer->getResume());
    }

    editTableModel->setDataSet(rows, {"Field", "Value"});
    editTableModel->setColumnReadOnly(0);
}
/// @brief Extrae los datos modificados por el usuario desde la tabla de edición.
QHash<UserField, QVariant> UsersAdminWidget::collectEditTableData() {

    QHash<UserField, QVariant> data;
    auto rows = editTableModel->getDataSet();
    for (const auto& row : rows) {
        QString keyStr = row.at(0).toString();
        QVariant val = row.at(1);
        UserField field = UserFieldFromString(keyStr);
        if (field != UserField::Unknown) {
            data[field] = val;
        }
    }
    return data;
}

/// @brief Guarda los datos del usuario actualmente en edición, reemplazando su instancia según su tipo.

void UsersAdminWidget::onSaveClicked() {
    if (!editUser) return;

    QHash<UserField, QVariant> data = collectEditTableData();
   // qDebug(UserRepo) << "Contenido de userData:";
    for (UserField f:data.keys()) qDebug()<<UserFieldToString(f)<<":"<<data.value(f).toString();
    // editUser->setUserName(data[UserField::UserName].toString());
    // editUser->setEmail(data[UserField::Email].toString());
    // //editUser->changePassword(data[UserField::Pass].toString());
    // editUser->setJoin_up_date(QDateTime::fromString(data[UserField::join_up_date].toString(), Qt::ISODate));
    // //editUser->setLast_login(QDateTime::fromString(data[UserField::last_login].toString(), Qt::ISODate));
    // if (data

    // ciframos la contraseña si aún no está en formato hash
    if (data.contains(UserField::Pass)) {
        QString rawPass = data[UserField::Pass].toString();

        if (rawPass.length() < 50) {
            QString hashedPass = controller->getLoginManager()->hashPassword(rawPass);
            data[UserField::Pass] = hashedPass;
        }
    }

    if (editUser->getUserType() == UserType::Admin){
        editUser.clear();
        editUser = QSharedPointer<Admin>::create(data);}
    else if (editUser->getUserType() == UserType::Trainer){
        editUser.clear();
        editUser = QSharedPointer<FitnessTrainer>::create(data);}
    else{
        editUser.clear();
        editUser = QSharedPointer<Client>::create(data);}

     controller->getUserManager()->updateUser(editUser);
    // selectedUserId = editUser->getId();
    loadUsers();
}
/// @brief Elimina el usuario actualmente seleccionado del sistema.

void UsersAdminWidget::onDeleteClicked() {
    if (selectedUserId < 0) return;
    controller->getUserManager()->deleteUser(selectedUserId);
    selectedUserId = -1;
    clearEditTable();
    loadUsers();
}

// void UsersAdminWidget::onNewClicked() {

//     selectedUserId =controller->getUserManager()->getNextUserId();
//     //selectedUserId = -1;
//     clearEditTable();
//     QList<QList<QVariant>> baseFields = {
//         {"User Type", ""},
//         {"UserName", ""},
//         {"Email", ""},
//         {"Password", ""},
//         {"join_up_date", QDateTime::currentDateTime().toString(Qt::ISODate)},
//         {"last_login", QDateTime::currentDateTime().toString(Qt::ISODate)}
//     };
//     editTableModel->setDataSet(baseFields, {"Field", "Value"});
//     editTableModel->setColumnReadOnly(0);
// }
/// @brief Inicia el flujo de creación de un nuevo usuario, mostrando los campos básicos para edición.

void UsersAdminWidget::onNewClicked() {
    clearEditTable();
    selectedUserId = controller->getUserManager()->getNextUserId();  // nuevo ID

    QList<QVariant> userTypes = { "Admin", "Trainer", "Client" };

    QList<QList<QVariant>> baseFields = {
        { "IdUser", selectedUserId },
        { "User Type", "" },
    };

    editTableModel->setDataSet(baseFields, { "Field", "Value" });
    ui->editTable->setItemDelegateForRow(1, new ComboBoxDelegate(this, {"Admin", "Trainer", "Client"}));
    editTableModel->setColumnReadOnly(0);


}
/// @brief Reacciona a los cambios en la tabla de edición, en particular al cambiar el tipo de usuario.

void UsersAdminWidget::onEditTableDataChanged(const QModelIndex &m1, const QModelIndex &m2)
{
    if (m1.row() == 1 && m1.column() == 1) {
        QString selectedType = editTableModel->getDataSet()[1][1].toString();
        onUserTypeChanged(selectedType);
    }


}
/// @brief Limpia la tabla de edición.
void UsersAdminWidget::clearEditTable() {
    editTableModel->setDataSet({}, {"Field", "Value"});
}
/// @brief Cambia el tipo de usuario actual en edición y carga campos apropiados por tipo.
void UsersAdminWidget::onUserTypeChanged(const QString& typeStr) {
    UserType tipo = UserTypeFromString(typeStr);

    QHash<UserField, QVariant> dataMap;
    dataMap.insert(UserField::IdUser,selectedUserId);
    dataMap.insert(UserField::UserName,"introduzca el userName");
    dataMap.insert(UserField::Email,"introduzca el Email");
    dataMap.insert(UserField::Pass,"Introduzca el password por defecto");

    dataMap.insert(UserField::join_up_date, QDateTime::currentDateTime());
    dataMap.insert(UserField::last_login, QDateTime::currentDateTime());
    dataMap.insert(UserField::Plan, "Not assigned yet");

    if (tipo == UserType::Admin)
        editUser = QSharedPointer<Admin>::create(dataMap);
    else if (tipo == UserType::Trainer)
        editUser = QSharedPointer<FitnessTrainer>::create(dataMap);
    else
        editUser = QSharedPointer<Client>::create(dataMap);

    populateEditTable(editUser);
}
