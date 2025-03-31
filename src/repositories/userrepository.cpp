#include "userrepository.h"
#include <QDebug>



UserRepository::UserRepository(DBManager* dbManager, QObject* parent)
    : QObject(parent), db(dbManager) {}

QPointer<User> UserRepository::getUserById(int id) {
    DBTable userTable = db->get(TypeBDEnum::User, DBFilter::IdUser, QString::number(id));
    if (userTable.rowCount() == 0) return nullptr;

    QMap<UserField, QVariant> userData;
    for (const QString& col : userTable.getColumnsNames()) {
        userData.insert(UserFieldFromString(col), userTable.value(0, col));
    }

    UserType type = UserTypeFromString(userTable.value(0, "userType").toString().toLower());
    if (type == UserType::Unknown) return nullptr;

    TypeBDEnum tipoEntidad;
    if (type == UserType::Client) tipoEntidad = TypeBDEnum::Client;
    else if (type == UserType::Trainer) tipoEntidad = TypeBDEnum::Trainer;
    else if (type == UserType::Admin) tipoEntidad = TypeBDEnum::Admin;
    else return nullptr;

    DBTable specificTable = db->get(tipoEntidad, DBFilter::IdUser, QString::number(id));
    if (specificTable.rowCount() == 0) return nullptr;

    for (const QString& col : specificTable.getColumnsNames()) {
        UserField field = UserFieldFromString(col);
        if (!userData.contains(field)) {
            userData.insert(field, specificTable.value(0, col));
        }
    }

    if (type == UserType::Client) return QPointer<User>(new Client(userData));
    if (type == UserType::Trainer) return QPointer<User>(new FitnessTrainer(userData));
    if (type == UserType::Admin) return QPointer<User>(new Admin(userData));

    return nullptr;
}


DBTable UserRepository::listUsers(UserType tipo) {
    if (tipo == UserType::Unknown) return DBTable("no_Data");

    return db->get(TypeBDEnumFromString(UserTypeToString(tipo)));
}


void UserRepository::saveUser(const User& user) {
    const int id = user.getId();
    const QString idStr = QString::number(id);
    const UserType type = user.getUserType();

    // Guardar datos comunes en tabla "users"
    DBTable userTable("users");
    userTable.setColumns({"idUser", "userName", "email", "password", "userType", "join_up_date", "last_login"});

    userTable.addRow({
        id,
        user.getUserName(),
        user.getEmail(),
        "*****",
        UserTypeToString(type),
        user.getJoin_up_date().toString(Qt::ISODate),
        user.getLast_login().toString(Qt::ISODate)
    });

    db->save(TypeBDEnum::User, userTable, DBFilter::IdUser, idStr);

    // Guardar en tabla específica según tipo
    if (type == UserType::Client) {
        const Client* client = dynamic_cast<const Client*>(&user);
        if (!client) return;

        DBTable clientTable("clients");
        clientTable.setColumns({"idUser"});
        clientTable.addRow({id});
        db->save(TypeBDEnum::Client, clientTable, DBFilter::IdUser, idStr);

        // Guardar perfil del cliente
        saveClientProfile(client->getProfile());
    }
    else if (type == UserType::Trainer) {
        const FitnessTrainer* trainer = dynamic_cast<const FitnessTrainer*>(&user);
        if (!trainer) return;

        DBTable trainerTable("trainers");
        trainerTable.setColumns({"idUser"});
        trainerTable.addRow({id});
        db->save(TypeBDEnum::Trainer, trainerTable, DBFilter::IdUser, idStr);
    }
    else if (type == UserType::Admin) {
        const Admin* admin = dynamic_cast<const Admin*>(&user);
        if (!admin) return;

        DBTable adminTable("admins");
        adminTable.setColumns({"idUser"});
        adminTable.addRow({id});
        db->save(TypeBDEnum::Admin, adminTable, DBFilter::IdUser, idStr);
    }
}


void UserRepository::deleteUser(int id) {

    QString idStr = QString::number(id);

    QList<QPair<TypeBDEnum, DBFilter>> filtros = {
        {TypeBDEnum::User, DBFilter::IdUser},
        {TypeBDEnum::Client, DBFilter::IdUser},
        {TypeBDEnum::Trainer, DBFilter::IdUser},
        {TypeBDEnum::Admin, DBFilter::IdUser},
        {TypeBDEnum::ProfileFunctional, DBFilter::IdUser},
        {TypeBDEnum::ProfileMorphology, DBFilter::IdUser},
        {TypeBDEnum::ProfileROM, DBFilter::IdUser},
        {TypeBDEnum::ProfileLifestyle, DBFilter::IdUser},
        {TypeBDEnum::ProfileMedical, DBFilter::IdUser},
         {TypeBDEnum::ProfileGeneral, DBFilter::IdUser}
    };

    for (const auto& filtro : filtros) {
        if (!db->deleteRow(filtro.first, filtro.second, idStr)) {
            qWarning() << "No se pudo eliminar en tabla tipo"
                       << TypeBDEnumToString(filtro.first)
                       << "con IdUser:" << idStr;
        }
    }
}



int UserRepository::checkUserName(QString newUserName) const
{

}


QPointer<ClientProfile> UserRepository::getClientProfile(int idClient) {

    QPointer<ClientProfile> profile=new ClientProfile();

    profile->setIdClient(idClient);
    QString idStr = QString::number(idClient);

    // General
    DBTable general = db->get(TypeBDEnum::ProfileGeneral, DBFilter::IdUser,idStr);

    for (const QString& col : general.getColumnsNames())
        if (col != "idClient" && GeneralDataFieldFromString(col)!=GeneralDataField::Unknown)

            profile->setGeneralRecord(GeneralDataFieldFromString(col), general.value(0, col));

    // Functional
    DBTable functional = db->get(TypeBDEnum::ProfileFunctional, DBFilter::IdUser,idStr);
    for (const QString& col : functional.getColumnsNames())
        if (col != "idClient" && FunctionalFieldFromString(col)!=FunctionalField::Unknown)

                profile->setFunctionalRecord(FunctionalFieldFromString(col), functional.value(0, col));

    // Morphological
    DBTable morph = db->get(TypeBDEnum::ProfileMorphology, DBFilter::IdUser,idStr);
    for (const QString& col : morph.getColumnsNames())
        if (col != "idClient" && MorfologicalFieldsFromString(col)!=MorfologicalFields::Unknown)
            profile->setMorfologicalRecord(MorfologicalFieldsFromString(col), morph.value(0, col).toDouble());

    // ROM
    DBTable rom = db->get(TypeBDEnum::ProfileROM, DBFilter::IdUser,idStr);
    for (const QString& col : rom.getColumnsNames())
        if (col != "idClient" && ROMJointFromString(col)!=ROMJoint::Unknown)
            profile->setROM(ROMJointFromString(col), rom.value(0, col).toDouble());

    // Lifestyle
    DBTable life = db->get(TypeBDEnum::ProfileLifestyle, DBFilter::IdUser,idStr);
    for (const QString& col : life.getColumnsNames())
        if (col != "idClient" && LifeStyleFieldsFromString(col)!=LifeStyleFields::Unknown)
            profile->setLifestyleRecord(LifeStyleFieldsFromString(col), life.value(0, col).toString());

    // Medical
    DBTable medical = db->get(TypeBDEnum::ProfileMedical, DBFilter::IdUser,idStr);
    for (const QString& col : medical.getColumnsNames())
        if (col != "idClient" && MedicalRecordsFieldsFromString(col)!=MedicalRecordsFields::Unknown)
            profile->addMedicalRecord(MedicalRecordsFieldsFromString(col), medical.value(0, col).toString());

    return profile;
}

template<typename K, typename V>

DBTable buildDBTable(int idClient, const QMap<K, V>& data, const TypeBDEnum tipo) {
    DBTable table(TypeBDEnumToString(tipo));

    QStringList columns = {"idClient"};
    QList<QVariant> row = {idClient};

    for (auto it = data.begin(); it != data.end(); ++it) {
        QString columnName;

        switch (tipo) {
        case TypeBDEnum::ProfileGeneral:
            columnName = GeneralDataFieldToString(static_cast<GeneralDataField>(it.key()));
            break;
        case TypeBDEnum::ProfileFunctional:
            columnName = FunctionalFieldToString(static_cast<FunctionalField>(it.key()));
            break;
        case TypeBDEnum::ProfileMorphology:
            columnName = MorfologicalFieldsToString(static_cast<MorfologicalFields>(it.key()));
            break;
        case TypeBDEnum::ProfileROM:
            columnName = ROMJointToString(static_cast<ROMJoint>(it.key()));
            break;
        case TypeBDEnum::ProfileMedical:
            columnName = MedicalRecordsFieldsToString(static_cast<MedicalRecordsFields>(it.key()));
            break;
        case TypeBDEnum::ProfileLifestyle:
            columnName = LifeStyleFieldsToString(static_cast<LifeStyleFields>(it.key()));
            break;
        case TypeBDEnum::ProfileAditional:
            columnName = QVariant::fromValue(it.key()).toString();
            break;
        default:
           columnName = QString::number(static_cast<int>(it.key()));
            break;
        }

        columns << columnName;
        row << it.value();
    }

    table.setColumns(columns);
    table.addRow(row);
    return table;
}




void UserRepository::saveClientProfile(const ClientProfile& profile) {
    const int id = profile.getIdClient();
    const QString idStr = QString::number(id);

    // General
    DBTable tempTable= buildDBTable(id, profile.getFunctionalRecords(), TypeBDEnum::ProfileGeneral);
    db->save(TypeBDEnum::ProfileGeneral,tempTable,DBFilter::IdUser, idStr);

    // Functional
    tempTable= buildDBTable(id, profile.getFunctionalRecords(), TypeBDEnum::ProfileFunctional);
    db->save(TypeBDEnum::ProfileFunctional,tempTable,DBFilter::IdUser, idStr);

    // Morphological
    tempTable=  buildDBTable(id, profile.getMorfologicalRecords(), TypeBDEnum::ProfileMorphology);
    db->save(TypeBDEnum::ProfileMorphology,tempTable, DBFilter::IdUser, idStr);

    // ROM
    tempTable=  buildDBTable(id, profile.getRangeOfMovements(), TypeBDEnum::ProfileROM);
    db->save(TypeBDEnum::ProfileROM,tempTable,DBFilter::IdUser, idStr);

    // Lifestyle
    tempTable=  buildDBTable(id, profile.getLifeStyleRecords(), TypeBDEnum::ProfileLifestyle);
    db->save(TypeBDEnum::ProfileLifestyle,tempTable,DBFilter::IdUser, idStr);

    // Medical
    tempTable=  buildDBTable(id, profile.getMedicalRecords(), TypeBDEnum::ProfileMedical);
    db->save(TypeBDEnum::ProfileMedical,tempTable,DBFilter::IdUser, idStr);

    // Aditional
    tempTable=  buildDBTable(id, profile.getMedicalRecords(), TypeBDEnum::ProfileAditional);
    db->save(TypeBDEnum::ProfileAditional,tempTable,DBFilter::IdUser, idStr);
}



