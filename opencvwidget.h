#ifndef WIDGET_H
#define WIDGET_H

#include "opencv2/opencv.hpp"
#include "mainwindow.h"
#include <QWidget>

using namespace cv;

namespace Ui {
class MainWindow;
}

class OpenCVWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OpenCVWidget(QWidget *parent = 0);
    ~OpenCVWidget();

private:
    Ui::MainWindow *ui;
    VideoCapture * webCam_;

private slots:
    void on_captureButton__clicked();
};

#endif // WIDGET_H
