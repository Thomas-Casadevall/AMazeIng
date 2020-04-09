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

    double angle2x();
    double angle2z();

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
    float l = 0.1, c = 2;

    int angleVue = 0;
    double pos_x = 0, pos_y = 0, pos_z = 0;
    double y = 0;

    Maze laby;
    MiniMap *miniMap;

    QGraphicsOpacityEffect * effect;

public slots:

    void updateView(char command);

signals:
    void onArrete();

};

#endif // MYGLWIDGET_H
