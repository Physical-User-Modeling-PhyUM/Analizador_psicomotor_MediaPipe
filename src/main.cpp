#include "mainwindow.h"

#include <QApplication>
// Incluir los archivos de prueba


int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
