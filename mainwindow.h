#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "opencv2/opencv.hpp"
#include <QTimer>
#include <QTime>
//using namespace cv;
#include "detectmotion.h"
#include <thread>
#include <mutex>


namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    cv::Mat image2;// Image capturée en continu
    cv::Mat image1; // Matrice non utilisée
    cv::Mat ImageReference; //Image de reference
    cv::Mat image_gray_Reference;//Image de reference en Noir et blanc
    std::vector<cv::Rect> facesRef; // Rectangles des visages detectés (Fait pour n'en utiliser qu'un, risque de bug si plusieurs visages detectés)
    int codeSortie; // Variable de debug obtenue en sortie de detection de mouvement (fct detectMotion présente dans Properties.h
    //void processUpdateCV(std::mutex &m);
    std::mutex mutx; // Implanté mais non utilisé puisque le programme n'est pas prévu pour renvoyer de sortie sur la console

private slots:
    void updateCV(); // Matthieu Poulain // Mise a jour de la detection de visage
    void initCV(); // Matthieu Poulain // Initialisation de la detection de visage
    void on_pushButton_clicked(); // Matthieu Poulain // Bouton de ré-initialisation de la detection de visage
    int startMultiThreadProcess(std::mutex &m); // Matthieu Poulain // Fonction de multi-threading, actualisant en parallèle la detection de visage sous OpenCV et l'affichage du labyrinthe lorsqu'une mise à jour est necessaire.


signals:
    void updateGLWidget(char a); // Thomas Casadevall // Signal de declenchement de la mise a jour de l'affichage du labyrinthe.

protected:
    // Fonction de gestion d'interactions clavier
    void keyPressEvent(QKeyEvent * event); // Thomas Casadevall //

private:
    Ui::MainWindow *ui;
    cv::VideoCapture * webCam_; // Accès Caméra

    // Timer d'animation
    float m_TimeElapsed { 0.0f }; // variable plus utilisée.
    QTimer timer; // Timer de mise a jour.

    // camera parameters
    int width;
    int height;
    Properties * props; // Objet properties, contenant toutes les variables liées à la caméra et les éléments nécessaires pour la détection de visage, notamment les images et rectangles de référence.
    int checkCollide(int posX,int posY, int posZ); // Changement de methode, n'est plus utilisé.
    QDateTime startTime; // Variable permetant d'avoir une reference temporelle pour la mise en place du chronometre du temps de jeu.
};




QString calculTempsDeJeu(QDateTime startTime);  // Matthieu Poulain // Mise en place du chronometre du temps de jeu.


#endif // MAINWINDOW_H
