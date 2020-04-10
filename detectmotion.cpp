#include "detectmotion.h"
#include <QDebug>
using namespace cv;


Properties::Properties(cv::VideoCapture * webCam_){



    //workingRect = Rect ((frameWidth-subImageWidth)/2,frameHeight/2+(frameHeight/2-subImageHeight)/2,subImageWidth,subImageHeight);
    //templateRect = Rect ((workingRect.width-templateWidth)/2,(workingRect.height-templateHeight)/2,templateWidth,templateHeight);
    //workingCenter = Point (workingRect.x+subImageWidth/2,workingRect.y+subImageHeight/2);


    //std::cout<<"width :"<<webCam_->get(CAP_PROP_FRAME_WIDTH)<<endl;
    //std::cout<<"height :"<<webCam_->get(CAP_PROP_FRAME_HEIGHT)<<endl;

    webCam_->set(CAP_PROP_FRAME_WIDTH,frameWidth);
    webCam_->set(CAP_PROP_FRAME_HEIGHT,frameHeight);


    if( !face_cascade.load("../test/haarcascade_frontalface_alt.xml" ) )
    {
        std::cout<<"Error loading haarcascade"<<std::endl;
    }

    /*

    if( !nose_cascade.load("../test/nose.xml" ) )
    {
        std::cout<<"Error loading haarcascade"<<std::endl;
    }

    */
    


}
void Properties::chgtNezRef(Mat Image){
    imageReduiteNezReference = Image;
};
void Properties::chgtNezRefTmp(cv::Mat Image){
    imageReduiteNezReferenceTampon = Image;
};

Properties::~Properties(){
    std::cout<<"Error: Destructeur Propertise appelé";
}

void Properties::CheckMove(Point vecteur){
    if (abs(vecteur.x)>abs(vecteur.y)){
        if (abs(vecteur.x)<60 && abs(vecteur.x)>25 ){
            flagMajMaze = 1;
            if (vecteur.x>0){
                std::cout<<"gauche"<<std::endl;
                deplacementAExecuter = 'l';
            }

            else{
                std::cout<<"droite"<<std::endl;
                deplacementAExecuter = 'r';
            }
        }


    }
    else if (abs(vecteur.x)<abs(vecteur.y)){
        if (abs(vecteur.y)<60 && abs(vecteur.y)>25 ){
            flagMajMaze = 1;
            if (vecteur.y>0){
                std::cout<<"bas"<<std::endl;
                deplacementAExecuter = 'f';
            }
            else{
                std::cout<<"haut/avant"<<std::endl;
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
    imshow("WebCam", props.getNez());

    if(!webCam_->isOpened())  // check if we succeeded
    {
        std::cout<<"Error openning the default camera"<<std::endl;
        Mat image2;
        return {image2,2};
    }



    //Definition des Matrices utilisées
    Mat image2, image2copy, image_gray_Capture , imageReduiteReference,imageReduiteCapture2 , imageReduiteCapture,imageReduiteTest;//imageReduiteNezReference;


    // Get imageReference // Passé en paramètre -> Image de reference
    //webCam_->read(imageReference);


    //----------------------------------------------------//
    //    Affichage du visage sur l'image de reference    //
    //----------------------------------------------------//








    // Extract image of the size of workingRect and convert it to gray
    cv::cvtColor(Mat(imageReference,props.workingRect),imageReduiteReference,COLOR_BGR2GRAY);

    Rect WorkingRectNez = props.WorkingRectNez;// A passer en parametre pour plus de simplicité et moins de calculs

    //Rect WorkingRectNez = Rect (props.workingRect.x + props.workingRect.width/2.5,props.workingRect.y + props.workingRect.height/2.5, props.workingRect.height/2.5 ,props.workingRect.width/2.5);
    //Rect WorkingRectNez = Rect (0 ,0, props.workingRect.height/2.5 ,props.workingRect.width/2.5);

    //std::cout<<WorkingRectNez<<std::endl<<std::endl<<std::endl;





    //cv::cvtColor(Mat(imageReference,props.workingRect),imageReduiteTest,COLOR_BGR2GRAY);
    //Mat(imageReference,props.workingRect).copyTo(imageReduiteTest);


    //Mat(imageReference,props.workingRect).copyTo(imageReduiteReference);
    //Mat(imageReference,props.workingRect).copyTo(imageReduiteTest);

    // Create the matchTemplate image result

    Mat resultImage;   // to store the matchTemplate result
    int result_cols =  imageReference.cols-props.templateWidth  + 1;
    int result_rows =  imageReference.rows-props.templateHeight + 1;
    resultImage.create(result_cols, result_rows, CV_32FC1 );


    // Init output window
    //namedWindow("WebCam",1);

    // Get image2
    webCam_->read(image2);
    image2.copyTo(image2copy);
    // Mirror effect
    //cv::flip(image2,image2,1);

    // Extract working rect2 in image2 and convert it to gray
    Rect workingRectCapture;
    std::vector<Rect> facesCapture;

    cv::cvtColor(image2,image_gray_Capture,COLOR_BGR2GRAY);
    // Equalize graylevels
    //        equalizeHist( image_gray_Capture, image_gray_Capture );


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
            rectangle(image2,facesCapture[i],Scalar(255,255,255),3);
        }
        //std::cout <<"Size: " << facesCapture.size();
    }

    workingRectCapture = facesCapture[1];
    //workingRectCapture=props.workingRect;
    Point workingCenterCapture = Point (workingRectCapture.x+workingRectCapture.width/2,workingRectCapture.y+workingRectCapture.height/2);


    cv::cvtColor(Mat(image2,workingRectCapture),imageReduiteCapture,COLOR_BGR2GRAY);


    if (imageReduiteCapture.size() != imageReduiteReference.size() ){
        //std::cout<<imageReduiteReference.size()<<"     "<<imageReduiteCapture.size()<<std::endl;
        //imageReduiteCapture.resize(imageReduiteReference.size().width,imageReduiteReference.size().height);
        resize(imageReduiteCapture,imageReduiteCapture2, imageReduiteReference.size(), 0, 0,INTER_LINEAR );
        //std::cout<<imageReduiteReference.size()<<"     "<<imageReduiteCapture2.size()<<std::endl<<std::endl<<std::endl;

        //cv::hconcat(imageReduiteReference, imageReduiteCapture2, imageReduiteTest);
        //imshow("WebCam", imageReduiteTest);
        //cv::waitKey(1);

    }
    else{
        imageReduiteCapture.copyTo(imageReduiteCapture2);
    }



    // MATCHING  Du Nez sur l'image 2




    matchTemplate(image_gray_Capture, props.getNez(), resultImage, TM_CCORR_NORMED );
    // Localize the best match with minMaxLoc
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    minMaxLoc( resultImage, &minVal, &maxVal, &minLoc, &maxLoc);

    // Compute the translation vector between the origin and the matching rect
    Point vect(maxLoc.x-WorkingRectNez.x,maxLoc.y-WorkingRectNez.y);




    //---------------------------------------------------------------------------//
    //  Changement d'image de reference en cas de deplacement important selon Y; //
    //---------------------------------------------------------------------------//
    // double x = pow((vect.y-props.getPosY()),2);
    //double y = pow(vect.x-props.getPosX(),2);
    /*
     * double x = pow((vect.y-props.oldVectY),2);
    double y = pow(vect.x-props.oldVectX,2);
    double z = x+y;
    double test = std::sqrt(z);
    std::cout <<  "---" << test << "---" << std::endl;
    props.oldVectX = vect.x;
    props.oldVectY = vect.y;

    Point p(props.workingCenter.x+vect.x,props.workingCenter.y+vect.y);
    arrowedLine(image2,props.workingCenter,p,Scalar(255,255,255),2);
*/
    //std::cout <<  "-y-" << props.oldVectY << "---" << std::endl;
   // std::cout <<  "-x-" << props.oldVectX << "---" << std::endl;
    //if (sqrt(pow(vect.y-props.oldVectY,2)+pow(vect.x-props.oldVectX,2))<=30){

    props.oldVectX = vect.x;
    props.oldVectY = vect.y;

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

    imshow("WebCam2", props.getNez());
    // Draw green rectangle and the translation vector
    //rectangle(frame2,workingRect,Scalar( 0, 255, 0),2);
    Point p(props.workingCenter.x+vect.x,props.workingCenter.y+vect.y);
    //Point p(workingCenterCapture.x+vect.x , workingCenterCapture.y+vect.y);
    arrowedLine(image2,props.workingCenter,p,Scalar(255,255,255),2);
    std::cout << vect << std::endl << std::endl;
    props.CheckMove(vect);

















    /*
         //affichage Image reduite
          if (1==1){
            //destroyWindow("WebCam");
            imshow("WebCam", getNez());
            }

*/

    // ///////////////////////////////


    //----------------------------------------------------//
    //                 Detection des nez                  //
    //----------------------------------------------------//


    //std::vector<Rect> noseCapture;
    //noseCapture.clear();
    /*
        props.nose_cascade.detectMultiScale( imageReduiteCapture2, noseCapture, 1.1, 4, 0, Size(60, 60) );
        Rect noseRectCapture = noseCapture[1];

        //affichage
        rectangle(imageReduiteCapture2,noseRectCapture,Scalar( 0, 255, 0),2);
        //workingCenter = Point (workingRect.x+subImageWidth/2,workingRect.y+subImageHeight/2);
        //Point p(props.workingCenter.x+vect.x,props.workingCenter.y+vect.y);
        //arrowedLine(image2,props.workingCenter,p,Scalar(255,255,255),2);
        cv::hconcat(imageReduiteReference, imageReduiteCapture2, imageReduiteTest);
        imshow("WebCam", imageReduiteTest);
        cv::waitKey(1);


*/
    /*
         // Test0 non adapté
        // Extract template image in frame1
        Mat templateImage(imageReduiteReference,templateRect);
        // Do the Matching between the working rect in frame2 and the templateImage in frame1
        matchTemplate( imageReduiteCapture, templateImage, resultImage, TM_CCORR_NORMED );
        // Localize the best match with minMaxLoc
        double minVal; double maxVal; Point minLoc; Point maxLoc;
        minMaxLoc( resultImage, &minVal, &maxVal, &minLoc, &maxLoc);
        // Compute the translation vector between the origin and the matching rect
        Point vect(maxLoc.x-templateRect.x,maxLoc.y-templateRect.y);

        // Draw green rectangle and the translation vector
        rectangle(frame2,workingRect,Scalar( 0, 255, 0),2);
        Point p(workingCenter.x+vect.x,workingCenter.y+vect.y);
        arrowedLine(frame2,workingCenter,p,Scalar(255,255,255),2);
        */


    /*      // Test1
        // Extract template image in imageReference
        Mat templateImage(imageReduiteReference,props.templateRect);

        // Do the Matching between the working rect in image2 and the templateImage in imageReference
        matchTemplate( imageReduiteCapture, templateImage, resultImage, TM_CCORR_NORMED );

        // Localize the best match with minMaxLoc
        double minVal; double maxVal; Point minLoc; Point maxLoc;
        minMaxLoc( resultImage, &minVal, &maxVal, &minLoc, &maxLoc);

        // Compute the translation vector between the origin and the matching rect
        Point vect(maxLoc.x-props.templateRect.x,maxLoc.y-props.templateRect.y);

        // Draw green rectangle and the translation vector
        rectangle(image2,props.workingRect,Scalar( 0, 255, 0),2);
        Point p(props.workingCenter.x+vect.x,props.workingCenter.y+vect.y);
        arrowedLine(image2,props.workingCenter,p,Scalar(255,255,255),2);
*/
    //Affichage Nez
    rectangle(image2,WorkingRectNez,Scalar( 0, 255, 0),2);
    rectangle(image2,props.workingRect,Scalar( 0, 255, 0),2);


    //rectangle(image2,workingRectCapture,Scalar( 255, 255, 255),2);

    // Display image2
    //imshow("WebCam", image2);

    // Swap matrixes
    //swap(imageReduiteReference,imageReduiteCapture);

    // the camera will be deinitialized automatically in VideoCapture destructor
    //std::cout <<" test " << std::endl;
    return {image2,0};
    //return imageReduiteTest;




}























/*
 *
 * while (waitKey(5)<0)
    {
        // Get image2
        cap >> image2;
        // Mirror effect
        cv::flip(image2,image2,1);
        // Extract working rect in image2 and convert to gray
        cv::cvtColor(Mat(image2,workingRect),imageReduiteCapture,COLOR_BGR2GRAY);

        // Extract template image in imageReference
        Mat templateImage(imageReduiteReference,templateRect);
        // Do the Matching between the working rect in image2 and the templateImage in imageReference
        matchTemplate( imageReduiteCapture, templateImage, resultImage, TM_CCORR_NORMED );
        // Localize the best match with minMaxLoc
        double minVal; double maxVal; Point minLoc; Point maxLoc;
        minMaxLoc( resultImage, &minVal, &maxVal, &minLoc, &maxLoc);
        // Compute the translation vector between the origin and the matching rect
        Point vect(maxLoc.x-templateRect.x,maxLoc.y-templateRect.y);

        // Draw green rectangle and the translation vector
        rectangle(image2,workingRect,Scalar( 0, 255, 0),2);
        Point p(workingCenter.x+vect.x,workingCenter.y+vect.y);
        arrowedLine(image2,workingCenter,p,Scalar(255,255,255),2);

        // Display image2
        imshow("WebCam", image2);

        // Swap matrixes
        swap(imageReduiteReference,imageReduiteCapture);
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
    */






