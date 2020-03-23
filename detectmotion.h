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
    int subImageWidth=100;
    int subImageHeight=100;
    int templateWidth=25;
    int templateHeight=25;
    cv::Rect workingRect,templateRect;
    cv::Point workingCenter;

    cv::CascadeClassifier face_cascade;
protected:
private:


};



cv::Mat DetectMotion(cv::VideoCapture * webCam_, cv::Mat image1,Properties props);



