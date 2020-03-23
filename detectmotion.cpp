#include "detectmotion.h"
using namespace cv;


Properties::Properties(cv::VideoCapture * webCam_){



    workingRect = Rect ((frameWidth-subImageWidth)/2,frameHeight/2+(frameHeight/2-subImageHeight)/2,subImageWidth,subImageHeight);
    templateRect = Rect ((workingRect.width-templateWidth)/2,(workingRect.height-templateHeight)/2,templateWidth,templateHeight);
    workingCenter = Point (workingRect.x+subImageWidth/2,workingRect.y+subImageHeight/2);


    //std::cout<<"width :"<<webCam_->get(CAP_PROP_FRAME_WIDTH)<<endl;
    //std::cout<<"height :"<<webCam_->get(CAP_PROP_FRAME_HEIGHT)<<endl;

    webCam_->set(CAP_PROP_FRAME_WIDTH,frameWidth);
    webCam_->set(CAP_PROP_FRAME_HEIGHT,frameHeight);


    if( !face_cascade.load("../test/haarcascade_frontalface_alt.xml" ) )
    {
        std::cout<<"Error loading haarcascade"<<std::endl;
    }
    


}


Mat DetectMotion(cv::VideoCapture * webCam_,Mat image1,Properties props)
{

/*
    if(!webCam_->isOpened())  // check if we succeeded
    {
        cerr<<"Error openning the default camera"<<endl;
        Mat image2;
        return image2;
    }
*/

    Mat image2, frame_gray , frameRect1 , frameRect2;


    // Get image1
    //webCam_->read(image1); // Passé en paramètre
    // Mirror effect
    cv::flip(image1,image1,1);
    // Extract rect1 and convert to gray
    cv::cvtColor(Mat(image1,props.workingRect),frameRect1,COLOR_BGR2GRAY);
    //Mat(image1,rect).copyTo(frameRect1);

    // Create the matchTemplate image result
    Mat resultImage;   // to store the matchTemplate result
    int result_cols =  image1.cols-props.templateWidth  + 1;
    int result_rows =  image1.rows-props.templateHeight + 1;
    resultImage.create(result_cols, result_rows, CV_32FC1 );

    // Init output window
    //namedWindow("WebCam",1);

        // Get image2
        webCam_->read(image2);
        // Mirror effect
        cv::flip(image2,image2,1);



        // Extract working rect in image2 and convert to gray
        cv::cvtColor(Mat(image2,props.workingRect),frameRect2,COLOR_BGR2GRAY);


                //----------------------------------------------------//
                //      Detection du visage sur l'image capturée      //
                //----------------------------------------------------//

        cv::cvtColor(image2,frame_gray,COLOR_BGR2GRAY);
        // Equalize graylevels
//        equalizeHist( frame_gray, frame_gray );
        //-- Detect faces
        std::vector<Rect> faces;
        props.face_cascade.detectMultiScale( frame_gray, faces, 1.1, 4, 0, Size(60, 60) );
        if (faces.size()>0)
        {
            // Draw green rectangle
            for (int i=0;i<(int)faces.size();i++)
                rectangle(image2,faces[i],Scalar(0,255,0),2);
        }



        // ///////////////////////////////


        //----------------------------------------------------//
        //          Match du visage sur l'image neutre        //
        //----------------------------------------------------//

        /*
         // Test0 non adapté
        // Extract template image in frame1
        Mat templateImage(frameRect1,templateRect);
        // Do the Matching between the working rect in frame2 and the templateImage in frame1
        matchTemplate( frameRect2, templateImage, resultImage, TM_CCORR_NORMED );
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
        // Extract template image in image1
        Mat templateImage(frameRect1,props.templateRect);

        // Do the Matching between the working rect in image2 and the templateImage in image1
        matchTemplate( frameRect2, templateImage, resultImage, TM_CCORR_NORMED );

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


        // Display image2
        //imshow("WebCam", image2);

        // Swap matrixes
        swap(frameRect1,frameRect2);

    // the camera will be deinitialized automatically in VideoCapture destructor
    return image2;




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
        cv::cvtColor(Mat(image2,workingRect),frameRect2,COLOR_BGR2GRAY);

        // Extract template image in image1
        Mat templateImage(frameRect1,templateRect);
        // Do the Matching between the working rect in image2 and the templateImage in image1
        matchTemplate( frameRect2, templateImage, resultImage, TM_CCORR_NORMED );
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
        swap(frameRect1,frameRect2);
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
    */






