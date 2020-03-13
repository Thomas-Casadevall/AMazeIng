#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "QWidget"
//#include "QLabel"
#include <QPixmap>
#include "opencv2/opencv.hpp"
using namespace cv;
MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(670,1050);

    webCam_ =new cv::VideoCapture(0);
    int width= webCam_->get(CAP_PROP_FRAME_WIDTH);
    int height= webCam_->get(CAP_PROP_FRAME_HEIGHT);
    ui->imageLabel_->setText("zonedeTexte !");
    // Affichage de l'image camera
    Mat image;
    if (webCam_->read(image)) {   // Capture a frame
        // Flip to get a mirror effect
        flip(image,image,1);
        // Invert Blue and Red color channels
        cvtColor(image,image,COLOR_BGR2RGB);
        // Convert to Qt image



        QImage img= QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);
        //img.scaledToHeight(ui->imageLabel_->size().height());
        //img.scaledToWidth(ui->imageLabel_->size().width());


        ui->imageLabel_->setPixmap(QPixmap::fromImage(img));
        // Resize the label to fit the image
        ui->imageLabel_->resize(ui->imageLabel_->pixmap()->size());



        img.scaled(ui->imageLabel_->size().height(),ui->imageLabel_->size().width(),Qt::KeepAspectRatio);
        // Display on label


        //ui->imageLabel_->resize(ui->imageLabel_->pixmap()->size());
        //ui->openGLWidget->resize(ui->openGLWidget->size());



    }
    else {
        ui->imageLabel_->setText("Error capturing the frame");
    }



}

MainWindow::~MainWindow()
{
    delete ui;
}

