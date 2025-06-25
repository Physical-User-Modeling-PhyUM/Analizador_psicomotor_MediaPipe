#ifndef TESTINTEGRACIONDUMMYPOSEMANAGER_H
#define TESTINTEGRACIONDUMMYPOSEMANAGER_H

#include "dummyposemanager.h"
#include <QLoggingCategory>



Q_DECLARE_LOGGING_CATEGORY(TestDummyPoseLog)
class TestIntegracionDummyPoseManager
{
public:
    TestIntegracionDummyPoseManager();
    void run();
    void loadConfig();
    QHash<QPair<int, int>, QString>connections;
    QHash<QString, QVariant> poseCaptureConfig;
    QHash<int,QString> keypoints;

    int maxBufferSize=100;
    bool dualmode=false;

};

#endif // TESTINTEGRACIONDUMMYPOSEMANAGER_H
