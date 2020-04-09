#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "QWidget"
//#include "QLabel"
#include <QPixmap>
#include <iostream>
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

    // Connexion du timer à la MaJ de MazeWidget
    connect(this,  &MainWindow::updateGLWidget, ui->maze,    &MazeWidget::updateView);


    // Camera setup
    webCam_ =new cv::VideoCapture(0);
    width = webCam_->get(CAP_PROP_FRAME_WIDTH);
    height= webCam_->get(CAP_PROP_FRAME_HEIGHT);
    ui->imageLabel_->setText("L'image va bientôt s'afficher, veuillez patienter!");


    //
    props = new Properties (webCam_);
    initCV();
}

MainWindow::~MainWindow()
{
    delete webCam_;
    delete props;
    delete ui;
}


void MainWindow::updateCV(){

    if (webCam_->read(image1)) {   // Capture a frame

        image2 = DetectMotion(webCam_,ImageReference,*props);
        //webCam_->read(image2);

        // Flip to get a mirror effect - Useless, done inside the function
        flip(image2,image2,1);


        // Invert Blue and Red color channels
        cv::cvtColor(image2,image2,COLOR_BGR2RGB);

        // Convert to Qt image
        QImage img= QImage((const unsigned char*)(image2.data),image2.cols,image2.rows,QImage::Format_RGB888);

        //Fonctions regroupées dans la section Resize.
        //img.scaledToHeight(ui->imageLabel_->size().height());
        //img.scaledToWidth(ui->imageLabel_->size().width());


        ui->imageLabel_->setPixmap(QPixmap::fromImage(img));
        // Resize the label to fit the image


        ui->imageLabel_->resize(ui->imageLabel_->pixmap()->size());

        //img.scaled(ui->imageLabel_->size().height(),ui->imageLabel_->size().width(),Qt::KeepAspectRatio);


    }
    else {
        ui->imageLabel_->setText("Error capturing the frame");
    }

}

void MainWindow::keyPressEvent(QKeyEvent * event){

    ui->maze->keyPressEventCall(event);

}

void MainWindow::initCV(){
    do{
       webCam_->read(ImageReference);
       // Mirror effect
       //cv::flip(ImageReference,ImageReference,1);

       //----------------------------------------------------//
       //    Detection du visage sur l'image de reference    //
       //----------------------------------------------------//

       cv::cvtColor(ImageReference,image_gray_Reference,COLOR_BGR2GRAY);
       // Equalize graylevels
       //        equalizeHist( frame_gray, frame_gray );

       //-- Detect faces
          facesRef.clear();

          props->face_cascade.detectMultiScale( image_gray_Reference, facesRef, 1.1, 4, 0, Size(60, 60) );

       props->workingRect = facesRef[1];


       props->subImageWidth = props->workingRect.width;
       props->subImageHeight = props->workingRect.height;
       props->workingCenter = Point (props->workingRect.x+props->subImageWidth/2,props->workingRect.y+props->subImageHeight/2);

       std::cout << "taille facesRef " <<facesRef.size()<< ' ';

       for (int i = 0; i < (int)facesRef.size(); i++) {
           std::cout << "facesRef " <<facesRef.at(i)<< ' ';
       }

    }while (facesRef.size()==0);





    props->WorkingRectNez = Rect (props->workingCenter.x - props->workingRect.width/4 , props->workingCenter.y - props->workingRect.width/4, props->workingRect.height/2 ,props->workingRect.width/2);
    cv::cvtColor(Mat(ImageReference,props->WorkingRectNez),props->imageReduiteNezReference,COLOR_BGR2GRAY);










}


void MainWindow::on_pushButton_clicked()
{
    initCV();
}
