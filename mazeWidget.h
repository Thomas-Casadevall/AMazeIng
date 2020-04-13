#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <qopengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QColor>
#include <QGraphicsOpacityEffect>
#include <QTime>
#include <Maze/cell.h>
#include <Maze/maze.h>
#include "minimap.h"


// Classe dediee a l'affichage d'une scene OpenGL
// Auteur : Thomas Casadevall
class MazeWidget : public QOpenGLWidget
{
    Q_OBJECT

public:

    // Constructeur
    MazeWidget(QWidget * parent = nullptr);
    // Destructeur
    ~MazeWidget();

    // Events
    void keyPressEventCall(QKeyEvent * event);

    /*
     * Role : déplace le joueur en fonction du char en entrée et met à jour l"affichage
     * Entrée : char command = défini le déplacement à faire : f = forward, b = backward, l = left, r = right sinon affiche erreur
     * Sortie : vide
     * */
    void majVue(char command);

    /*
     * Role : gère la fin de partie en remettant tous les paramètres à leur valeur de défaut, regénèrant le labyrinthe et l'affichant
     * Entrée : vide
     * Sortie : vide
     * */
    void finDePartie();

    /*
     * Role : calcule le déplacement selon x en fonction de l'angle de vu et du pas de déplacement
     * Entrée : vide
     * Sortie : double = le déplacement selon x
     * */
    double angle2x();

    /*
     * Role : calcule le déplacement selon y en fonction de l'angle de vu et du pas de déplacement
     * Entrée : vide
     * Sortie : double = le déplacement selon y
     * */
    double angle2z();

    int getPosX()const{return pos_x;};
    int getPosY()const{return pos_y;};
    int getPosZ()const{return pos_z;};
    int getAngleVue()const{return angleVue;};

    // pointer vers le widget de minimap
    MiniMap *miniMap;

    // l'heure actuelle
    QDateTime CurrentTimeRef;

protected:

    /*
     * Role : initialise l'affichage en activant certains buffers, définissant et activant les sources de lumière, réglant la couleur de fond
     * Entrée : vide
     * Sortie : vide
     * */
    void initializeGL();

    /*
     * Role : gère le redimentionnement de la fenêtre, elle est appelée automatiquement
     * Entrée : - int width = largeur, donné par Qt
     *          - int height = heureur, donné par Qt
     * Sortie : vide
     * */
    void resizeGL(int width, int height);

    /*
     * Role : affiche le labyrinthe et la minimap, elle est automatiquement appelé par Qt, ou lorsque l'on appelle repaint()
     * Entrée : vide
     * Sortie : vide
     * */
    void paintGL();

private:

    // taille de la fenetre
    int width, height;

    // largeur des murs et d'un cellule
    float l = 0.2, c = 3;

    // position du joueur
    int angleVue = 0;
    double pos_x = 0, pos_y = 0, pos_z = 0;

    // hauteur de la caméra
    double y = 0;

    // le labytrinthe
    Maze laby;

    // taille du labyrinthe
    int width_l = 10, height_l = 6;

    // pour mettre la minimap en semi-transparent
    QGraphicsOpacityEffect * effect;




public slots:

    // inutilisé
    void newGame();

    /*
     * Role : connecté à un signal de MainWindow, gère le déplacement en fonctiond de la commande et met à jour l'affichage
     * Entrée : char command = défini le déplacement à faire : f = forward, b = backward, l = left, r = right sinon affiche erreur
     * Sortie : vide
     * */
    void updateView(char command);

signals:
    // connecté à un slot de MainWindow, arrête l'application
    void onArrete();

};

#endif // MYGLWIDGET_H
