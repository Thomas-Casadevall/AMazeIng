#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "QWidget"
//#include "QLabel"
#include <QPixmap>
#include <iostream>
#include <QTime>
#include <QDebug>

using namespace cv;
MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),    ui(new Ui::MainWindow) {

    ui->setupUi(this);
    setFixedSize(670,1050);
    ui->tempsDeJeuLabel_->setText(QTime::currentTime().toString());
    //startTime = QTime::currentTime();
    startTime = QDateTime::currentDateTime();
    timer.setInterval(10);
    timer.start();

    // Connexion du timer à la MaJ d'OpenCV
    connect(&timer,  &QTimer::timeout, this,    [&]{ this->startMultiThreadProcess(mutx); });

    // Connexion d'UpdateView à la MaJ de MazeWidget
    connect(this,  &MainWindow::updateGLWidget, ui->maze,    &MazeWidget::updateView);

    connect(ui->maze, &MazeWidget::onArrete, [&] {exit(0);});


    // Camera setup
    webCam_ =new cv::VideoCapture(0);
    width = webCam_->get(CAP_PROP_FRAME_WIDTH);
    height= webCam_->get(CAP_PROP_FRAME_HEIGHT);
    ui->imageLabel_->setText("L'image va bientôt s'afficher, veuillez patienter!");
    ui->statutLabel_->setText("Démarrage...");


    props = new Properties (webCam_);
    initCV();

}

MainWindow::~MainWindow()
{
    delete webCam_;
    delete props;
    delete ui;

}

int MainWindow::startMultiThreadProcess(mutex &m){

    QString TmpDeJeu = calculTempsDeJeu(startTime);
    ui->tempsDeJeuLabel_->setText(TmpDeJeu);

    //ui->tempsDeJeuLabel_->setText(startTime-QTime::currentTime().toString());
    std::thread th1([&]{this->updateCV();});
    qint64 millisecondsDiff = ui->maze->CurrentTimeRef.msecsTo(QDateTime::currentDateTime());
    if (props->flagMajMaze == 1 || (millisecondsDiff>1500)){

        std::thread th2([&]{this->updateGLWidget(props->deplacementAExecuter);});

        th2.join();

        props->flagMajMaze = 0;
        props->deplacementAExecuter = '0';


       }



    th1.join();
    return(0);


}


QString calculTempsDeJeu(QDateTime startTime){
    QDateTime CurrentTime = QDateTime::currentDateTime();
    qint64 millisecondsDiff = startTime.msecsTo(CurrentTime);
    qint64 minutes = floor(millisecondsDiff/60000);
    qint64 secondes = floor(millisecondsDiff/1000);
    secondes = secondes - minutes*60;
    QString test = QString::number(minutes);
    QString test2 = QString::number(secondes);
    QString test3 = "Temps de jeu : "+test+" min "+test2+" sec";
    return test3;
}



void MainWindow::updateCV(){

    if (webCam_->read(image1)) {   // Capture a frame
        std::pair<Mat, int> myPair;
        myPair = DetectMotion(webCam_,ImageReference,*props);
        image2 = myPair.first;
        codeSortie = myPair.second;
        //webCam_->read(image2);

        // Flip to get a mirror effect - Useless, done inside the function
        flip(image2,image2,1);


        // Invert Blue and Red color channels
        cv::cvtColor(image2,image2,COLOR_BGR2RGB);

        // Convert to Qt image
        QImage img= QImage((const unsigned char*)(image2.data),image2.cols,image2.rows,QImage::Format_RGB888);


        ui->imageLabel_->setPixmap(QPixmap::fromImage(img));

        // Resize the label to fit the image
        ui->imageLabel_->resize(ui->imageLabel_->pixmap()->size());




    }
    else {
        ui->imageLabel_->setText("Error capturing the frame");
    }


    qint64 millisecondsDiff = ui->maze->CurrentTimeRef.msecsTo(QDateTime::currentDateTime());
    //qDebug()<<millisecondsDiff;
    if (millisecondsDiff>1500){
        ui->maze->miniMap->affichageAutorise=0;



    }
    else
        ui->maze->miniMap->affichageAutorise=1;



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
        props->workingCenter = cv::Point (props->workingRect.x+props->subImageWidth/2,props->workingRect.y+props->subImageHeight/2);
        props->chgtPosX(props->workingCenter.x);
        props->chgtPosY(props->workingCenter.y);
        std::cout << "taille facesRef " <<facesRef.size()<< ' ';

        for (int i = 0; i < (int)facesRef.size(); i++) {
            std::cout << "facesRef " <<facesRef.at(i)<<' ';
        }

    }while (facesRef.size()==0);





    props->WorkingRectNez = Rect (props->workingCenter.x - props->workingRect.width/4 , props->workingCenter.y - props->workingRect.width/4, props->workingRect.height/2 ,props->workingRect.width/2);
    Mat tampon;
    cv::cvtColor(Mat(ImageReference,props->WorkingRectNez),tampon,COLOR_BGR2GRAY);
    props->chgtNezRef(tampon);


}


void MainWindow::on_pushButton_clicked()
{
    initCV();
}

int MainWindow::checkCollide(int posX,int posY, int posZ){

    int ok = 0;
    if (ok == 1)
        return(1);
    else
        return(0);


}
