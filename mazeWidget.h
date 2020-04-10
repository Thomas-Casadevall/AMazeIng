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

    // Fonction du labyrinthe
    void majVue(char command);

    void finDePartie();

    double angle2x();
    double angle2z();
    int getPosX()const{return pos_x;};
    int getPosY()const{return pos_y;};
    int getPosZ()const{return pos_z;};
    int getAngleVue()const{return angleVue;};

    MiniMap *miniMap;
    QDateTime CurrentTimeRef;

protected:

    // Fonction d'initialisation
    void initializeGL();

    // Fonction de redimensionnement
    void resizeGL(int width, int height);

    // Fonction d'affichage
    void paintGL();

private:

    int width, height;

    // largeur des murs et d'un cellule
    float l = 0.2, c = 3;

    int angleVue = 0;
    double pos_x = 0, pos_y = 0, pos_z = 0;
    double y = 0;

    Maze laby;
    int width_l = 10, height_l = 6;



    QGraphicsOpacityEffect * effect;




public slots:
    void newGame();
    void updateView(char command);

signals:
    void onArrete();

};

#endif // MYGLWIDGET_H
