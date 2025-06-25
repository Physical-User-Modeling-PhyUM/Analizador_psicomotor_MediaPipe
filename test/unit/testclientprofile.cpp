#include "testclientprofile.h"
#include "profiles/clientprofile.h"
#include "enums/enums.h"
#include <QtTest>

/**
 * @brief Inicializa el entorno de pruebas para TestClientProfile.
 */
void TestClientProfile::init_Test() {
    qDebug() << "== TestClientProfile empieza ==";
}

/**
 * @brief Finaliza el entorno de pruebas para TestClientProfile.
 */
void TestClientProfile::finish_Test() {
    qDebug() << "== TestClientProfile termina ==";
}

/**
 * @brief Prueba la inserción y recuperación de un registro funcional.
 */
void TestClientProfile::testRegistroFuncional() {
    ClientProfile profile;
    profile.setFunctionalRecord(FunctionalField::CardioLevel, 3);
    int cardioLevel = profile.getRecord(FunctionalField::CardioLevel).toInt();

    QCOMPARE(cardioLevel, 3);
}

/**
 * @brief Verifica que los valores funcionales inválidos sean rechazados.
 */
void TestClientProfile::test_ValoresFuncional_FueraDeRango() {
    ClientProfile profile;

    bool aceptado = profile.setFunctionalRecord(FunctionalField::HipMobility, 100);  // fuera de rango (debería ser 0–10)
    QVERIFY(!aceptado);

    bool aceptado2 = profile.setRecord(FunctionalField::HipMobility, -2);
    QVERIFY(!aceptado2);
}




/**
 * @brief Prueba el almacenamiento de un valor morfológico y su recuperación.
 */
void TestClientProfile::testRegistroMorfologico() {
    ClientProfile profile;
    profile.setMorfologicalRecord(MorfologicalFields::FatPercentage, 1.75);
    double altura = profile.getRecord(MorfologicalFields::FatPercentage).toDouble();

    QCOMPARE(altura, 1.75);
}

/**
 * @brief Verifica que los valores morfológicos inválidos sean rechazados.
 */
void TestClientProfile::test_Valores_Morfologia_FueraDeRango() {
    ClientProfile profile;

    bool aceptado = profile.setMorfologicalRecord(MorfologicalFields::BMI, 150);  // fuera de rango
    QVERIFY(!aceptado);

    bool aceptado2 = profile.setMorfologicalRecord(MorfologicalFields::FatPercentage, -5);  // negativo
    QVERIFY(!aceptado2);
}



/**
 * @brief Prueba la inserción y recuperación de un rango de movimiento (ROM).
 */
void TestClientProfile::testROM() {
    ClientProfile profile;
    profile.setROM(ROMJoint::ShoulderAbduction, 120.0);
    double rango = profile.getRecord(ROMJoint::ShoulderAbduction).toDouble();

    QCOMPARE(rango, 120.0);
}

/**
 * @brief Verifica que los valores fuera de rango para movilidad articular sean rechazados.
 */
void TestClientProfile::test_ValoresROM_FueraDeRango() {
    ClientProfile profile;
    bool aceptado = profile.setROM(ROMJoint::ShoulderFlexion, 300);  // fuera de rango
    QVERIFY(aceptado == false);

    bool aceptado2 = profile.setROM(ROMJoint::ShoulderFlexion, -10); // fuera de rango inferior
    QVERIFY(aceptado2 == false);
}



/**
 * @brief Verifica la adición de registros médicos al historial del cliente.
 */
void TestClientProfile::testRegistroMedico() {
    ClientProfile profile;
    profile.setRecord(MedicalRecordsFields::PreviousInjuries, "Lesión de rodilla");

    QString registro = profile.getRecord(MedicalRecordsFields::PreviousInjuries).toString();
    QVERIFY(registro.contains("Lesión de rodilla"));
}

/**
 * @brief Prueba la inserción y consulta de un registro de estilo de vida.
 */
void TestClientProfile::testRegistroEstiloVida() {
    ClientProfile profile;
    profile.setRecord(LifeStyleFields::SmokingHabits, "No fuma");

    QString habito = profile.getRecord(LifeStyleFields::SmokingHabits).toString();
    QCOMPARE(habito, QString("No fuma"));
}

/**
 * @brief Prueba la gestión de datos adicionales personalizados del perfil.
 */
void TestClientProfile::testDatosAdicionales() {
    // ClientProfile profile;
    // profile.setRecord("Observaciones", "Muy motivado");

    // QString obs = profile.getAdditionalData("Observaciones");
    // QCOMPARE(obs, QString("Muy motivado"));
}
