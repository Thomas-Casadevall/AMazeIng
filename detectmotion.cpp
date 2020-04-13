#include "detectmotion.h"
#include <QDebug>
using namespace cv;


Properties::Properties(cv::VideoCapture * webCam_){


    webCam_->set(CAP_PROP_FRAME_WIDTH,frameWidth);
    webCam_->set(CAP_PROP_FRAME_HEIGHT,frameHeight);

    //Chargement des Haarcascades
    if( !face_cascade.load("../test/haarcascade_frontalface_alt.xml" ) )
    {
        std::cout<<"Error loading haarcascade"<<std::endl;
    }


    
}
void Properties::chgtNezRef(Mat Image){
    imageReduiteNezReference = Image;
};
void Properties::chgtNezRefTmp(cv::Mat Image){
    imageReduiteNezReferenceTampon = Image;
};

Properties::~Properties(){
    std::cout<<"Error: Destructeur Properties appelé";
}

void Properties::CheckMove(Point vecteur){
    if (abs(vecteur.x)>abs(vecteur.y)){
        if (abs(vecteur.x)<60 && abs(vecteur.x)>25 ){
            flagMajMaze = 1;
            if (vecteur.x>0){
                //std::cout<<"gauche"<<std::endl;
                deplacementAExecuter = 'l';
            }

            else{
                //std::cout<<"droite"<<std::endl;
                deplacementAExecuter = 'r';
            }
        }


    }
    else if (abs(vecteur.x)<abs(vecteur.y)){
        if (abs(vecteur.y)<60 && abs(vecteur.y)>15 ){
            flagMajMaze = 1;
            if (vecteur.y>0){
                //std::cout<<"bas"<<std::endl;
                deplacementAExecuter = 'f';
            }
            else{
                //std::cout<<"haut/avant"<<std::endl;
                deplacementAExecuter = 'b';
            }
        }

    }

}



std::pair<Mat, int> DetectMotion(cv::VideoCapture * webCam_,Mat imageReference,Properties & props)
{

    //----------------------------------------------------//
    // Verification de l'ouverture correcte de la camera  //
    //----------------------------------------------------//
    //imshow("WebCam", props.getNez());

    if(!webCam_->isOpened())  // check if we succeeded
    {
        std::cout<<"Error openning the default camera"<<std::endl;
        Mat image2;
        return {image2,2};
    }



    //Definition des Matrices utilisées
    Mat image2, image2copy, image_gray_Capture , imageReduiteReference,imageReduiteCapture2 , imageReduiteCapture;//imageReduiteNezReference;



    //----------------------------------------------------//
    //    extraction du visage sur l'image de reference   //
    //----------------------------------------------------//








    // Extract image of the size of workingRect and convert it to gray
    cv::cvtColor(Mat(imageReference,props.workingRect),imageReduiteReference,COLOR_BGR2GRAY);

    Rect WorkingRectNez = props.WorkingRectNez;// Rectangle correspondant au Nez de reference


    // Create the matchTemplate image result

    Mat resultImage;   // to store the matchTemplate result
    int result_cols =  imageReference.cols-props.templateWidth  + 1;
    int result_rows =  imageReference.rows-props.templateHeight + 1;
    resultImage.create(result_cols, result_rows, CV_32FC1 );



    // Get image2
    webCam_->read(image2);
    image2.copyTo(image2copy);
    // Mirror effect
    //cv::flip(image2,image2,1);

    // Extract working rect2 in image2 and convert it to gray
    Rect workingRectCapture;
    std::vector<Rect> facesCapture;

    cv::cvtColor(image2,image_gray_Capture,COLOR_BGR2GRAY);


    //----------------------------------------------------//
    //      Detection du visage sur l'image capturée      //
    //----------------------------------------------------//


    facesCapture.clear();

    props.face_cascade.detectMultiScale( image_gray_Capture, facesCapture, 1.1, 4, 0, Size(60, 60) );
    if (facesCapture.size()==0){
        return {image2,1};
    }

    if (facesCapture.size()>0)
    {
        // Draw green rectangle
        for (int i=0;i<(int)facesCapture.size();i++){
            rectangle(image2,facesCapture[i],Scalar(255,255,255),3); // Visage detecté
        }
        //std::cout <<"Size: " << facesCapture.size();
    }

    workingRectCapture = facesCapture[1];

    Point workingCenterCapture = Point (workingRectCapture.x+workingRectCapture.width/2,workingRectCapture.y+workingRectCapture.height/2);


    cv::cvtColor(Mat(image2,workingRectCapture),imageReduiteCapture,COLOR_BGR2GRAY);


    if (imageReduiteCapture.size() != imageReduiteReference.size() ){
        // Resize du visage pour avoir un point de comparaison correct
        resize(imageReduiteCapture,imageReduiteCapture2, imageReduiteReference.size(), 0, 0,INTER_LINEAR );


    }
    else{
        imageReduiteCapture.copyTo(imageReduiteCapture2);
    }



    // MATCHING Du Nez de reference sur l'image 2




    matchTemplate(image_gray_Capture, props.getNez(), resultImage, TM_CCORR_NORMED );
    // Localize the best match with minMaxLoc
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    minMaxLoc( resultImage, &minVal, &maxVal, &minLoc, &maxLoc);

    // Compute the translation vector between the origin and the matching rect
    Point vect(maxLoc.x-WorkingRectNez.x,maxLoc.y-WorkingRectNez.y);


    props.oldVectX = vect.x;
    props.oldVectY = vect.y;
   //Changement d'image de reference en cas de deplacement important selon Y;
    if (vect.y <= -10 && vect.y <= props.getPosY()-10){
        Rect WorkingRectNez2 = Rect (workingCenterCapture.x - workingRectCapture.width/4 , workingCenterCapture.y - workingRectCapture.width/4, workingRectCapture.height/2 ,workingRectCapture.width/2);
        if (props.valTest==1)
            props.chgtNezRefTmp(props.getNez());
        Mat test;
        cv::cvtColor(Mat(image2copy,WorkingRectNez2),test,COLOR_BGR2GRAY);
        props.chgtNezRef(test);
        props.chgtPosY(props.getPosY()-10);
        props.valTest=0;
    }
    else if (vect.y <= -10 && vect.y >= props.getPosY()+10){
        Rect WorkingRectNez2 = Rect (workingCenterCapture.x - workingRectCapture.width/4 , workingCenterCapture.y - workingRectCapture.width/4, workingRectCapture.height/2 ,workingRectCapture.width/2);
        if (props.valTest==1)
            props.chgtNezRefTmp(props.getNez());
        Mat test;
        cv::cvtColor(Mat(image2copy,WorkingRectNez2),test,COLOR_BGR2GRAY);
        props.chgtNezRef(test);
        props.chgtPosY(props.getPosY()+10);
        props.valTest=0;
    }

    else if (vect.y >= 10 && vect.y >= props.getPosY()+10){
        Rect WorkingRectNez2 = Rect (workingCenterCapture.x - workingRectCapture.width/4 , workingCenterCapture.y - workingRectCapture.width/4, workingRectCapture.height/2 ,workingRectCapture.width/2);
        if (props.valTest==1)
            props.chgtNezRefTmp(props.getNez());
        Mat test;
        cv::cvtColor(Mat(image2copy,WorkingRectNez2),test,COLOR_BGR2GRAY);
        props.chgtNezRef(test);
        props.chgtPosY(props.getPosY()+10);
        props.valTest=0;
    }
    else if (vect.y >= -10 && vect.y <= props.getPosY()-10){
        Rect WorkingRectNez2 = Rect (workingCenterCapture.x - workingRectCapture.width/4 , workingCenterCapture.y - workingRectCapture.width/4, workingRectCapture.height/2 ,workingRectCapture.width/2);
        if (props.valTest==1)
            props.chgtNezRefTmp(props.getNez());
        Mat test;
        cv::cvtColor(Mat(image2copy,WorkingRectNez2),test,COLOR_BGR2GRAY);
        props.chgtNezRef(test);
        props.chgtPosY(props.getPosY()-10);
        props.valTest=0;
    }

    //Changement d'image de reference en cas de deplacement important selon X;
    if (vect.x <= -10 && vect.x <= props.getPosX()-10){
        Rect WorkingRectNez2 = Rect (workingCenterCapture.x - workingRectCapture.width/4 , workingCenterCapture.y - workingRectCapture.width/4, workingRectCapture.height/2 ,workingRectCapture.width/2);
        if (props.valTest==1)
            props.chgtNezRefTmp(props.getNez());
        Mat test;
        cv::cvtColor(Mat(image2copy,WorkingRectNez2),test,COLOR_BGR2GRAY);
        props.chgtNezRef(test);
        props.chgtPosX(props.getPosX()-10);
        props.valTest=0;
    }
    else if (vect.x <= -10 && vect.x >= props.getPosX()+10){
        Rect WorkingRectNez2 = Rect (workingCenterCapture.x - workingRectCapture.width/4 , workingCenterCapture.y - workingRectCapture.width/4, workingRectCapture.height/2 ,workingRectCapture.width/2);
        if (props.valTest==1)
            props.chgtNezRefTmp(props.getNez());
        Mat test;
        cv::cvtColor(Mat(image2copy,WorkingRectNez2),test,COLOR_BGR2GRAY);
        props.chgtNezRef(test);
        props.chgtPosX(props.getPosX()+10);
        props.valTest=0;
    }

    else if (vect.x >= 10 && vect.x >= props.getPosX()+10){
        Rect WorkingRectNez2 = Rect (workingCenterCapture.x - workingRectCapture.width/4 , workingCenterCapture.y - workingRectCapture.width/4, workingRectCapture.height/2 ,workingRectCapture.width/2);
        if (props.valTest==1)
            props.chgtNezRefTmp(props.getNez());
        Mat test;
        cv::cvtColor(Mat(image2copy,WorkingRectNez2),test,COLOR_BGR2GRAY);
        props.chgtNezRef(test);
        props.chgtPosX(props.getPosX()+10);
        props.valTest=0;
    }
    else if (vect.x >= -10 && vect.x <= props.getPosX()-10){
        Rect WorkingRectNez2 = Rect (workingCenterCapture.x - workingRectCapture.width/4 , workingCenterCapture.y - workingRectCapture.width/4, workingRectCapture.height/2 ,workingRectCapture.width/2);
        if (props.valTest==1)
            props.chgtNezRefTmp(props.getNez());
        Mat test;
        cv::cvtColor(Mat(image2copy,WorkingRectNez2),test,COLOR_BGR2GRAY);
        props.chgtNezRef(test);
        props.chgtPosX(props.getPosX()-10);
        props.valTest=0;
    }



    if ((vect.y > -10 && vect.y < 10) && (vect.x > -10 && vect.x < 10)  && props.valTest==0){
        props.valTest=1;
        props.chgtNezRef(props.getNezTmp());
    }

    //}

    //imshow("WebCam2", props.getNez());
    // Draw green rectangle and the translation vector
    //rectangle(frame2,workingRect,Scalar( 0, 255, 0),2);
    Point p(props.workingCenter.x+vect.x,props.workingCenter.y+vect.y);
    //Point p(workingCenterCapture.x+vect.x , workingCenterCapture.y+vect.y);
    arrowedLine(image2,props.workingCenter,p,Scalar(255,255,255),2);
    //std::cout << vect << std::endl << std::endl;
    props.CheckMove(vect);















    rectangle(image2,WorkingRectNez,Scalar( 0, 255, 0),2); // rectangle de reference du nez
    rectangle(image2,props.workingRect,Scalar( 0, 255, 0),2); // visage de reference


    return {image2,0};
    //return imageReduiteTest;




}


















