#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "opencv2/opencv.hpp"
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

private slots:

private:
    Ui::MainWindow *ui;
    cv::VideoCapture * webCam_;

};

#endif // MAINWINDOW_H
