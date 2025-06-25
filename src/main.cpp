#include "ui/main/mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QLoggingCategory>

QFile logFile;

void initLogFile(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString fileName = "log_" + QDate::currentDate().toString("yyyyMMdd") + ".txt";
    logFile.setFileName(QCoreApplication::applicationDirPath() + "/" + fileName);
    logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    QTextStream out(&logFile);
    QString typeStr;
    switch (type) {
    case QtDebugMsg:    typeStr = "DEBUG"; break;
    case QtInfoMsg:     typeStr = "INFO"; break;
    case QtWarningMsg:  typeStr = "WARN"; break;
    case QtCriticalMsg: typeStr = "CRIT"; break;
    case QtFatalMsg:    typeStr = "FATAL"; break;
    }

    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
        << " [" << typeStr << "] " << msg << "\n";
    out.flush();

    if (type == QtFatalMsg)
        abort();
}

QScopedPointer<QFile> stateLogFile;

void customMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
    static QTextStream stream(stdout);

    if (stateLogFile && stateLogFile->isOpen() &&
        (QString::fromUtf8(context.category) == "stateLog" ||
         QString::fromUtf8(context.category) == "stateMachine"||
         QString::fromUtf8(context.category) == "posemanager"  )) {

        QTextStream logStream(stateLogFile.data());
        logStream << msg << "\n";
        logStream.flush();
    }

    // Opcional: también imprimir por consola
    stream << msg << "\n";
    stream.flush();
}




int main(int argc, char *argv[])
{
    stateLogFile.reset(new QFile("/Users/MZT/pfg/build/Desktop_x86_darwin_generic_mach_o_64bit-Debug/log_state_machine.txt"));
    stateLogFile->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);

    qInstallMessageHandler(customMessageHandler);

    QApplication a(argc, argv);

    //qInstallMessageHandler(initLogFile);
    auto controller = QSharedPointer<AppController>::create();
    controller->setSelf(controller);
    controller->initialize();

    //MainWindow w(controller);
    //w.show();

    return a.exec();
}
