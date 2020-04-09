#pragma once
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect.hpp>

#include <cstdio>
#include <iostream>
//using namespace cv;


class Properties{

public:
    Properties(cv::VideoCapture * webCam_);
    int frameWidth=320;
    int frameHeight=240;
    int subImageWidth=10;
    int subImageHeight=10;
    int templateWidth=25;
    int templateHeight=25;
    cv::Rect workingRect,templateRect,WorkingRectNez ;
    cv::Point workingCenter;
    cv::Mat imageReduiteNezReference;
    cv::Mat imageReduiteNezReferenceTampon;
    int PosX,PosY;


    int valTest = 1;

    cv::CascadeClassifier face_cascade,nose_cascade;
protected:
private:


};



cv::Mat DetectMotion(cv::VideoCapture * webCam_, cv::Mat imageReference,Properties props);



