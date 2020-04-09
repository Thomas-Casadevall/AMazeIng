#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "opencv2/opencv.hpp"
#include <QTimer>
//using namespace cv;
#include "detectmotion.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    cv::Mat image2;
    cv::Mat image1;
    cv::Mat ImageReference;
    cv::Mat image_gray_Reference;
    std::vector<cv::Rect> facesRef;
    int codeSortie;

private slots:
    void updateCV();
    void initCV();
    void on_pushButton_clicked();

signals:
    void updateGLWidget(char a);

protected:
    // Fonction de gestion d'interactions clavier
    void keyPressEvent(QKeyEvent * event);

private:
    Ui::MainWindow *ui;
    cv::VideoCapture * webCam_;

    // Timer d'animation
    float m_TimeElapsed { 0.0f };
    QTimer timer;

    // camera parameters
    int width;
    int height;
    Properties * props;
};

#endif // MAINWINDOW_H
