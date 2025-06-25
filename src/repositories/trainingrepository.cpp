/*!
 * \file trainingrepository.cpp
 * \brief Implementación de la clase TrainingRepository.
 *
 * Esta clase gestiona la lógica de acceso a la base de datos para los datos de entrenamiento,
 * incluyendo especificaciones de ejercicios, workouts completos, sesiones realizadas y sus reportes asociados.
 */
#include "trainingrepository.h"
#include "enums/ExEspecFieldEnum.h"
#include "enums/WorkoutFieldEnum.h"
#include <QString>



Q_LOGGING_CATEGORY(TrainingRepo, "TrainingRepo")

/*!
 * \brief Constructor de TrainingRepository.
 * \param dbManager Instancia del manejador de base de datos.
 * \param parent Objeto padre en la jerarquía Qt.
 */
TrainingRepository::TrainingRepository(DBManager* dbManager, QObject* parent)
    : QObject(parent),
    dbManager(dbManager) {}


/*!
 * \brief Obtiene una especificación de ejercicio a partir de su ID.
 * \param id Identificador del ejercicio.
 * \return Puntero compartido a `ExerciseEspec` o `nullptr` si no se encuentra.
 */
QSharedPointer<ExerciseEspec> TrainingRepository::getEspec(int id) {

    if (id < 0) return nullptr;

    QString idstr = QString::number(id);
    DBTable table = dbManager->get(TypeBDEnum::ExerciseEspec, DBFilter::IdEx, idstr);
    if (table.rowCount() == 0) {
        QString error=QString("No existe ese ejercicio con id : %1 en la base de datos").arg(idstr);
        qWarning(TrainingRepo) << error;
        emit ui_errorMessage(error,QtMsgType::QtWarningMsg);
        return nullptr;}

    QHash<ExEspecField, QVariant> data;
    for (int col = 0; col < table.columnCount(); ++col) {
        QString colName = table.getColumnName(col);
        ExEspecField field = ExEspecFieldFromString(colName);
        if (field != ExEspecField::Unknown) data[field] = table.value(0, colName);
    }

    QSharedPointer<ExerciseEspec> espec(new ExerciseEspec(data));
    espec->setIdEx(id);

    // Cargar states
    DBTable statesTable = dbManager->get(TypeBDEnum::State, {{DBFilter::IdEx, id}});
    for (int row = 0; row < statesTable.rowCount(); ++row) {
        int idState = statesTable.value(row, "idState").toInt();
        int minT = statesTable.value(row, "minTime").toInt();
        int maxT = statesTable.value(row, "maxTime").toInt();
        int order = statesTable.value(row, "orderInList").toInt();
        QString name=statesTable.value(row, "name").toString();
        State state(idState, id, minT, maxT,name);

        // Cargar constraints para ese state
        DBTable acTable = dbManager->get(TypeBDEnum::AngleConstraint, {{DBFilter::IdState, idState}, {DBFilter::IdEx, id}});
        for (int j = 0; j < acTable.rowCount(); ++j) {
            QString line = acTable.value(j, "line").toString();
            AngleConstraint c(
                acTable.value(j, "idConstraint").toInt(),
                idState,
                id,
                acTable.value(j, "minAngle").toDouble(),
                acTable.value(j, "maxAngle").toDouble(),
                acTable.value(j, "maxSafeAngle").toDouble(),
                acTable.value(j, "minSafeAngle").toDouble(),
                acTable.value(j, "fastThreshold").toInt(),
                acTable.value(j, "slowThreshold").toInt(),
                acTable.value(j, "symetricalAngle").toDouble(),
                directionFromString(acTable.value(j, "evolution").toString()),
                acTable.value(j, "toler").toDouble(),
                line,
                PoseViewFromString(acTable.value(j,"view").toString())
                );
            //c.setLinea(line);
            state.addAngleConstraint(line, c);
            //qDebug(TrainingRepo)<<"Obtenida line: "<<line<<"para el estado:"<<idState;
        }
        espec->addState(state);
        qDebug(TrainingRepo)<<"Se ha creado un nuevo estado para el ejercicio: "<<id;
    }

    // Cargar transiciones
    DBTable transitions = dbManager->get(TypeBDEnum::Transition, {{DBFilter::IdEx, id}});
    for (int row = 0; row < transitions.rowCount(); ++row) {
        int from = transitions.value(row, "fromState").toInt();
        int to = transitions.value(row, "toState").toInt();
        QString type = transitions.value(row, "conditionType").toString();
        QString line = transitions.value(row, "keypointLine").toString();
        PoseView   view=PoseViewFromString(transitions.value(row, "view").toString());
        Condition cond(conditionTypeFromString(type), line,0,view);
        espec->addTransition(qMakePair(from, to), cond);
    }

    return espec;
}




bool compareByOrder(const QPair<int, int>& a, const QPair<int, int>& b) {
    return a.first < b.first;
}
/*!
 * \brief Recupera un entrenamiento completo desde la base de datos.
 * \param id Identificador del workout.
 * \return Puntero a `TrainingWorkout` o `nullptr` si no existe.
 */
QSharedPointer<TrainingWorkout> TrainingRepository::getWorkout(int idWorkout) {

    if (idWorkout < 0) {
        qWarning(TrainingRepo) << "getWorkout: idWorkout inválido:" << idWorkout;
        return nullptr;
    }

    DBTable table = dbManager->get(TypeBDEnum::TrainingWorkout, DBFilter::IdWorkout, QString::number(idWorkout));

    if (table.rowCount() == 0) {
        QString error=QString("No existe un workout con id : %1 en la base de datos").arg(idWorkout);
        qWarning(TrainingRepo) << error;
        emit ui_errorMessage(error,QtMsgType::QtWarningMsg);
        return nullptr;}

    QHash<WorkoutField, QVariant> data;
    for (int col = 0; col < table.columnCount(); ++col) {
        QString colName = table.getColumnName(col);
        WorkoutField field = WorkoutFieldFromString(colName);
        if (field != WorkoutField::Unknown) {
            data[field] = table.value(0, colName);
        }
    }

    QSharedPointer<TrainingWorkout> workout(new TrainingWorkout(data));
    workout->setIdWorkout(idWorkout);

    DBTable workoutLinks = dbManager->get(TypeBDEnum::WorkoutExercises, DBFilter::IdWorkout, QString::number(idWorkout));
    if (workoutLinks.rowCount() == 0) return workout;

    QList<QPair<int, int>> exerciseOrder;
    for (int i = 0; i < workoutLinks.rowCount(); ++i) {

        int idEx = workoutLinks.value(i, "idEx").toInt();
        int order = workoutLinks.value(i, "exerciseOrder").toInt();
        exerciseOrder.append(qMakePair(order, idEx));
    }

    std::sort(exerciseOrder.begin(), exerciseOrder.end(), compareByOrder);
    for (const auto& pair : exerciseOrder) {
        QSharedPointer<ExerciseEspec> espec = getEspec(pair.second);
        if (espec) {
            workout->addExercise(espec, pair.first);
        }
    }

    return workout;
}


/*!
 * \brief Elimina un workout completo incluyendo sus ejercicios.
 */

bool TrainingRepository::deleteWorkoutfull(int idWorkout) {
    QString strId = QString::number(idWorkout);

    // Obtener ids de ejercicios del workout
    DBTable rel = dbManager->get(TypeBDEnum::WorkoutExercises, DBFilter::IdWorkout, strId);
    QStringList idExs;
    for (int i = 0; i < rel.rowCount(); ++i)
        idExs.append(rel.value(i, "idEx").toString());


    for (const QString& idExStr : idExs) {
        int idEx = idExStr.toInt();
        if (!deleteEspec(idEx)) {
            qWarning() << " Error al eliminar ExerciseEspec con idEx=" << idEx;
            QString error=QString("No se ha podido eliminar el ejercicio con id : %1 de la base de datos").arg(idEx);
            emit ui_errorMessage(error,QtMsgType::QtWarningMsg);

            return false;
        }
    }

    // Eliminamos  la relación workout-ejercicio y el propio workout
    if (!dbManager->deleteRow(TypeBDEnum::WorkoutExercises, DBFilter::IdWorkout, strId))
        return false;

    return dbManager->deleteRow(TypeBDEnum::TrainingWorkout, DBFilter::IdWorkout, strId);
}

/*!
 * \brief Guarda una especificación de ejercicio y todos sus componentes en la base de datos.
 * \param espec Puntero a la especificación.
 * \return ID de ejercicio guardado o -1 si falla.
 */

int TrainingRepository::saveEspec(QSharedPointer<ExerciseEspec> espec)
{
    if (espec == nullptr) return -1;
    int idEx = espec->getIdEx();

    // Si ya existe, eliminar completamente
    if (idEx != -1) {
        if (!deleteEspec(idEx)) return -1;
    } else {
        idEx = dbManager->generateID(TypeBDEnum::ExerciseEspec);
        espec->setIdEx(idEx);
    }

     qDebug(TrainingRepo)<<"saveEspec:Guardando Ejercicio con id:"<<idEx;
    // Guardar datos básicos del ejercicio
    DBTable especTable = toDBTable_ExerciseEspec(espec);
     if (especTable.rowCount() > 0) {
         if (!dbManager->save(TypeBDEnum::ExerciseEspec, especTable)) {

             QString error=QString("error guardando el ejercicio con id : %1 en la base de datos").arg(idEx);
            qWarning(TrainingRepo) << error;
             emit ui_errorMessage(error,QtMsgType::QtWarningMsg);
             return -1;
         }
     }
    // Guardar estados
    const QList<State> stateList = espec->getStatesList();
    DBTable stateTable("State");
    stateTable.setColumns(dbManager->get(TypeBDEnum::State).getColumnsNames());

    for (int i = 0; i < stateList.size(); ++i) {
        State s = stateList[i];
        if (s.getId() < 0) {
            qWarning(TrainingRepo) << "saveEspec:State inválido con id < 0, se ignora.";
            continue;
        }
        //int idState = dbManager->generateID(TypeBDEnum::State);
        //s.setId(idState);
        stateTable.addRow({s.getId(), idEx, s.getMinTime(), s.getMaxTime(), i,s.getName()});
        //espec->updateState(s);
        qDebug(TrainingRepo)<<"saveEspec:Guardando estado con id:"<<s.getId();
    }
    if (stateTable.rowCount() > 0) {
        if (!dbManager->save(TypeBDEnum::State, stateTable)) {

            QString error=QString("error guardando el ejercicio con id : %1 en la base de datos").arg(idEx);
            qWarning(TrainingRepo) << error;
            emit ui_errorMessage(error,QtMsgType::QtWarningMsg);
            return -1;
        }
    }
    // Guardar constraints
    DBTable acTable("AngleConstraint");
    acTable.setColumns(dbManager->get(TypeBDEnum::AngleConstraint).getColumnsNames());

    for (const State& s : espec->getStatesList()) {
        for (const QString& line : s.getConstraints().keys()) {
            AngleConstraint c = s.getConstraints().value(line);
            //int idConstraint = dbManager->generateID(TypeBDEnum::AngleConstraint);

            qDebug(TrainingRepo) << "saveEspec:Guardando constraint con id"<< c.getIdConstraint();

            acTable.addRow({
                c.getIdConstraint(),
                s.getId(),
                idEx,
                line,
                c.getMinAngle(),
                c.getMaxAngle(),
                c.getMaxSafeAngle(),
                c.getMinSafeAngle(),
                c.getFastThreshold(),
                c.getSlowThreshold(),
                c.getSymetricalAngle(),
                directionToString(c.getEvolution()),
                c.getToler(),
                PoseViewToString(c.getView())
            });
        }
    }
    if (acTable.rowCount() > 0) {
        if (!dbManager->save(TypeBDEnum::AngleConstraint, acTable)) {

            QString error=QString("error guardando el ejercicio con id : %1 en la base de datos").arg(idEx);
            qWarning(TrainingRepo) << error;
            emit ui_errorMessage(error,QtMsgType::QtWarningMsg);
            return -1;
        }
    }
    // Guardar transiciones
    DBTable trTable("Transition");
    trTable.setColumns(dbManager->get(TypeBDEnum::Transition).getColumnsNames());

    auto transitions = espec->getTransitionTable();
    for (auto it = transitions.constBegin(); it != transitions.constEnd(); ++it) {

        int from = it.key().first;
        int to = it.key().second;

         QSet<Condition> conditions = it.value();

        for (const Condition& cond : conditions) {
            trTable.addRow({
                idEx,
                from,
                to,
                conditionTypeToString(cond.type),
                cond.keypointLine,
                PoseViewToString(cond.view)
            });
        }
    }
    if (trTable.rowCount() > 0) {
        if (!dbManager->save(TypeBDEnum::Transition, trTable)) {

            QString error=QString("error guardando el ejercicio con id : %1 en la base de datos").arg(idEx);
            qWarning(TrainingRepo) << error;
            emit ui_errorMessage(error,QtMsgType::QtWarningMsg);
            return -1;
        }
    }
    //qDebug(TrainingRepo)<<"Tabla generada: Transitions";
    //trTable.print();
    return idEx;
}

/*!
 * \brief Elimina una especificación completa del sistema.
 * \param idEx ID del ejercicio a eliminar.
 * \return true si fue eliminada correctamente.
 */

bool TrainingRepository::deleteEspec(int idEx)
{
    QString idStr = QString::number(idEx);

    // Obtener todos los estados asociados al ejercicio
    DBTable stateTable = dbManager->get(TypeBDEnum::State, DBFilter::IdEx, idStr);

    // Eliminar transiciones del ejercicio
    if (!dbManager->deleteRow(TypeBDEnum::Transition, DBFilter::IdEx, idStr)){
        QString error=QString("error borrando el ejercicio con id : %1 de la base de datos").arg(idEx);
        qWarning(TrainingRepo) << error;
        emit ui_errorMessage(error,QtMsgType::QtWarningMsg);
        return false;
    }
    // Eliminar constraints asociadas a los estados del ejercicio
    for (int i = 0; i < stateTable.rowCount(); ++i) {
        QString idState = stateTable.value(i, "idState").toString();

        QMap<DBFilter, QVariant> constraintFilter;
        constraintFilter.insert(DBFilter::IdEx, idStr);
        constraintFilter.insert(DBFilter::IdState, idState);

        if (!dbManager->deleteRow(TypeBDEnum::AngleConstraint, constraintFilter)){
            QString error=QString("error borrando el ejercicio con id : %1 de la base de datos").arg(idEx);
            qWarning(TrainingRepo) << error;
            emit ui_errorMessage(error,QtMsgType::QtWarningMsg);
            return false;
        }
    }

    // Eliminar estados
    if (!dbManager->deleteRow(TypeBDEnum::State, DBFilter::IdEx, idStr)){
        QString error=QString("error borrando el ejercicio con id : %1 de la base de datos").arg(idEx);
        qWarning(TrainingRepo) << error;
        emit ui_errorMessage(error,QtMsgType::QtWarningMsg);
        return false;
    }

    // Eliminar la especificación del ejercicio
    if (!dbManager->deleteRow(TypeBDEnum::ExerciseEspec, DBFilter::IdEx, idStr)){
        QString error=QString("error borrando el ejercicio con id : %1 de la base de datos").arg(idEx);
        qWarning(TrainingRepo) << error;
        emit ui_errorMessage(error,QtMsgType::QtWarningMsg);
        return false;
    }

    return true;
}

/*!
 * \brief Guarda un workout completo incluyendo sus ejercicios asociados.
 * \param workout Workout completo.
 * \return ID del workout guardado.
 */
int TrainingRepository::saveWorkout(QSharedPointer<TrainingWorkout> workout) {
    if (workout==nullptr){

        QString error=QString("error guardando el workout de la base de datos");
        qWarning(TrainingRepo) << error;
        emit ui_errorMessage(error,QtMsgType::QtWarningMsg);
        return -1;
    }

    int idWorkout = workout->getIdWorkout();

    if (idWorkout != -1) {
        // Si ya existe, eliminamos completamente el workout anterior
        if (!deleteWorkoutfull(idWorkout)) {
            QString error=QString("error borrando el workout con id : %1 de la base de datos").arg(idWorkout);
            qWarning(TrainingRepo) << error;
            emit ui_errorMessage(error,QtMsgType::QtWarningMsg);
            return -1;
        }

    } else {
        // Si es nuevo, generamos un ID
        idWorkout = dbManager->generateID(TypeBDEnum::TrainingWorkout);
        workout->setIdWorkout(idWorkout);
    }

    // Guardamos los datos del workout
    DBTable workoutTable = toDBTable_TrainingWorkout(workout);
    //qDebug(TrainingRepo)<<"Tabla generada toDBTable_TrainingWorkout(workout)";
    //workoutTable.print();
    if (!dbManager->save(TypeBDEnum::TrainingWorkout, workoutTable)) {
        QString error=QString("error borrando el workout con id : %1 de la base de datos").arg(idWorkout);
        qWarning(TrainingRepo) << error;
        emit ui_errorMessage(error,QtMsgType::QtWarningMsg);
        return -1;
    }

    // Guardamos los ejercicios y su relación con el workout
    int order = 0;

    DBTable rel("WorkoutExercises");
    DBTable temp=dbManager->get(TypeBDEnum:: WorkoutExercises);
    rel.setColumns(temp.getColumnsNames());

    QList<QSharedPointer<ExerciseEspec>> exs=workout->getExercises();
    if (exs.isEmpty()){
         qWarning(TrainingRepo) << "saveWorkout:Se grabó un workout sin ejercicios, id:"<<idWorkout;
        return idWorkout;
    }
    for (const QSharedPointer<ExerciseEspec>& espec : exs) {
         // Guardamos  o reemplazamos la especificación del ejercicio
        if (espec == nullptr) {
            qWarning(TrainingRepo) << "saveWorkout:Se detectó un ejercicio nullptr en workout";
            continue;}
        if (saveEspec(espec)==-1) {
            QString error=QString("error borrando el workout con id : %1 de la base de datos").arg(idWorkout);
            qWarning(TrainingRepo) << error;
            emit ui_errorMessage(error,QtMsgType::QtWarningMsg);
            return -1;
        }
        qDebug(TrainingRepo)<<"saveWorkout:ejercicio guardado";
        // // Guardamosa la relación workout - ejercicio
        // DBTable rel("WorkoutExercises");
        // DBTable temp=dbManager->get(TypeBDEnum:: WorkoutExercises);
        // rel.setColumns(temp.getColumnsNames());
        //rel.setColumns({"idWorkout", "idEx", "exerciseOrder"});

        rel.addRow({idWorkout, espec->getIdEx(), order++});
    }
    // Guardar la relación workout - ejercicio
    //qDebug(TrainingRepo)<<"Tabla generada: WorkoutExercises";
    //rel.print();
    if (!dbManager->save(TypeBDEnum::WorkoutExercises, rel)) {
        QString error=QString("error borrando el workout con id : %1 de la base de datos").arg(idWorkout);
        qWarning(TrainingRepo) << error;
        emit ui_errorMessage(error,QtMsgType::QtWarningMsg);
        return -1;
    }
     qDebug(TrainingRepo) << "saveWorkout:Guardado Workout con id"<< idWorkout;
    return idWorkout;
}

/*!
 * \brief Guarda una sesión de entrenamiento junto con su resumen y reporte.
 * \param sesion Objeto completo de sesión.
 * \return true si se guardó con éxito.
 */
bool TrainingRepository::SaveTrainingSesion(QSharedPointer<TrainingSesion> sesion)
{
    if (sesion == nullptr || sesion->getExerciseSummary() == nullptr) {
        QString error=QString("Sesión o resumen nulos.");
        qWarning(TrainingRepo) << error;
        emit ui_errorMessage(error,QtMsgType::QtWarningMsg);
        return false;
    }

    // Guardamos la sesión principal
    DBTable sessionTable("TrainingSesion");
    DBTable t=dbManager->get(TypeBDEnum::TrainingSesion);
    sessionTable.setColumns(t.getColumnsNames());
    int idSesion = dbManager->generateID(TypeBDEnum::TrainingSesion);
    sesion->setIdSesion(idSesion);


    //sessionTable.setColumns({"idSesion", "idClient", "idExercise", "date", "duration"});
    sessionTable.addRow({

        //QString::number(sesion->getIdSesion()),
        idSesion,
        sesion->getIdClient(),
        sesion->getIdWorkout(),
        sesion->getExerciseSummary()->getIdEx(),
        sesion->getDate().toString(Qt::ISODate),
        sesion->getDuration()

    });

    if (!dbManager->save(TypeBDEnum::TrainingSesion, sessionTable)) {
        QString error=QString("error guardando la sesión de entrenamiento en la base de datos");
        qWarning(TrainingRepo) << error;
        emit ui_errorMessage(error,QtMsgType::QtWarningMsg);
        return false;
    }

    // Guardamos  el ExerciseSummary asociado

    DBTable summaryTable("ExerciseSummary");

    DBTable s=dbManager->get(TypeBDEnum::ExerciseSummary);

    summaryTable.setColumns(s.getColumnsNames());

   // summaryTable.setColumns({"idExercise", "name", "descricion","type", "targetMuscle"});
    QSharedPointer<ExerciseSummary>summary = sesion->getExerciseSummary();

    summaryTable.addRow({

        summary->getIdEx(),
        summary->getIdWorkout(),
        summary->getName(),
        summary->getDescription(),
        summary->getType(),
        summary->getTargetMuscle(),
        summary->getSeries(),
        summary->getRepetitions(),
        summary->getDuration(),
        summary->getWeightPercentage(),
        summary->getRestTime()

    });

    if (!dbManager->save(TypeBDEnum::ExerciseSummary, summaryTable)) {

        QString error=QString("error guardando el resumen del ejercicio en la base de datos");
        qWarning(TrainingRepo) << error;
        emit ui_errorMessage(error,QtMsgType::QtWarningMsg);
        return false;
    }


    SesionReport report = sesion->getReport();

    nlohmann::json seriesJson = report.toSeriesJson();
    nlohmann::json globalJson = report.toGlobalAnglesJson();
    nlohmann::json overloadsJson = report.toGlobalOverloadsJson();

    QString seriesStr = QString::fromStdString(seriesJson.dump());
    QString globalStr = QString::fromStdString(globalJson.dump());
    QString overloadsStr = QString::fromStdString(overloadsJson.dump());
    //globalOverloadsJson
    QString logText = report.getLog();

    DBTable reportTable("SesionReport");
    DBTable base = dbManager->get(TypeBDEnum::SesionReport);
    reportTable.setColumns(base.getColumnsNames());

    reportTable.addRow({
        idSesion,
        seriesStr,
        globalStr,
        overloadsStr,
        logText
    });
    QString idStr = QString::number(idSesion);
    if (!dbManager->save(TypeBDEnum::SesionReport, reportTable)) {
    //if (!dbManager->save( TypeBDEnum::SesionReport,reportTable,DBFilter::IdSesion,idStr))
        QString error = QString("error guardando el reporte de sesión de entrenamiento en la base de datos");
        qWarning(TrainingRepo) << error;
        emit ui_errorMessage(error, QtMsgType::QtWarningMsg);
        return false;
    }

    return true;
}
/*!
 * \brief Recupera una sesión de entrenamiento a partir de su ID.
 * \param idSesion ID de la sesión.
 * \return Puntero a `TrainingSesion` o nullptr.
 */
QSharedPointer<TrainingSesion> TrainingRepository::getTrainingSesion(int idSesion)
{
    QString strId = QString::number(idSesion);

    // Obtenemos la sesión principal
    DBTable table = dbManager->get(TypeBDEnum::TrainingSesion, DBFilter::IdSesion, strId);
    if (table.rowCount() == 0){
        QString error=QString("error recuperando la sesión de entrenamiento de la base de datos");
        qWarning(TrainingRepo) << error;
        emit ui_errorMessage(error,QtMsgType::QtWarningMsg);
          return nullptr;
    }




    int idClient = table.value(0, "idClient").toInt();
    int idEx = table.value(0, "idEx").toInt();
    QDateTime date = QDateTime::fromString(table.value(0, "date").toString(), Qt::ISODate);
    qint64 duration = table.value(0, "duration").toLongLong();

    // Obtenemos  el resumen del ejercicio
    DBTable summaryTable = dbManager->get(TypeBDEnum::ExerciseSummary, DBFilter::IdEx, QString::number(idEx));
    if (summaryTable.rowCount() == 0) {
        QString error=QString("error recuperando la sesión de entrenamiento de la base de datos");
        qWarning(TrainingRepo) << error;
        emit ui_errorMessage(error,QtMsgType::QtWarningMsg);
        return nullptr;
    }

    QSharedPointer<ExerciseSummary> summary(new ExerciseSummary(
        summaryTable.value(0, "idEx").toInt(),
        summaryTable.value(0, "idWorkout").toInt(),
        summaryTable.value(0, "name").toString(),
        summaryTable.value(0, "description").toString(),
        summaryTable.value(0, "type").toString(),
        summaryTable.value(0, "targetMuscle").toString(),
        summaryTable.value(0, "series").toInt(),
        summaryTable.value(0, "repetitions").toInt(),
        summaryTable.value(0, "duration").toInt(),
        summaryTable.value(0, "weightPercentage").toFloat(),
        summaryTable.value(0, "restTime").toInt()
        ));




    // Se construye la sesión con los datos obtenidos
    QSharedPointer<TrainingSesion> sesion(new TrainingSesion(summary, date, idClient, idSesion));
    sesion->setDuration(duration);


    // se carga el reporte
    DBTable reportTable = dbManager->get(TypeBDEnum::SesionReport, DBFilter::IdSesion, strId);
    SesionReport report;

    QString jsonSeries = reportTable.value(0, "seriesDataJson").toString();
    QString jsonGlobal = reportTable.value(0, "globalAnglesJson").toString();
     QString jsonOver = reportTable.value(0, "globalOverloadsJson").toString();
    QString logText = reportTable.value(0, "log").toString();

    if (reportTable.rowCount() > 0) {
        report.fromSeriesJson(nlohmann::json::parse(jsonSeries.toStdString()));
        report.fromGlobalAnglesJson(nlohmann::json::parse(jsonGlobal.toStdString()));
        report.fromGlobalOverloadsJson(nlohmann::json::parse(jsonGlobal.toStdString()));
        report.setLog(logText);

        report.setIdSesion(idSesion);
        sesion->setReport(report);
    }
    else {
        qWarning(TrainingRepo) << "No se encontró SesionReport para idSesion" << idSesion;
    }

    return sesion;
}



/*!
 * \brief Convierte una especificación de ejercicio a su representación en tabla.
 */

DBTable TrainingRepository::toDBTable_ExerciseEspec(QSharedPointer<ExerciseEspec> espec)
{
    DBTable table("ExerciseEspec");
    DBTable t=dbManager->get(TypeBDEnum::ExerciseEspec);
    table.setColumns(t.getColumnsNames());
    // table.setColumns({
    //     "idEx", "name", "description", "type", "targetMuscle", "equipment","series", "repetitions", "duration", "weightPercentage", "restTime"
    // });

    table.addRow({
        espec->getIdEx(),
        espec->getName(),
         espec->getDescription(),
        espec->getExersiseType(),
        espec->getTargetMuscle(),
        espec->getEquipment(),
         espec->getSeries(),
        espec->getRepetitions(),
        espec->getDuration(),
          espec->getWeightPercentage(),
        espec->getRestTime()
    });

    return table;
}

/*!
 * \brief Convierte un workout a su representación persistente.
 */
DBTable TrainingRepository::toDBTable_TrainingWorkout(QSharedPointer<TrainingWorkout> workout)
{
    DBTable table("TrainingWorkout");
    DBTable t=dbManager->get(TypeBDEnum::TrainingWorkout);
    table.setColumns(t.getColumnsNames());

    //table.setColumns({"idWorkout", "name", "description", "estimatedDuration" });

    table.addRow({
        workout->getIdWorkout(),
        workout->getName(),
         workout->getDescription(),
        workout->getEstimatedDuration()
    });

    return table;
}


/*!
 * \brief Lista los resúmenes de sesiones anteriores filtradas por cliente, workout y ejercicio.
 * \param idClient ID del cliente.
 * \param idWorkout ID del workout.
 * \param idEx ID del ejercicio.
 * \return Mapa de ID de sesión a fecha (`QDateTime`).
 */
QMap<int, QDateTime> TrainingRepository::listSesionSummaries(int idClient, int idWorkout, int idEx) {

    QMap<DBFilter, QVariant> filtros;
    filtros.insert(DBFilter::IdClient, idClient);
    filtros.insert(DBFilter::IdWorkout, idWorkout);
    filtros.insert(DBFilter::IdEx,      idEx);

    DBTable table = dbManager->get(TypeBDEnum::TrainingSesion, filtros);


    QMap<int,QDateTime> mapa;
    for (int row = 0; row < table.rowCount(); ++row) {
        int idSes = table.value(row, "idSesion").toInt();
        // la columna "date" la guardas en ISO8601
        QDateTime dt = QDateTime::fromString(table.value(row, "date").toString(),Qt::ISODate);
        mapa.insert(idSes, dt);
    }

    return mapa;
}


/*!
 * \brief Lista los workouts según un filtro determinado.
 * \param filtro Tipo de filtro (por cliente, ejercicio, etc.).
 * \param valor Valor del filtro aplicado.
 * \return Tabla `DBTable` con los workouts coincidentes.
 */
DBTable TrainingRepository::listWorkouts(DBFilter filtro,QString valor) {
    return dbManager->get(TypeBDEnum::TrainingWorkout,filtro,valor);
}
/*!
 * \brief Lista los ejercicios asociados a un filtro.
 */
DBTable TrainingRepository::listExercises(DBFilter filtro,QString valor) {
    return dbManager->get(TypeBDEnum::ExerciseEspec,filtro,valor);
}
/*!
 * \brief Lista los estados de un ejercicio.
 */

DBTable TrainingRepository::listStates(DBFilter filtro,QString valor){
     return dbManager->get(TypeBDEnum::State,filtro,valor);
}

/*!
 * \brief Lista los constraints asociados a un estado.
 */
DBTable TrainingRepository::listConstraints(DBFilter filtro,QString valor){
     return dbManager->get(TypeBDEnum::AngleConstraint,filtro,valor);
}

/*!
 * \brief Obtiene el siguiente ID disponible para una especificación de ejercicio.
 * \return ID generado.
 */
int TrainingRepository::getNextExerciseId(){return dbManager->generateID(TypeBDEnum::ExerciseEspec);}
/*!
 * \brief Obtiene el siguiente ID disponible para un workout.
 * \return ID generado.
 */
int TrainingRepository::getNextWorkoutId(){return dbManager->generateID(TypeBDEnum::TrainingWorkout);}
