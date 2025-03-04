#include <QApplication>
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

private:
    QLabel *imageLabel1, *imageLabel2;
    uchar *shm_cam1, *shm_cam2, *shm_kp;

public:
    PoseVisualizer(QWidget *parent = nullptr) : QMainWindow(parent) {
        imageLabel1 = new QLabel(this);
        imageLabel2 = new QLabel(this);
        QVBoxLayout *layout = new QVBoxLayout();
        layout->addWidget(imageLabel1);
        layout->addWidget(imageLabel2);
        QWidget *container = new QWidget();
        container->setLayout(layout);
        setCentralWidget(container);

        // Conectar con memoria compartida
        int fd1 = shm_open("shm_cam1", O_RDWR, 0666);
        int fd2 = shm_open("shm_cam2", O_RDWR, 0666);
        int fd_kp = shm_open("shm_keypoints", O_RDWR, 0666);

        shm_cam1 = (uchar *)mmap(NULL, WIDTH * HEIGHT * 3, PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);
        shm_cam2 = (uchar *)mmap(NULL, WIDTH * HEIGHT * 3, PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);
        shm_kp = (uchar *)mmap(NULL, 2048, PROT_READ | PROT_WRITE, MAP_SHARED, fd_kp, 0);

        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &PoseVisualizer::updateImages);
        timer->start(30);
    }

private slots:
    void updateImages() {
        cv::Mat img1(HEIGHT, WIDTH, CV_8UC3, shm_cam1);
        cv::Mat img2(HEIGHT, WIDTH, CV_8UC3, shm_cam2);

        std::string json_str(reinterpret_cast<char*>(shm_kp));
        auto keypoints = nlohmann::json::parse(json_str);

        // Dibujar keypoints en ambas imágenes
        for (int cam = 0; cam < 2; cam++) {
            std::string camKey = (cam == 0) ? "cam1" : "cam2";
            cv::Mat &img = (cam == 0) ? img1 : img2;

            for (auto &kp : keypoints[camKey].items()) {
                int x = kp.value()[0];
                int y = kp.value()[1];
                cv::circle(img, cv::Point(x, y), 5, cv::Scalar(0, 255, 0), -1);
            }
        }

        imageLabel1->setPixmap(QPixmap::fromImage(QImage(img1.data, WIDTH, HEIGHT, QImage::Format_RGB888)));
        imageLabel2->setPixmap(QPixmap::fromImage(QImage(img2.data, WIDTH, HEIGHT, QImage::Format_RGB888)));
    }
};
