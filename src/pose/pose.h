#ifndef POSE_H
#define POSE_H

#include <QMap>
#include <QPointF>
#include <QVector>
#include <QPair>
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>
#include <QLoggingCategory>



Q_DECLARE_LOGGING_CATEGORY(PoseLog);

class Pose {
public:
    explicit Pose(const nlohmann::json &jsonData, cv::Mat image,QVector<QPair<int, int>> connections);

    int64_t getTimestamp() const;
    QMap<int, QPointF> getKeypoints() const;
    bool areConnected(int keypoint1, int keypoint2) const;
    double getPixelDistance(int keypoint1, int keypoint2) const;
    double getAngle(int keypoint1, int keypoint2) const;
    QVector<QPair<int, int>> getConnections() const;
    cv::Mat drawKeypoints();
    cv::Mat getImage_bgr() const;
    QHash<QString, double> getAngles() const;


private:
    int64_t timestamp;
    cv::Mat image_bgr;
    QMap<int, QPointF> keypoints;
    QVector<QPair<int, int>> connections;

};

#endif // POSE_H
