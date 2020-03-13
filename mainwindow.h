#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "opencv2/opencv.hpp"
#include <QTimer>
//using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    cv::Mat image;
private slots:
    void updateCV();
signals:
    void updateGLWidget(char a);


private:
    Ui::MainWindow *ui;
    cv::VideoCapture * webCam_;

    // Timer d'animation
    float m_TimeElapsed { 0.0f };
    QTimer timer;

    // camera parameters
    int width;
    int height;
};

#endif // MAINWINDOW_H
