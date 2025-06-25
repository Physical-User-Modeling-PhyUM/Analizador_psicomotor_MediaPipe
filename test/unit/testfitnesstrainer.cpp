#include "testfitnesstrainer.h"
#include "profiles/fitnesstrainer.h"
#include "enums/enums.h"
#include <QtTest>

/**
 * @brief Inicializa el entorno de pruebas para TestFitnessTrainer.
 */
void TestFitnessTrainer::init_Test() {

    qDebug() << "== TestFitnessTrainer empieza ==";
}

/**
 * @brief Finaliza el entorno de pruebas para TestFitnessTrainer.
 */
void TestFitnessTrainer::finish_Test() {
     qDebug() << "== TestFitnessTrainer termina ==";
}

/**
 * @brief Verifica la correcta inicialización de un objeto FitnessTrainer con campos profesionales.
 */
void TestFitnessTrainer::testConstructorValido() {
    QHash<UserField, QVariant> datos;
    datos[UserField::IdUser] = 30;
    datos[UserField::UserName] = "trainer1";
    datos[UserField::Email] = "trainer@mail.com";
    datos[UserField::Pass] = "trainerpass";
    datos[UserField::Qualification] = "MUcho";
    datos[UserField::specialtyArea] = "Fuerza y Resistencia";
    datos[UserField::experienceYears] = 5;
    datos[UserField::resume] = "bla bla bla.";

    FitnessTrainer trainer(datos);

    QString qual = trainer.getQualifications();
    QString area = trainer.getSpecialtyArea();
    int years = trainer.getYearsOfExperience();
    QString cv = trainer.getResume();

    QCOMPARE(qual, QString("MUcho"));
    QCOMPARE(area, QString("Fuerza y Resistencia"));
    QCOMPARE(years, 5);

}

/**
 * @brief Verifica los métodos para modificar atributos profesionales del entrenador.
 */
void TestFitnessTrainer::testModificacionCamposProfesionales() {
    QHash<UserField, QVariant> datos;
    datos[UserField::IdUser] = 31;
    datos[UserField::UserName] = "trainer2";
    datos[UserField::Email] = "t2@mail.com";
    datos[UserField::Pass] = "pass";
      datos[UserField::resume] = "bla bla bla.";
    FitnessTrainer trainer(datos);

    trainer.setQualifications("mucha");
    trainer.setSpecialtyArea("a tope");
    trainer.setYearsOfExperience(10);


    QString qual = trainer.getQualifications();
    QString area = trainer.getSpecialtyArea();
    int years = trainer.getYearsOfExperience();
    QString cv = trainer.getResume();

    QCOMPARE(qual, QString("mucha"));
    QCOMPARE(area, QString("a tope"));
    QCOMPARE(years, 10);
    QCOMPARE(cv, QString("bla bla bla."));
}
/**
 * @brief Prueba la construcción inválida del objeto Client con distintos errores en los datos.
 *
 * Se verifican casos como ausencia de campos obligatorios, ID negativo y ID no convertible.
 */
void TestFitnessTrainer::test_ConstructorInvalido() {


    QHash<UserField, QVariant> datos;
    datos[UserField::UserName] = "no_id";
    datos[UserField::Email] = "email";
    datos[UserField::Pass] = "123";

    FitnessTrainer admin1(datos);


    datos[UserField::IdUser] = -20;
    datos[UserField::UserName] = "Id negativo";
    datos[UserField::Email] = "email";
    datos[UserField::Pass] = "123";

    FitnessTrainer admin2(datos);


    QHash<UserField, QVariant> datos2;
    datos2[UserField::IdUser] = 1;
    datos2[UserField::Pass] = "123";

    FitnessTrainer admin3(datos2);


    QHash<UserField, QVariant> datos3;
    datos[UserField::IdUser] = "id";
    datos3[UserField::UserName] = "ID String";
    datos3[UserField::Email] = "email";
    datos3[UserField::Pass] = "123";

    FitnessTrainer admin4(datos3);



    int id = admin1.getId();
    int id2 = admin2.getId();
    int id3 = admin3.getId();
    int id4=admin4.getId();


    QVERIFY(id ==-1);
    QVERIFY(id2 ==-1 );
    QVERIFY(id3 ==-1 );
    QVERIFY(id4 ==-1 );
}
