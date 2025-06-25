#ifndef TESTPOSE_H
#define TESTPOSE_H

#include <iostream>
#include "../../src/pose/pose.h"
#include <QCoreApplication>
#include <qfile.h>
#include "utils/jsonutils.h"
#include "utils/imageutils.h"




class TestPose {

public:

    nlohmann::json keypoints1;
    nlohmann::json keypoints2;
    nlohmann::json JsonConnections;
    QHash<QPair<int, int>, QString>connections;
    cv::Mat image1;
    cv::Mat image2;
     QHash<int,QString> keypoints;

    void runTests();
private:
    void testPoseInitialization();
    void testKeypointsAndConnections();
    void testAngles();
    void testDistance();
    void testDrawKeyPoints();




};
#endif // TESTPOSE_H
