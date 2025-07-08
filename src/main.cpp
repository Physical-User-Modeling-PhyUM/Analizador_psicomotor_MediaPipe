#include "ui/main/mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QLoggingCategory>

static QFile logFile;

void initLogFileHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Creamos la carpeta Logs si no existe
    QString logDirPath = QCoreApplication::applicationDirPath() + "/Logs";
    QDir logDir(logDirPath);
    if (!logDir.exists()) {
        logDir.mkpath(".");
    }

    // Solo configuramos una vez
    if (!logFile.isOpen()) {
        QString fileName = "log_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".txt";
        QString fullPath = logDirPath + "/" + fileName;

        logFile.setFileName(fullPath);
        if (!logFile.open(QIODevice::Append | QIODevice::Text)) {
            qCritical() << "No se pudo abrir el archivo de log:" << fullPath;
            return;
        }
    }

    QTextStream out(&logFile);
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ")
        << "[" << context.category << "] "
        << msg << "\n";
    out.flush();
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


    //stream << msg << "\n";
    //stream.flush();
}




int main(int argc, char *argv[])
{
    //stateLogFile.reset(new QFile("/Users/MZT/pfg/build/Desktop_x86_darwin_generic_mach_o_64bit-Debug/log_state_machine.txt"));
    //stateLogFile->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);

    //qInstallMessageHandler(customMessageHandler);

    QApplication a(argc, argv);

    qInstallMessageHandler(initLogFileHandler);
    auto controller = QSharedPointer<AppController>::create();
    controller->setSelf(controller);
    controller->initialize();

    return a.exec();
}
