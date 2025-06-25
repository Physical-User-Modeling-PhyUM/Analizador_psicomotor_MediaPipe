#include "testdb.h"

void TestDB::initTestCase() {
    dbPath = "testdb.sqlite";
    QFile::remove(dbPath);
    dbManager = new DBManager(dbPath);
    QVERIFY(dbManager->openDatabase(dbPath));
    QVERIFY(dbManager->initializeSchema());
}

void TestDB::cleanupTestCase() {
    dbManager->closeDatabase();
    delete dbManager;
    QFile::remove(dbPath);
}

void TestDB::testOpenDatabase() {
    //DBManager tempManager("temp.sqlite");
    QVERIFY(dbManager->openDatabase(dbPath));
    //QFile::remove("temp.sqlite");
}

void TestDB::testInitializeSchema() {
    // Se probó ya en initTestCase, aquí verificamos una tabla conocida
    DBTable t = dbManager->get(TypeBDEnum::WorkoutExercises);
    QVERIFY(t.getColumnsNames().contains("idEx")); // columna definida en schema.sql
}

void TestDB::testSaveAndGet() {
    DBTable tabla("exercise_espec");
    tabla.setColumns(QStringList() << "idEx" << "Name" << "Description" << "ExerciseType" << "TargetMuscle" << "Equipment" << "Series" << "Repetitions" << "Duration" << "Weight" << "RestTime");
    tabla.addRow({1, "Sentadilla", "Ejercicio para pierna", "fuerza", "cuádriceps", "barra", 3, 10, 60, 50, 90});

    QVERIFY(dbManager->save(TypeBDEnum::ExerciseEspec, tabla));

    DBTable resultado = dbManager->get(TypeBDEnum::ExerciseEspec, DBFilter::IdEx, "1");
    QCOMPARE(resultado.rowCount(), 1);
    QCOMPARE(resultado.value(0, "Name").toString(), QString("Sentadilla"));
}

void TestDB::testSaveWithFilter() {
    qDebug()<<"incluimos los datos en la BD";
    DBTable tabla("exercise_especs");
    tabla.setColumns(QStringList() << "idEx" << "Name" << "Description" << "ExerciseType" << "TargetMuscle" << "Equipment" << "Series" << "Repetitions" << "Duration" << "Weight" << "RestTime");
    tabla.addRow({1, "Sentadilla", "Piernas básicas", "fuerza", "piernas", "barra", 3, 10, 60, 40, 90});
    tabla.addRow({2, "Peso Muerto", "Espalda y piernas", "fuerza", "espalda", "barra", 3, 8, 60, 60, 90});
    tabla.print();
    QVERIFY(dbManager->save(TypeBDEnum::ExerciseEspec, tabla));

    qDebug()<< "Hacemos un update sobre Sentadilla con el mismo idEx (1) y mismo filtro (name)";
    DBTable tablaUpdate("exercise_espec");
    tablaUpdate.setColumns(tabla.getColumnsNames());
    tablaUpdate.addRow({1, "Sentadilla", "Nueva descripción", "potencia", "glúteos", "mancuernas", 4, 12, 70, 45, 60});

    QVERIFY(dbManager->save(TypeBDEnum::ExerciseEspec, tablaUpdate, DBFilter::Name, "Sentadilla"));

    qDebug()<< "Comprobamos que se actualizó la fila con idEx = 1";
    DBTable resultado = dbManager->get(TypeBDEnum::ExerciseEspec, DBFilter::IdEx, "1");
    resultado.print();
    QCOMPARE(resultado.rowCount(), 1);
    QCOMPARE(resultado.value(0, "Description").toString(), QString("Nueva descripción"));
    QCOMPARE(resultado.value(0, "Equipment").toString(), QString("mancuernas"));

    qDebug()<< "agregamos una nueva fila con el mismo name pero diferente idEx";
    DBTable tablaNueva("exercise_espec");
    tablaNueva.setColumns(tabla.getColumnsNames());
    tablaNueva.addRow({3, "Sentadilla", "Variación con salto", "fuerza", "piernas", "peso corporal", 4, 15, 50, 0, 30});

    QVERIFY(dbManager->save(TypeBDEnum::ExerciseEspec, tablaNueva, DBFilter::Name, "Sentadilla"));

    qDebug()<< "Ahora debe haber dos filas con name = Sentadilla";
    DBTable resultadoFinal = dbManager->get(TypeBDEnum::ExerciseEspec, DBFilter::Name, "Sentadilla");
    resultadoFinal.print();
    QCOMPARE(resultadoFinal.rowCount(), 2);

    qDebug()<< "Comprobamos como queda la tabla";
    DBTable tablaFinal = dbManager->get(TypeBDEnum::ExerciseEspec);
    tablaFinal.print();
    QCOMPARE(tablaFinal.rowCount(), 3);


}
void TestDB::testGetWithMultipleValues() {
    qDebug()<< " Insertamos datos de prueba";
    DBTable tabla("exercise_espec");
    tabla.setColumns(QStringList() << "idEx" << "Name" << "Description" << "ExerciseType" << "TargetMuscle" << "Equipment" << "Series" << "Repetitions" << "Duration" << "Weight" << "RestTime");

    tabla.addRow({10, "Curl Biceps", "Ejercicio de brazo", "fuerza", "bíceps", "mancuernas", 3, 12, 45, 15, 30});
    tabla.addRow({11, "Sentadilla", "Ejercicio de pierna", "fuerza", "piernas", "barra", 4, 10, 60, 40, 90});
    tabla.addRow({12, "Press Banca", "Pecho y triceps", "fuerza", "pecho", "barra", 4, 8, 60, 60, 90});
    tabla.print();

    QVERIFY(dbManager->save(TypeBDEnum::ExerciseEspec, tabla));

     qDebug()<< "Filtro múltiple por ID (10,12)";
    QStringList ids;
    ids << "10" << "12";

    DBTable resultado = dbManager->get(TypeBDEnum::ExerciseEspec, DBFilter::IdEx, ids);
    resultado.print();

    qDebug()<< "Verificamos que obtuvimos exactamente 2 resultados";
    QCOMPARE(resultado.rowCount(), 2);

    qDebug()<< "Comprobamos que están las filas correctas (por nombres)";
    QStringList nombres;
    for (int i = 0; i < resultado.rowCount(); ++i) {
        nombres << resultado.value(i, "Name").toString();
    }

    QVERIFY(nombres.contains("Curl Biceps"));
    QVERIFY(nombres.contains("Press Banca"));
    QVERIFY(!nombres.contains("Sentadilla"));
}


void TestDB::testDeleteRow() {
    QString id = "1";
    QVERIFY(dbManager->deleteRow(TypeBDEnum::ExerciseEspec, DBFilter::IdEx, id));
    qDebug()<< "Eliminamos la fila con id=1";
    DBTable resultado = dbManager->get(TypeBDEnum::ExerciseEspec);
    resultado.print();
    DBTable resultadoFiltrado = dbManager->get(TypeBDEnum::ExerciseEspec, DBFilter::IdEx, id);
    qDebug()<< "recuperamos la fila con id=1";
     resultadoFiltrado.print();
    QVERIFY(resultadoFiltrado.isEmpty());
}

void TestDB::testGenerateID() {
    int nuevoID = dbManager->generateID(TypeBDEnum::ExerciseEspec);
    qDebug()<< "Obtenemos un nuevo ID: "<<nuevoID;
    DBTable resultado = dbManager->get(TypeBDEnum::ExerciseEspec);
    resultado.print();
    QVERIFY(nuevoID >= 1);
}

void TestDB::testGetTableColumns() {
    // Caja Blanca – se usa indirectamente aquí como comparación
    QStringList cols = dbManager->getTableColumns("exercise_espec");
    QVERIFY(cols.contains("Name"));
    QVERIFY(cols.contains("Description"));
    QVERIFY(cols.contains("RestTime"));
}
void TestDB::testIsValidFilterForTable() {
    // El nombre de la tabla que existe en schema.sql
    QString table = "exercise_espec";

    // Este filtro está definido como "IdEx" → columna "idEx"
    bool valido = dbManager->isValidFilterForTable(table, DBFilter::IdEx);

    QVERIFY(valido); // Debe ser true porque esa columna existe

    // Ahora probamos un filtro que no exista en esa tabla (por ejemplo, "IdUser")
    bool invalido = dbManager->isValidFilterForTable(table, DBFilter::IdUser);

    QVERIFY(!invalido); // Debe ser false porque "idUser" no está en esa tabla
}

