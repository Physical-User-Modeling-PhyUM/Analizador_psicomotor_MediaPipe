/**
 * @file uiutils.cpp
 * @brief Implementación de funciones auxiliares para vincular estructuras de datos con la interfaz de usuario mediante UiTableModel.
 */

#include "uiutils.h"

// -----------------------------------------------------------

/**
 * @brief Carga una lista de resúmenes de entrenamientos en el modelo de tabla.
 * @param list Lista de objetos WorkoutSummary.
 * @param model Modelo de tabla a poblar.
 */
void UiUtils::fromWorkoutSummaries(const QList<WorkoutSummary>& list, UiTableModel* model)
{
    QStringList headers = {"ID", "Workout name", "Description", "Author", " Estimated time"};
    QList<QList<QVariant>> rows;

    for (const auto& ws : list) {
        rows.append({
            ws.getIdWorkout(),
            ws.getTitle(),
            ws.getDescription(),
            ws.getAuthor(),
            ws.getEstimatedTime()
        });
    }

    model->setDataSet(rows, headers);
}
/**
 * @brief Carga los ejercicios de un entrenamiento específico en una tabla UI.
 * @param workout Puntero al objeto TrainingWorkout.
 * @param model Modelo de tabla que se llenará con los datos.
 */
void UiUtils::fromWorkoutExercises(const QSharedPointer<TrainingWorkout>& workout, UiTableModel* model)
{
    QStringList headers = {"ID", "Exercise name", "Description","type", "Target Muscle", "Equipment", "Series", "Reps", "Duration (sec)", "% Weight", "Rest (sec)"};
    QList<QList<QVariant>> rows;

    auto exercises = workout->getExercises();
    for (const auto& ex : exercises) {

        rows.append({
            ex->getIdEx(),
            ex->getName(),
                     ex->getDescription(),
            ex->getExersiseType(),
            ex->getTargetMuscle(),
            ex->getEquipment(),
            ex->getSeries(),
            ex->getRepetitions(),
            ex->getDuration(),
            ex->getWeightPercentage(),
            ex->getRestTime()
        });
    }

    model->setDataSet(rows, headers);
}

/**
 * @brief Carga los estados de un ejercicio en el modelo de tabla.
 * @param exercise Puntero al objeto ExerciseEspec.
 * @param model Modelo de tabla a poblar.
 */
void UiUtils::fromExerciseStates(const QSharedPointer<ExerciseEspec>& exercise, UiTableModel* model)
{
    QStringList headers = {"IdState","IdExercise", "Min time(ms)", "Max Time(ms)","Name"};
    QList<QList<QVariant>> rows;

     auto states = exercise->getStatesList();
    for (const auto& st : states) {
        rows.append({
            st.getId(),
            st.getIdEx(),
            st.getMinTime(),
            st.getMaxTime(),
            st.getName()

        });
    }

    model->setDataSet(rows, headers);
}

/**
 * @brief Llena la tabla con los constraints angulares asociados a estados.
 * @param constraints Hash de restricciones angulares indexado por línea clave.
 * @param model Modelo de tabla destino.
 */
void UiUtils::fromStateConstraints(const QHash<QString, AngleConstraint>& constraints, UiTableModel* model)
{
    QStringList headers = {"idConst","idState","IdEx","Axis", "Min Angle.", "Max Angle", "Min Safe Angle", "Max Safe Angle", "Constant Angle", "Movement direction", "Tolerance", "Fast Threshold(deg/sec)", "Slow Threshold(deg/sec)","view"};
    QList<QList<QVariant>> rows;

    for (auto it = constraints.constBegin(); it != constraints.constEnd(); ++it) {
        QString key = it.key();
        AngleConstraint c = it.value();

        rows.append({
            c.getIdConstraint(),
            c.getIdState(),
            c.getIdEx(),
            key,
            c.getMinAngle(),
            c.getMaxAngle(),
            c.getMinSafeAngle(),
            c.getMaxSafeAngle(),
            c.getSymetricalAngle(),
            directionToString(c.getEvolution()),
            c.getToler(),
            c.getFastThreshold(),
            c.getSlowThreshold(),
            PoseViewToString(c.getView())

        });
        //qDebug() << "fromStateConstraints:" <<PoseViewToString(c.getView());

    }

    model->setDataSet(rows, headers);
}
/**
 * @brief Llena la tabla con las transiciones entre estados y sus condiciones.
 * @param transitions Hash de pares de estados asociados a un conjunto de condiciones.
 * @param model Modelo de tabla a poblar.
 */
void UiUtils::fromStateTransitions(const QHash<QPair<int, int>, QSet<Condition>>& transitions, UiTableModel* model)
{
    QStringList headers = {"start State", "End State", "Condition reached", "Axis","view"};
    QList<QList<QVariant>> rows;

    for (auto it = transitions.constBegin(); it != transitions.constEnd(); ++it) {
        const QPair<int, int>& key = it.key();
        const QSet<Condition>& conditions = it.value();

        for (const auto& cond : conditions) {
            rows.append({
                key.first,
                key.second,
                conditionTypeToString(cond.type),
                cond.keypointLine,
                PoseViewToString(cond.view)
            });
        }
    }

    model->setDataSet(rows, headers);
}
/**
 * @brief Convierte una tabla UI en un conjunto de transiciones de estados.
 * @param model Modelo de tabla que contiene las transiciones.
 * @return Hash de transiciones entre estados con sus condiciones.
 */
QHash<QPair<int, int>, QSet<Condition>> UiUtils::toStateTransitions(UiTableModel* model)
{
    QHash<QPair<int, int>, QSet<Condition>> transitions;

    auto rows = model->getDataSet();
    for (const auto& row : rows) {
        if (row.size() < 5) continue;

        int fromId = row[0].toInt();
        int toId = row[1].toInt();

        ConditionType type = conditionTypeFromString(row[2].toString());
        QString line = row[3].toString();
        PoseView view=PoseViewFromString(row[4].toString());
        //QVariant val=row[5].toString();
        QPair<int, int> key(fromId, toId);
        transitions[key].insert(Condition(type, line,0,view));
    }
    //qDebug()<<"UiUtils model-> transitions"<<model->rowCount()<<"->"<<transitions.size();
    return transitions;
}
/**
 * @brief Convierte una tabla en un conjunto actualizado de estados, conservando constraints si es posible.
 * @param originalStates Estados originales con constraints.
 * @param model Modelo de tabla UI con los datos actualizados.
 * @return Conjunto de objetos State actualizados.
 */
QSet<State> UiUtils::toExerciseStates(QSet<State> originalStates, UiTableModel* model)
{
    QSet<State> updatedStates;
    auto rows = model->getDataSet();

    for (const auto& row : rows) {
        if (row.size() < 4) continue;

        int id = row[0].toInt();
        int idEx = row[1].toInt();
        int minTime = row[2].toInt();
        int maxTime = row[3].toInt();
        QString name= row[4].toString();

        // Buscar el State original para conservar constraints
        State updated;
        bool found = false;

        for (auto it = originalStates.begin(); it != originalStates.end(); ++it) {
            if (it->getId() == id) {
                updated = *it;
                found = true;
                break;
            }
        }

        if (!found) {
            updated.setId(id); // crear nuevo si no se encuentra
            updated.setIdEx(idEx);
        }

        updated.setMinTime(minTime);
        updated.setMaxTime(maxTime);
        updated.setName(name);

        updatedStates.insert(updated);
    }

    return updatedStates;
}

/**
 * @brief Actualiza directamente los tiempos y nombres de los estados en base a los datos del modelo de tabla.
 * @param states Conjunto de estados que se actualizarán.
 * @param model Modelo de tabla con datos nuevos.
 */
void UiUtils::updateExerciseStates(QSet<State>& states, UiTableModel* model)
{
    if (model == nullptr) return;

    auto rows = model->getDataSet();

    for (const auto& row : rows) {
        if (row.size() < 3) continue;

        int id = row[0].toInt();
        int minTime = row[1].toInt();
        int maxTime = row[2].toInt();
        QString name=row[3].toString();


        for (auto it = states.begin(); it != states.end(); ++it) {
            if (it->getId() == id) {
                const_cast<State&>(*it).setMinTime(minTime);
                const_cast<State&>(*it).setMaxTime(maxTime);
                 const_cast<State&>(*it).setName(name);
                break;
            }
        }
    }

}

/**
 * @brief Actualiza un objeto ExerciseEspec a partir de los valores presentes en una fila de tabla.
 * @param model Modelo de tabla con la información.
 * @param ex Puntero al ejercicio que se actualizará.
 */
void UiUtils::updateExerciseEspecFromTable(UiTableModel* model, QSharedPointer<ExerciseEspec> ex)
{
    if (!ex) return;
    if (!model) return;

    auto rows = model->getDataSet();
    if (rows.isEmpty()) return;

    int idEx = ex->getIdEx();

    for (const auto& row : rows) {
        if (row.size() < 11) continue;

        int rowId = row[0].toInt();
        if (rowId == idEx) {

            ex->setIdEx(row[0].toInt());
            ex->setName(row[1].toString());
            ex->setDescription(row[2].toString());
            ex->setExersiseType(row[3].toString());
            ex->setTargetMuscle(row[4].toString());
            ex->setEquipment(row[5].toString());
            ex->setSeries(row[6].toInt());
            ex->setRepetitions(row[7].toInt());
            ex->setDuration(row[8].toInt());
            ex->setWeightPercentage(row[9].toFloat());
            ex->setRestTime(row[10].toInt());
            return;
        }
    }
}
/**
 * @brief Actualiza un objeto TrainingWorkout desde los valores de su fila correspondiente en el modelo.
 * @param model Modelo de tabla con los datos.
 * @param workout Puntero al entrenamiento a modificar.
 */
void UiUtils::updateWorkoutFromTable(UiTableModel* model, QSharedPointer<TrainingWorkout> workout)
{
    if (!workout || !model) return;

    auto rows = model->getDataSet();
    if (rows.isEmpty()) return;

    int idWorkout = workout->getIdWorkout();

    for (const auto& row : rows) {
        if (row.size() < 5) continue;

        int rowId = row[0].toInt();
        if (rowId == idWorkout) {
            workout->setIdWorkout(row[0].toInt());
            workout->setName(row[1].toString());
            workout->setDescription(row[2].toString());
            workout->setAuthor(row[3].toString());
            workout->setEstimatedDuration(row[4].toInt());
            return;
        }
    }
}
/**
 * @brief Convierte una tabla UI en un hash de constraints angulares por línea.
 * @param model Modelo de tabla que contiene los constraints.
 * @return Hash con objetos AngleConstraint asociados a sus claves de línea.
 */

QHash<QString, AngleConstraint> UiUtils::toStateConstraints(UiTableModel* model)
{
    QHash<QString, AngleConstraint> constraints;
    auto rows = model->getDataSet();

    for (const auto& row : rows) {
        if (row.size() < 14) continue;
        AngleConstraint c;

        QString key = row[3].toString();
        c.setIdConstraint(row[0].toInt()),
            c.setIdState(row[1].toInt()),
             c.setIdEx(row[2].toInt()),
            c.setLinea(row[3].toString()),
        c.setMinAngle(row[4].toFloat());
        c.setMaxAngle(row[5].toFloat());
        c.setMinSafeAngle(row[6].toFloat());
        c.setMaxSafeAngle(row[7].toFloat());
        c.setSymetricalAngle(row[8].toFloat());
        c.setEvolution(directionFromString(row[9].toString()));
        c.setToler(row[10].toFloat());
        c.setFastThreshold(row[11].toFloat());
        c.setSlowThreshold(row[12].toFloat());
        c.setView(PoseViewFromString(row[13].toString()));
       // qDebug() << "toStateConstraints: row->" <<row[13].toString()  << "→ enum:" << PoseViewToString(PoseViewFromString(row[13].toString()));

        constraints.insert(key, c);
    }

    return constraints;
}


/**
 * @brief Carga en la tabla una lista de resúmenes de ejercicios.
 * @param list Lista de objetos ExerciseSummary.
 * @param model Modelo de tabla a poblar.
 */
void UiUtils::fromExerciseSummaries(const QList<ExerciseSummary>& list, UiTableModel* model)
{
    if (!model) return;

    QList<QList<QVariant>> rows;
    for (const auto& e : list) {
        rows.append({
            e.getIdEx(),
            e.getIdWorkout(),
            e.getName(),
            e.getType(),
            e.getTargetMuscle(),
            e.getDescription(),
        });
    }

    QStringList headers = {"ID", "Plan" "Nombre", "Tipo", "Grupo Muscular","Descripción"};
    model->setDataSet(rows, headers);
}

/**
 * @brief Muestra un popup de filtrado para una columna específica de una tabla.
 * @param parent Widget padre.
 * @param table Tabla a la que se aplica el filtro.
 * @param proxy Modelo proxy de filtrado.
 * @param logicalIndex Índice lógico de la columna a filtrar.
 */
void UiUtils::showFilterPopup(QWidget* parent, QTableView* table, QSortFilterProxyModel* proxy, int logicalIndex)
{
    if (!table || !proxy) return;

    QMenu* menu = new QMenu(parent);
    QWidgetAction* actionWidget = new QWidgetAction(menu);
    QLineEdit* filterEdit = new QLineEdit(menu);

    filterEdit->setPlaceholderText("Filtrar...");
    actionWidget->setDefaultWidget(filterEdit);
    menu->addAction(actionWidget);

    QObject::connect(filterEdit, &QLineEdit::textChanged, proxy, [proxy, logicalIndex](const QString& text) {
        proxy->setFilterKeyColumn(logicalIndex);
        proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
        proxy->setFilterFixedString(text);
    });

    menu->exec(QCursor::pos());
}
/**
 * @brief Llena la tabla con una lista de usuarios.
 * @param list Lista de punteros a objetos User.
 * @param model Modelo de tabla UI destino.
 */
void UiUtils::fromUserList(const QList<QSharedPointer<User>>& list, UiTableModel* model)
{
    if (!model) return;

    QStringList headers = {"ID", "Nombre", "Email", "Tipo"};
    QList<QList<QVariant>> rows;

    for (const auto& user : list) {
        rows.append({
            user->getId(),
            user->getUserName(),
            user->getEmail(),
            UserTypeToString(user->getUserType())
        });
    }

    model->setDataSet(rows, headers);
}
/**
 * @brief Carga en la tabla un calendario de entrenamientos asignados a fechas.
 * @param calendar Mapa de fechas a IDs de workout.
 * @param model Modelo de tabla a poblar.
 */
void UiUtils::fromWorkoutCalendar(const QMap<QDateTime, int>& calendar, UiTableModel* model)
{
    if (!model) return;

    QList<QList<QVariant>> rows;
    for (auto it = calendar.constBegin(); it != calendar.constEnd(); ++it) {
        rows.append({ it.value(), it.key() });
    }

    QStringList headers = { "ID Workout", "Fecha" };
    model->setDataSet(rows, headers);
}

/**
 * @brief Rellena una tabla editable con claves y valores a partir de un map, usando ComboBox para las claves.
 * @param table Tabla destino.
 * @param data Mapa de clave-valor que se visualizará.
 * @param allFields Lista de claves válidas para mostrar en el ComboBox.
 */
void UiUtils::populateEnumTable(QTableView* table, const QVariantMap& data, const QStringList& allFields)
{
    if (!table) return;

    auto* model = new QStandardItemModel(data.size(), 2, table);
    model->setHorizontalHeaderLabels(QStringList{"Campo", "Valor"});

    int row = 0;
    for (auto it = data.constBegin(); it != data.constEnd(); ++it, ++row) {
        QStandardItem* field = new QStandardItem(it.key());
        QStandardItem* valor= new QStandardItem(it.value().toString());
        model->setItem(row, 0, field);
        model->setItem(row, 1, valor);
    }

    table->setModel(model);

    //La primera columna usará los valores en un combobox
    auto* delegate = new ComboBoxDelegate(table, allFields);
    table->setItemDelegateForColumn(0, delegate);
}

/**
 * @brief Convierte el contenido de una tabla editable en un QVariantMap.
 * @param table Tabla que contiene los datos clave-valor.
 * @return Mapa con los datos recuperados de la tabla.
 */
QVariantMap UiUtils::tableToVariantMap(QTableView* table)
{
    QVariantMap map;
    if (!table || !table->model()) return map;

    QAbstractItemModel* model = table->model();
    for (int row = 0; row < model->rowCount(); ++row) {
        QString key = model->data(model->index(row, 0)).toString();
        QVariant value = model->data(model->index(row, 1));
        if (!key.isEmpty()) {
            map.insert(key, value);
        }
    }
    return map;
}
/**
 * @brief Carga en la tabla un resumen de sesiones pasadas con su ID y fecha.
 * @param sesiones Mapa de ID de sesión a fecha.
 * @param model Modelo de tabla destino.
 */
void UiUtils::fromSessionSummaries(const QMap<int,QDateTime>& sesiones,
                                   UiTableModel* model)
{
    if (!model) return;

    // Cabeceras
    QStringList headers = { "ID Sesión", "Fecha" };
    QList<QList<QVariant>> rows;
    rows.reserve(sesiones.size());

    // Cada entrada: [ idSesion, fecha.isoString() ]
    for (auto it = sesiones.constBegin(); it != sesiones.constEnd(); ++it) {
        rows.append({
            it.key(),
            it.value().toString(Qt::ISODate)
        });
    }

    model->setDataSet(rows, headers);
}
