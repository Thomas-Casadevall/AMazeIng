#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "QWidget"
//#include "QLabel"
#include <QPixmap>
#include <QDebug>


using namespace cv;
MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),    ui(new Ui::MainWindow) {

    ui->setupUi(this);
    setFixedSize(670,1050);

    timer.setInterval(10);
    timer.start();

    // Connexion du timer à la MaJ d'OpenCV
    //connect(&timer,  &QTimer::timeout, this,    &MainWindow::updateCV);

    //connexion du exit
    connect(ui->maze, &MazeWidget::onArrete, this,  &MainWindow::quitter);
    connect(this,  &MainWindow::updateGLWidget, ui->maze,    &MazeWidget::updateView);


    // Camera setup
    webCam_ =new cv::VideoCapture(0);
    width = webCam_->get(CAP_PROP_FRAME_WIDTH);
    height= webCam_->get(CAP_PROP_FRAME_HEIGHT);
    ui->imageLabel_->setText("L'image va bientôt s'afficher, veuillez patienter!");


    //
    props = new Properties (webCam_);


}

MainWindow::~MainWindow()
{
    delete webCam_;
    delete props;
    delete ui;
}


void MainWindow::updateCV(){

    if (webCam_->read(image1)) {   // Capture a frame

        image2 = DetectMotion(webCam_,image1,*props);
        // Flip to get a mirror effect
        // flip(image2,image2,1);

        // Invert Blue and Red color channels
        cv::cvtColor(image2,image2,COLOR_BGR2RGB);
        // Convert to Qt image



        QImage img= QImage((const unsigned char*)(image2.data),image2.cols,image2.rows,QImage::Format_RGB888);
        //img.scaledToHeight(ui->imageLabel_->size().height());
        //img.scaledToWidth(ui->imageLabel_->size().width());


        ui->imageLabel_->setPixmap(QPixmap::fromImage(img));
        // Resize the label to fit the image
        ui->imageLabel_->resize(ui->imageLabel_->pixmap()->size());
        img.scaled(ui->imageLabel_->size().height(),ui->imageLabel_->size().width(),Qt::KeepAspectRatio);


    }
    else {
        ui->imageLabel_->setText("Error capturing the frame");
    }

}

void MainWindow::keyPressEvent(QKeyEvent * event){

    ui->maze->keyPressEventCall(event);

}

void MainWindow::quitter(){
    close();
}
