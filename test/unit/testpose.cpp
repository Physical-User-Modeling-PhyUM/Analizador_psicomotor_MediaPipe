#include <iostream>
#include "../../src/pose/pose.h"
#include <QCoreApplication>
#include <qfile.h>
#include "utils/jsonutils.h"
#include "utils/imageutils.h"
#include"testpose.h"


Q_LOGGING_CATEGORY(TestPoseLog, "testpose");
    void TestPose::runTests() {
        qDebug(TestPoseLog) << "Ejecutando pruebas de TestPose...";

        QList<QString> jsonList={
            "/jsonTest/image_1_keypoints.json",
            "/jsonTest/image_2_keypoints.json",
            "/jsonTest/poseConfig.json",
        };
        QString buildPath = QCoreApplication::applicationDirPath();
        keypoints1=JsonUtils::readJsonFile(buildPath+jsonList[0]);
        keypoints2=JsonUtils::readJsonFile(buildPath+jsonList[1]);
        JsonConnections=JsonUtils::readJsonFile(buildPath+jsonList[2]);

        // if (JsonConnections.contains("connections")) {
        //     for (auto &conn : JsonConnections["connections"]) {
        //         connections.append(qMakePair(conn[0].get<int>(), conn[1].get<int>()));
        //     }
        // }
        qDebug(TestPoseLog) << "Contenido de CONNECTIONS: " << JsonConnections["CONNECTIONS"].dump(4);

        // if (JsonConnections.contains("CONNECTIONS")) {
        //     for (auto &conn : JsonConnections["CONNECTIONS"]) {
        //         if (!conn.is_array() || conn.size() != 2) {
        //             qCritical() << "Error en el formato de 'CONNECTIONS' en el JSON";
        //             continue;
        //         }
        //         connections.append(qMakePair(conn[0].get<int>(), conn[1].get<int>()));
        //     }
        // }
        if (JsonConnections.contains("KEYPOINTS")) {
            nlohmann:: json keys = JsonConnections["KEYPOINTS"];
            for (auto it = keys.begin(); it != keys.end(); ++it) {
                int key = std::stoi(it.key());
                QString name = QString::fromStdString(it.value());
                keypoints.insert(key, name);
            }
        }

        if (JsonConnections.contains("CONNECTIONS")) {
            nlohmann:: json connArray = JsonConnections["CONNECTIONS"];
            for (const auto& conn : connArray) {
                int from = conn[0];
                int to = conn[1];
                QString fromName = keypoints.value(from, QString::number(from));
                QString toName = keypoints.value(to, QString::number(to));
                QString name = fromName + "<->" + toName;
                connections.insert(qMakePair(from, to), name);
            }
        }

        QString imagePath1= buildPath+"/images/1.jpeg";
        image1 = cv::imread(imagePath1.toStdString(), cv::IMREAD_COLOR);

        if (image1.empty()) {
            qCritical(TestPoseLog)<< "Error: No se pudo cargar la imagen en " << imagePath1;
            return;
        }

        QString imagePath2 = buildPath+"/images/2.jpeg";
        image2 = cv::imread(imagePath2.toStdString(), cv::IMREAD_COLOR);

        if (image2.empty()) {
            qCritical(TestPoseLog)<< "Error: No se pudo cargar la imagen en " << imagePath2;
            return;
        }


        testPoseInitialization();
        testKeypointsAndConnections();
        testDistance();
        testAngles();
        testDrawKeyPoints();
    }

    void TestPose::testPoseInitialization() {


        Pose pose(keypoints1,image1,connections);
        if (pose.getTimestamp() != keypoints1["timestamp"])
            {
            qCritical(TestPoseLog)<< "TestPoseInitialization FAILED (timestamp)";
            return;
            }
        else if (!ImageUtils::areSameImage(pose.getImage_bgr(), image1))
            { qCritical(TestPoseLog)<< "TestPoseInitialization FAILED (image)";return;}



        else if (connections.size() != pose.getConnections().size())
            { qCritical(TestPoseLog)<< "TestPoseInitialization FAILED (connections)";return;}

        // for (int i = 0; i < connections.size(); ++i) {
        //     if (connections[i] != pose.getConnections()[i]) {
        //         qCritical(TestPoseLog)<< "TestPoseInitialization FAILED (connections)";
        //         return;

        //     }
        // }
         qDebug(TestPoseLog) << "TestPoseInitialization PASSED";


    }

    void TestPose::testKeypointsAndConnections() {


        Pose pose(keypoints1,image1,connections);
        if (pose.getKeypoints().contains(0) && pose.getKeypoints().contains(1) && pose.areConnected(0, 1))
            {qDebug(TestPoseLog) << "TestKeypointsAndConnections PASSED";}
        else
            {qCritical(TestPoseLog)<< "TestKeypointsAndConnections FAILED";}
    }

    void TestPose::testAngles() {

        Pose pose(keypoints2,image2,connections);
        if (pose.getAngle(12, 11) == 90)
            {qDebug(TestPoseLog) << "TestAngles (12,11) PASSED";}
        else
            {qCritical(TestPoseLog)<< "TestAngles FAILED, angle(12,11)= "<<pose.getAngle(12, 11);}
        if (pose.getAngle(12, 24) == 170.981)
            {qDebug(TestPoseLog) << "TestAngles (12,24) PASSED";}
        else
            {qCritical(TestPoseLog)<< "TestAngles FAILED, angle(12,24)= "<<pose.getAngle(12, 24);}
        if (pose.getAngle(23, 24) == 270)
            {qDebug(TestPoseLog) << "TestAngles (23,24) PASSED";}
        else
            {qCritical(TestPoseLog)<< "TestAngles FAILED, angle(23,24)= "<<pose.getAngle(23, 24);}
        if (pose.getAngle(23, 11) == 8.1301)
            {qDebug(TestPoseLog) << "TestAngles (23,11) PASSED";}
        else
            {qCritical(TestPoseLog)<< "TestAngles FAILED, angle(23,11)= "<<pose.getAngle(23, 11);}

    }

    void TestPose::testDistance(){

    }


    void TestPose::testDrawKeyPoints() {

        cv::Mat notDraw1=image1.clone();
        cv::Mat notDraw2=image2.clone();

        Pose pose1(keypoints1, image1, connections);
        Pose pose2(keypoints2, image2, connections);

        cv::Mat draw1 = pose1.drawKeypoints();
        cv::Mat draw2 = pose2.drawKeypoints();

        if (draw1.empty() || draw2.empty()) {
            qCritical(TestPoseLog) << "Error: No se pudo generar la imagen con keypoints.";
            return;
        }

        // Obtenemos  el tamaño más grande entre las imágenes para homogeneizar tiodas
        int target_width = std::max({image1.cols, image2.cols, draw1.cols, draw2.cols});
        int target_height = std::max({image1.rows, image2.rows, draw1.rows, draw2.rows});

        int type = image1.type();

        // Redimensionamos las  imágenes para que todas tengan el mismo tamaño
        cv::resize(notDraw1, notDraw1, cv::Size(target_width, target_height));
        cv::resize(notDraw2, notDraw2, cv::Size(target_width, target_height));
        cv::resize(draw1, draw1, cv::Size(target_width, target_height));
        cv::resize(draw2, draw2, cv::Size(target_width, target_height));

        // Creamos  una imagen del doble del tamaño original (2x2)
        cv::Mat result(target_height * 2, target_width * 2, type, cv::Scalar(0, 0, 0));

        // se copian imágenes en su respectiva posición
        notDraw1.copyTo(result(cv::Rect(0, 0, target_width, target_height)));           // Arriba izquierda
        notDraw2.copyTo(result(cv::Rect(target_width, 0, target_width, target_height))); // Arriba derecha
        draw1.copyTo(result(cv::Rect(0, target_height, target_width, target_height))); // Abajo izquierda
        draw2.copyTo(result(cv::Rect(target_width, target_height, target_width, target_height))); // Abajo derecha


        cv::imshow("resultado", result);
        cv::waitKey(0);
    }






