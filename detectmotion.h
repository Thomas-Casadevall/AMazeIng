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
    ~Properties();
    Properties(Properties const& p) =delete;
    Properties& operator=(Properties const& p) = delete;
    Properties(Properties&&) = delete;
    Properties& operator=(Properties&&) = delete;

    // A passer en private
    int frameWidth=320;
    int frameHeight=240;
    int subImageWidth=10;
    int subImageHeight=10;
    int templateWidth=25;
    int templateHeight=25;
    cv::Rect workingRect,templateRect,WorkingRectNez ;
    cv::Point workingCenter;
    double oldVectX=0,oldVectY=0;
    int flagMajMaze=0;
    char deplacementAExecuter = '0';

    int valTest = 1;

    cv::CascadeClassifier face_cascade,nose_cascade;

    void chgtNezRef(cv::Mat Image);
    void chgtNezRefTmp(cv::Mat Image);
    cv::Mat getNez()const{return imageReduiteNezReference;};
    cv::Mat getNezTmp()const{return imageReduiteNezReferenceTampon;};
    int getPosX()const{return PosX;};
    int getPosY()const{return PosY;};
    void chgtPosX(int NewX){PosX = NewX;};
    void chgtPosY(int NewY){PosY = NewY;};
    void CheckMove(cv::Point vecteur);

protected:
private:
    cv::Mat imageReduiteNezReference;
    cv::Mat imageReduiteNezReferenceTampon;
    double PosX,PosY;




};



std::pair<cv::Mat,int> DetectMotion(cv::VideoCapture * webCam_, cv::Mat imageReference,Properties & props);



