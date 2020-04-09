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


Mat DetectMotion(cv::VideoCapture * webCam_,Mat imageReference,Properties props)
{

            //----------------------------------------------------//
            // Verification de l'ouverture correcte de la camera  //
            //----------------------------------------------------//
    imshow("WebCam", props.imageReduiteNezReference);

    if(!webCam_->isOpened())  // check if we succeeded
    {
    std::cout<<"Error openning the default camera"<<std::endl;
        Mat image2;
        return image2;
    }



    //Definition des Matrices utilisées
    Mat image2, image_gray_Capture , imageReduiteReference,imageReduiteCapture2 , imageReduiteCapture,imageReduiteTest;//imageReduiteNezReference;


    // Get imageReference // Passé en paramètre -> Image de reference
    //webCam_->read(imageReference);


    //----------------------------------------------------//
    //    Affichage du visage sur l'image de reference    //
    //----------------------------------------------------//

    /*
    if (facesRef.size()>0)
    {
    // Draw blue rectangle
    for (int i=0;i<(int)facesRef.size();i++)
      rectangle(imageReference,facesRef[i],Scalar(255,255,0),2);
    }
    */




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

               workingRectCapture = facesCapture[1];
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




        matchTemplate(image_gray_Capture, props.imageReduiteNezReference, resultImage, TM_CCORR_NORMED );
        // Localize the best match with minMaxLoc
        double minVal; double maxVal; Point minLoc; Point maxLoc;
        minMaxLoc( resultImage, &minVal, &maxVal, &minLoc, &maxLoc);

        // Compute the translation vector between the origin and the matching rect
        Point vect(maxLoc.x-WorkingRectNez.x,maxLoc.y-WorkingRectNez.y);

        // Draw green rectangle and the translation vector
        //rectangle(frame2,workingRect,Scalar( 0, 255, 0),2);
        Point p(props.workingCenter.x+vect.x,props.workingCenter.y+vect.y);
        //Point p(workingCenterCapture.x+vect.x , workingCenterCapture.y+vect.y);
        arrowedLine(image2,props.workingCenter,p,Scalar(255,255,255),2);
        std::cout << vect << std::endl << std::endl;
/*
        if (vect.y <= -15 && props.valTest == 1){
            Rect WorkingRectNez2 = Rect (workingCenterCapture.x - workingRectCapture.width/4 , workingCenterCapture.y - workingRectCapture.width/4, workingRectCapture.height/2 ,workingRectCapture.width/2);
            props.imageReduiteNezReferenceTampon = props.imageReduiteNezReference;
            cv::cvtColor(Mat(image2,WorkingRectNez2),props.imageReduiteNezReference,COLOR_BGR2GRAY);
            props.valTest=0;
        }
        if (vect.y > -15 && props.valTest==0 ){
            props.valTest=1;
            props.imageReduiteNezReference = props.imageReduiteNezReferenceTampon;
        }

*/




















/*
         //affichage Image reduite
          if (1==1){
            //destroyWindow("WebCam");
            imshow("WebCam", imageReduiteNezReference);
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
    rectangle(image2,WorkingRectNez,Scalar( 0, 255, 0),2);
    rectangle(image2,props.workingRect,Scalar( 0, 255, 0),2);
    //rectangle(image2,workingRectCapture,Scalar( 255, 255, 255),2);

        // Display image2
        //imshow("WebCam", image2);

        // Swap matrixes
        //swap(imageReduiteReference,imageReduiteCapture);

    // the camera will be deinitialized automatically in VideoCapture destructor
    return image2;
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






