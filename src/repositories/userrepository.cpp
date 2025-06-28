/**
 * @file userrepository.cpp
 * @brief Implementación de la clase UserRepository, encargada de la gestión de usuarios, perfiles y preferencias en base de datos.
 */

#include "userrepository.h"
#include <QDebug>
#include <QCoreApplication>
#include <QBuffer>
// Definimos una categoría para los logs
Q_LOGGING_CATEGORY(UserRepo, "UserRepo")
/**
 * @brief Constructor de la clase UserRepository.
 * @param dbManager Puntero al gestor de base de datos.
 * @param parent Objeto padre en la jerarquía de Qt.
 */
UserRepository::UserRepository(DBManager* dbManager, QObject* parent)
    : QObject(parent), db(dbManager) {}
/**
 * @brief Recupera un usuario a partir de su ID, instanciando la clase específica según el tipo de usuario.
 *
 * Este método consulta la tabla general de usuarios y, en función del tipo,
 * también recupera los datos de la tabla específica (Client, Trainer o Admin).
 * @param id ID del usuario a recuperar.
 * @return QSharedPointer al objeto User correspondiente, o nullptr si no se encuentra o no es válido.
 */
QSharedPointer<User> UserRepository::getUserById(int id) {

    if (id<0 ) return nullptr;


    qDebug(UserRepo) << " Buscando usuario con ID:" << id;

    QString idStr = QString::number(id);
    DBTable userTable = db->get(TypeBDEnum::User, DBFilter::IdUser, idStr);

    //qDebug(UserRepo) << " Datos del usuario ID:" << id;
    //userTable.print();

    //qDebug() << " Tabla de usuarios cargada con" << userTable.rowCount() << "filas";

    if (userTable.rowCount() == 0) return nullptr;
    if (!userTable.contains(BDFilterToString(DBFilter::IdUser),id)) return nullptr;

    QHash<UserField, QVariant> userData;
    for (const QString& col : userTable.getColumnsNames()) {
        userData.insert(UserFieldFromString(col), userTable.value(0, col));
    }

    UserType type = UserTypeFromString(userTable.value(0, "userType").toString().toLower());

    qDebug(UserRepo) << "Tipo de usuario detectado:" << UserTypeToString(type);

    //if (type == UserType::Unknown) return nullptr;

    TypeBDEnum tipoEntidad;
    if (type == UserType::Client) tipoEntidad = TypeBDEnum::Client;
    else if (type == UserType::Trainer) tipoEntidad = TypeBDEnum::Trainer;
    else if (type == UserType::Admin) tipoEntidad = TypeBDEnum::Admin;
    else return nullptr;

    DBTable specificTable = db->get(tipoEntidad, DBFilter::IdUser, QString::number(id));
    //qDebug(UserRepo) << "Datos Recuperados:";
    //userTable.print();
    //specificTable.print();

    if (specificTable.rowCount() == 0) return nullptr;

    for (const QString& col : specificTable.getColumnsNames()) {
        UserField field = UserFieldFromString(col);

        if (!userData.contains(field)) {
            userData.insert(field, specificTable.value(0, col));
        }
    }

    //qDebug(UserRepo) << "Contenido de userData:";
    //for (auto it = userData.constBegin(); it != userData.constEnd(); ++it) {
    //    qDebug() << "  " << UserFieldToString(it.key()) << ":" << it.value().toString();
    //}


    if (type == UserType::Client) {
        QSharedPointer<Client> client = QSharedPointer<Client>(new Client(userData));

        qDebug(UserRepo) << " Cliente construido. ¿Es válido? " << client->isValid();

        if (!client->isValid()) return nullptr;

        // Cargarmosad el workoutCalendar si existe
        DBTable calendar = db->get(TypeBDEnum::ClientWorkoutCalendar, DBFilter::IdClient, idStr);
        //qDebug(UserRepo) << " Tabla de calendario cargada con" << calendar.rowCount() << "filas";

        for (int i = 0; i < calendar.rowCount(); ++i) {
            //qDebug() << "Fila" << i << ":" << calendar.value(i, "date") << calendar.value(i, "idWorkout");

            QDateTime date = QDateTime::fromString(calendar.value(i, "date").toString(), Qt::ISODate);
            int idWorkout = calendar.value(i, "idWorkout").toInt();
            client->setWorkout(date, idWorkout);
        }
            qDebug(UserRepo) << " Cliente devuelto correctamente.";
        return client;
    }
    if (type == UserType::Trainer) {
         QSharedPointer<FitnessTrainer> trainer = QSharedPointer<FitnessTrainer>(new FitnessTrainer(userData));
        if (trainer->isValid()) return trainer;
        else return nullptr ;}

    if (type == UserType::Admin){
        QSharedPointer<Admin> admin = QSharedPointer<Admin>(new Admin(userData));
        if (admin->isValid()) return admin;
        else return nullptr ;}


    return nullptr;
}

/**
 * @brief Lista los usuarios de un tipo específico o todos si el tipo es Unknown.
 *
 * Devuelve una tabla con todos los registros de usuarios del tipo solicitado.
 * @param tipo Tipo de usuario a filtrar.
 * @return DBTable con los resultados de la consulta.
 */
DBTable UserRepository::listUsers(UserType tipo) {
    if (tipo == UserType::Unknown) return db->get(TypeBDEnum::User,DBFilter::none,"");
    QString tipoStr=UserTypeToString(tipo);
    return db->get(TypeBDEnumFromString(tipoStr),DBFilter::none,"");
}

/**
 * @brief Guarda o actualiza un usuario, incluyendo su imagen, datos generales y específicos según el rol.
 *
 * Si el usuario no tiene ID, se genera uno nuevo. Los datos se almacenan en la tabla general de usuarios
 * y luego en la tabla específica (clients, trainers o admins). También se guarda el perfil si es un cliente.
 * @param user Objeto de usuario a persistir.
 * @return true si la operación fue exitosa en todas las tablas necesarias.
 */
bool UserRepository::saveUser(QSharedPointer<User> user) {

    if (user==nullptr) return false;
    if(!user->isValid()) return false;
    bool result=true;


    int id = user->getId();
    //Si es un usuario nuevo, se obtiene el id de la base de datos
    if (id==-1){id=db->generateID(TypeBDEnum::User);
        //user->setId(id);
    };

    QString idStr = QString::number(id);
    UserType type = user->getUserType();

    if (type == UserType::Unknown) return false;

    // Guardamos datos comunes en tabla "users"
    DBTable userTabledb=db->get(TypeBDEnum::User);
    DBTable userTable("users");
    userTable.setColumns(userTabledb.getColumnsNames());
   // userTable.setColumns({"idUser", "userName", "email", "password", "userType", "join_up_date", "last_login"});
        QImage image;
    if (user->getProfile_Picture().isNull()) {
        QString path = QCoreApplication::applicationDirPath() + "/config/blank-profile.jpg";
        image.load(path);
        if (image.isNull()) {
            qWarning() << "No se pudo cargar ninguna imagen del perfil. Usando imagen en blanco.";
        }
    }

    QByteArray arrayImagen;
    QBuffer buffer(&arrayImagen);
    buffer.open(QIODevice::WriteOnly);
    user->getProfile_Picture().save(&buffer, "PNG");

    userTable.addRow({
        id,
        user->getUserName(),
        user->getEmail(),
        user->getPassword(),
        UserTypeToString(type),
        user->getJoin_up_date().toString(Qt::ISODate),
        user->getLast_login().toString(Qt::ISODate),
        arrayImagen
        //user->getProfile_Picture()
    });
    //qDebug(UserRepo)<<"Tabla generada";
    //userTable.print();
    db->save(TypeBDEnum::User, userTable);
    //db->get(TypeBDEnum::User).print();

    // Guardamos en tabla específica según tipo

        //qDebug(UserRepo) << "Hacemos el dinamic Cast según el tipo de usuario .";

    if (type == UserType::Client) {

        Client* clientPtr = dynamic_cast<Client*>(user.data());
         qDebug(UserRepo) << "Client:"<<clientPtr->getId()<<"tipo:"<< UserTypeToString(clientPtr->getUserType());
        if (!clientPtr) {
             qWarning(UserRepo) << "Ha fallado el dinamic Cast a la clase Cliente .";
            return false;}

        DBTable clientTable("clients");
        clientTable.setColumns({"idUser"});
        clientTable.addRow({id});
        result=db->save(TypeBDEnum::Client, clientTable, DBFilter::IdUser, idStr);

        //Se guarda el perfil del cliente
        if (clientPtr->getProfile()!=nullptr) saveClientProfile(id,clientPtr->getProfile());
        else  qWarning(UserRepo) << "El profile del Cliente es un nullpointer .";
    }
    else if (type == UserType::Trainer) {
        FitnessTrainer* trainerPtr = dynamic_cast<FitnessTrainer*>(user.data());
        if (!trainerPtr) {
            qWarning(UserRepo) << "Ha fallado el dinamic Cast a la clase Trainer .";
            return false;}



        DBTable trainerTable("trainers");
        trainerTable.setColumns({"idUser","idTrainer",
                                 UserFieldToString(UserField::Qualification),
                                 UserFieldToString(UserField::specialtyArea),
                                UserFieldToString(UserField::resume),
                                UserFieldToString(UserField::experienceYears)  });
        trainerTable.addRow({id,trainerPtr->getId(),
                             trainerPtr->getQualifications(),
                             trainerPtr->getSpecialtyArea(),
                                trainerPtr->getResume(),
                             trainerPtr->getYearsOfExperience()   });

        result=db->save(TypeBDEnum::Trainer, trainerTable, DBFilter::IdUser, idStr);
    }
    else if (type == UserType::Admin) {
        Admin* adminPtr = dynamic_cast<Admin*>(user.data());
        if (!adminPtr) {
            qWarning(UserRepo) << "Ha fallado el dinamic Cast a la clase Admin .";
            return false;}


        DBTable adminTable("admins");

         adminTable.setColumns({"idUser","idAdmin",
                                  UserFieldToString(UserField::canManageUsers),
                                  UserFieldToString(UserField::canModifyPlans),
                                  UserFieldToString(UserField::roleDescription),
                                  UserFieldToString(UserField::permissionLevel)  });
         adminTable.addRow({id,adminPtr->getId(),
                            adminPtr->canManage(),
                            adminPtr->canManage(),
                              adminPtr->getRoleDescription(),
                            adminPtr->getPermissionLevel()   });

       result=db->save(TypeBDEnum::Admin, adminTable, DBFilter::IdUser, idStr);
    }
    return result;
}

/**
 * @brief Elimina un usuario y todos los registros relacionados de las distintas tablas de perfil.
 *
 * Elimina registros de la tabla de usuarios, tabla específica por tipo y perfiles del cliente si aplican.
 * @param id ID del usuario a eliminar.
 * @return true si todas las eliminaciones fueron exitosas, false si alguna falla.
 */
bool UserRepository::deleteUser(int id) {

    if (id<0) {return false;}
    QString idStr = QString::number(id);

    QList<QPair<TypeBDEnum, DBFilter>> filtros = {
        {TypeBDEnum::User, DBFilter::IdUser},
        {TypeBDEnum::Client, DBFilter::IdUser},
        {TypeBDEnum::Trainer, DBFilter::IdUser},
        {TypeBDEnum::Admin, DBFilter::IdUser},
        {TypeBDEnum::ProfileFunctional, DBFilter::IdClient},
        {TypeBDEnum::ProfileMorphology, DBFilter::IdClient},
        {TypeBDEnum::ProfileROM, DBFilter::IdClient},
        {TypeBDEnum::ProfileLifestyle, DBFilter::IdClient},
        {TypeBDEnum::ProfileMedical, DBFilter::IdClient},
         {TypeBDEnum::ProfileGeneral, DBFilter::IdClient}
    };
    bool check=true;
    for (const auto& filtro : filtros) {
        //esto es para FakeBD
        //TypeBDEnum tipoTabla = filtro.first;
        //QString campo = BDFilterToString(filtro.second);

        //DBTable tabla = db->get(tipoTabla);
       // if (!tabla.getColumnsNames().contains(campo, Qt::CaseInsensitive)) continue;

       // if (!tabla.contains(campo, idStr)) continue;
        //
        bool checktb=db->deleteRow(filtro.first, filtro.second, idStr);
        if (!checktb) {
            check=checktb;
            qWarning(UserRepo) << "No se pudo eliminar en tabla tipo"<< TypeBDEnumToString(filtro.first)
            << "con IdUser:" << idStr;
        }
    }
    return check;
}

/**
 * @brief Comprueba si un nombre de usuario está disponible o es válido.
 *
 * Actualmente devuelve siempre 1 (pendiente de implementación completa).
 * @param newUserName Nombre de usuario a comprobar.
 * @return 1 si se considera válido (por ahora).
 */

int UserRepository::checkUserName(QString newUserName) const
{
    return 1;
}

/**
 * @brief Recupera todos los datos de perfil físico de un cliente (general, funcional, morfológico, ROM, estilo de vida, médico y adicional).
 *
 * Cada sección se consulta por separado y se almacena en un único objeto `ClientProfile`.
 * @param idClient ID del cliente.
 * @return QSharedPointer al objeto ClientProfile completamente poblado.
 */
QSharedPointer<ClientProfile> UserRepository::getClientProfile(int idClient) {

    QSharedPointer<ClientProfile> profile(new ClientProfile);
    profile->setIdClient(idClient);

    QString idStr = QString::number(idClient);

    // General
    DBTable general = db->get(TypeBDEnum::ProfileGeneral, DBFilter::IdClient, idStr);
    //qDebug(UserRepo)<<"  UserRepository";
    //general.print();
    for (const QString& col : general.getColumnsNames()) {

        if (col != "idClient") {
             GeneralDataField field = GeneralDataFieldFromString(col);
            if (field !=  GeneralDataField::Unknown)

                profile->setRecord(field, general.value(0,col));
           // qDebug(UserRepo)<<"  UserRepository col:"<<col<<"valor:"<<general.value(0,col);
        }
    }

    // Functional
    DBTable functional = db->get(TypeBDEnum::ProfileFunctional, DBFilter::IdClient, idStr);

    for (const QString& col : functional.getColumnsNames()) {
        if (col != "idClient") {
             FunctionalField field = FunctionalFieldFromString(col);

            if (field != FunctionalField::Unknown)
                profile->setRecord(field, functional.value(0, col));
        }
    }

    // Morphological
    DBTable morph = db->get(TypeBDEnum::ProfileMorphology, DBFilter::IdClient, idStr);
    for (const QString& col : morph.getColumnsNames()) {
         if (col != "idClient") {
            MorfologicalFields field = MorfologicalFieldsFromString(col);
             if (field != MorfologicalFields::Unknown)
                profile->setRecord(field, morph.value(0, col));
        }
    }

    // ROM
    DBTable rom = db->get(TypeBDEnum::ProfileROM, DBFilter::IdClient, idStr);
    for (const QString& col : rom.getColumnsNames()) {

        if (col != "idClient") {
            ROMJoint joint = ROMJointFromString(col);
            if (joint != ROMJoint::Unknown)
                profile->setRecord(joint, rom.value(0, col));
        }
    }

    // Lifestyle
    DBTable life = db->get(TypeBDEnum::ProfileLifestyle, DBFilter::IdClient, idStr);
    for (const QString& col : life.getColumnsNames()) {
        if (col != "idClient") {
            LifeStyleFields field = LifeStyleFieldsFromString(col);
            if (field != LifeStyleFields::Unknown)
                  profile->setRecord(field, life.value(0, col));
        }
    }

    // Medical
    DBTable medical = db->get(TypeBDEnum::ProfileMedical, DBFilter::IdClient, idStr);
    for (const QString& col : medical.getColumnsNames()) {

        if (col != "idClient") {
              MedicalRecordsFields field = MedicalRecordsFieldsFromString(col);
            if (field != MedicalRecordsFields::Unknown)
                profile->setRecord(field, medical.value(0, col));
        }
    }

    // Additional
    DBTable additional = db->get(TypeBDEnum::ProfileAditional, DBFilter::IdClient, idStr);
    for (const QString& col : additional.getColumnsNames()) {
        if (col != "idClient")

            profile->setRecord(col, additional.value(0, col));
    }
    qDebug(UserRepo)<<"Obtenido el Profile del cliente: "<< idClient;
    return profile;
}


/**
 * @brief Construye una tabla DBTable para almacenar datos de perfil a partir de un QVariantMap.
 *
 * El método convierte los datos clave-valor en columnas y filas para una tabla específica de perfil.
 * @param idClient ID del cliente asociado.
 * @param data Mapa de valores a incluir.
 * @param tableType Tipo de tabla (según enum TypeBDEnum) para establecer el nombre.
 * @return DBTable lista para ser almacenada.
 */
DBTable UserRepository::buildDBTable(int idClient, const QVariantMap& data, TypeBDEnum tableType)
{

    QString tableName = TypeBDEnumToString(tableType);
    DBTable table(tableName);

    QStringList columnNames;
    QList<QVariant> rowValues;

    columnNames.append("idClient");
    rowValues.append(idClient);

    for (auto it = data.begin(); it != data.end(); ++it) {
        columnNames.append(it.key());
        rowValues.append( it.value());
    }

    table.setColumns(columnNames);
    table.addRow(rowValues);

    return table;
}


/**
 * @brief Guarda todos los componentes del perfil de un cliente en sus respectivas tablas.
 *
 * El perfil incluye datos generales, funcionales, morfológicos, ROM, estilo de vida, médicos y adicionales.
 * Cada sección se guarda en su tabla correspondiente usando `buildDBTable`.
 * @param id ID del cliente.
 * @param profile Objeto con todos los datos de perfil a almacenar.
 * @return true si todas las operaciones de guardado fueron exitosas.
 */

bool UserRepository::saveClientProfile(int id,QSharedPointer<ClientProfile> profile) {
    bool saved = true;
    //const int id = profile->getIdClient();
    const QString idStr = QString::number(id);

    // General data s
    DBTable temp = buildDBTable(id, profile->getAllRecords(ClientProfileData::General), TypeBDEnum::ProfileGeneral);
    saved = db->save(TypeBDEnum::ProfileGeneral, temp, DBFilter::IdClient, idStr);

    // Functional
    temp = buildDBTable(id, profile->getAllRecords(ClientProfileData::Functional), TypeBDEnum::ProfileFunctional);
    saved = db->save(TypeBDEnum::ProfileFunctional, temp, DBFilter::IdClient, idStr);

    // Morphological
    temp = buildDBTable(id, profile->getAllRecords(ClientProfileData::Morfological), TypeBDEnum::ProfileMorphology);
    saved = db->save(TypeBDEnum::ProfileMorphology, temp, DBFilter::IdClient, idStr);

    // ROM
    temp = buildDBTable(id, profile->getAllRecords(ClientProfileData::ROM), TypeBDEnum::ProfileROM);
    saved = db->save(TypeBDEnum::ProfileROM, temp, DBFilter::IdClient, idStr);

    // Lifestyle
    temp = buildDBTable(id, profile->getAllRecords(ClientProfileData::LifeStyle), TypeBDEnum::ProfileLifestyle);
    saved = db->save(TypeBDEnum::ProfileLifestyle, temp, DBFilter::IdClient, idStr);

    // Medical
    temp = buildDBTable(id, profile->getAllRecords(ClientProfileData::Medical), TypeBDEnum::ProfileMedical);
    saved = db->save(TypeBDEnum::ProfileMedical, temp, DBFilter::IdClient, idStr);

    // Additional
    temp = buildDBTable(id, profile->getAllRecords(ClientProfileData::Additional), TypeBDEnum::ProfileAditional);
    saved = db->save(TypeBDEnum::ProfileAditional, temp, DBFilter::IdClient, idStr);
    qDebug(UserRepo)<<"Registrado el Profile del cliente: "<< idStr;
    return saved;
}


/**
 * @brief Recupera las preferencias del usuario desde la tabla correspondiente.
 *
 * Incluye parámetros como modo dual, sonidos de alerta/información, muteo y configuración de conexiones.
 * @param idUser ID del usuario.
 * @return Estructura `UserPreferences` con los valores cargados desde base de datos.
 */
UserPreferences UserRepository::getUserPreferences(int idUser) {
    DBTable table = db->get(TypeBDEnum::Preferences, DBFilter::IdUser, QString::number(idUser));
    if (table.rowCount() == 0) return UserPreferences();  // vacío por defecto

    UserPreferences prefs;
    prefs.setIdUser(idUser);
    prefs.setDualMode(table.value(0, "dualMode").toString() == "true");
    prefs.setInfoSound(table.value(0, "infoSound").toString() == "true");
    prefs.setAlertSound(table.value(0, "alertSound").toString() == "true");
    prefs.setMute(table.value(0, "mute").toString() == "true");
    prefs.setConnectionsJson(table.value(0, "connectionsJson").toString());

    return prefs;
}

/**
 * @brief Guarda las preferencias de usuario en la base de datos.
 *
 * Convierte el contenido de `UserPreferences` en una fila de tabla y la persiste mediante `DBManager`.
 * @param prefs Estructura con las preferencias a almacenar.
 * @return true si el guardado fue exitoso.
 */
bool UserRepository::saveUserPreferences( UserPreferences prefs) {
    DBTable table("userpreferences");
    table.setColumns({ "idUser", "dualMode", "infoSound", "alertSound", "mute", "connectionsJson" });

    table.addRow({
        prefs.getIdUser(),
        prefs.getDualMode() ? "true" : "false",
        prefs.getInfoSound() ? "true" : "false",
        prefs.getAlertSound() ? "true" : "false",
        prefs.getMute() ? "true" : "false",
        prefs.getConnectionsJson()
    });

    return db->save(TypeBDEnum::Preferences, table, DBFilter::IdUser, QString::number(prefs.getIdUser()));
}
/**
 * @brief Genera el siguiente ID único disponible para un nuevo usuario.
 *
 * Llama a la función de `DBManager` para obtener el próximo ID para la tabla de usuarios.
 * @return Entero representando el nuevo ID.
 */
int UserRepository::getNextUserId() {
    return db->generateID(TypeBDEnum::User);
}

