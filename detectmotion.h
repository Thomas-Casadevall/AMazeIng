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
    Properties(cv::VideoCapture * webCam_); //Constructeur
    ~Properties(); //Destructeur
    Properties(Properties const& p) =delete;
    Properties& operator=(Properties const& p) = delete;
    Properties(Properties&&) = delete;
    Properties& operator=(Properties&&) = delete;

    // A passer en private
    //Propriétés de la camera et des sous-images
    int frameWidth=320;
    int frameHeight=240;
    int subImageWidth=10;
    int subImageHeight=10;
    int templateWidth=25;
    int templateHeight=25;

    cv::Rect workingRect,templateRect,WorkingRectNez ; //Rectangles associés au visage et au nez de l'image de reference
    cv::Point workingCenter; // Centre du rectangle associé au visage detecté
    double oldVectX=0,oldVectY=0; //Inutilisés, implantation non satisfaisante, à supprimer du code.
    int flagMajMaze=0; // Flag de mise a jour de l'affichage du labyrinthe. Si = 1, il y aura mise à jour au prochain déclanchement de la fonction.
    char deplacementAExecuter = '0'; // Variable de transmission de l'ordre de deplacement aux classes de MazeWidget.

    int valTest = 1; // Variable de debug

    cv::CascadeClassifier face_cascade,nose_cascade; // Classifieurs Cascade, seul le face_cascade est toujours utilisé.

    void chgtNezRef(cv::Mat Image); // Fonction de mise a jour de l'imagette du nez de reference
    void chgtNezRefTmp(cv::Mat Image); // Fonction de mise a jour de l'imagette Tampon du nez de reference. Permet de stocker l'imagette de reference d'origine pour pouvoir la réutiliser en cas de retour dans la zone de reference.
    cv::Mat getNez()const{return imageReduiteNezReference;}; // Fonction d'obtention de l'imagette du nez de reference
    cv::Mat getNezTmp()const{return imageReduiteNezReferenceTampon;}; // Fonction d'obtention de l'imagette tampon du nez de reference

    int getPosX()const{return PosX;}; // Retournent Positions X et Y de l'imagette du nez obtenu en continu
    int getPosY()const{return PosY;};

    void chgtPosX(int NewX){PosX = NewX;}; // Met à jour les positions X et Y de l'imagette du nez obtenu en continu
    void chgtPosY(int NewY){PosY = NewY;};

    void CheckMove(cv::Point vecteur); //evalue le déplacement entre x,Y et le point workingRectCenter pour determiner s'il faut effectuer un déplacement.

protected:
private:
    cv::Mat imageReduiteNezReference;
    cv::Mat imageReduiteNezReferenceTampon;
    double PosX,PosY; // Positions X et Y de l'imagette du nez extraite (à evaluer pour determiner s'il faut effectuer un déplacement.)




};



std::pair<cv::Mat,int> DetectMotion(cv::VideoCapture * webCam_, cv::Mat imageReference,Properties & props); // Fonction de detection de déplacement, retourne les rectangles associés au visage de reference, nez de référence et visag detecté ajoutés à l'image obtenue via la camera
// Retourne egalement un entier nommé code de sortie, permettant le debug. Les paramettres principaux evalués sont mis à jour dans les attributs de Props (objet Properties).


