#ifndef POSEVISUALIZER_H
#define POSEVISUALIZER_H

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <fcntl.h>
#include <sys/mman.h>
#include <nlohmann/json.hpp>

#define WIDTH 640
#define HEIGHT 480

class PoseVisualizer : public QMainWindow {
    Q_OBJECT

public:
    explicit PoseVisualizer(QWidget *parent = nullptr);
    ~PoseVisualizer();

private slots:
    void updateImages();

private:
    QLabel *imageLabel1, *imageLabel2;
    uchar *shm_cam1, *shm_cam2, *shm_kp;
};

#endif // POSEVISUALIZER_H
