#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <qopengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QColor>

#include <Maze/cell.h>
#include <Maze/maze.h>


// Classe dediee a l'affichage d'une scene OpenGL
class MazeWidget : public QGLWidget
{
    Q_OBJECT

public:

    // Constructeur
    MazeWidget(QWidget * parent = nullptr);

    // Fonction de gestion d'interactions clavier
    void keyPressEventCall(QKeyEvent * event);

    // Fonction du labyrinthe
    void majVue(char command);

    // debug
    void drawCube();

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
    int angleVue = 0;
    double pos_x = 0, pos_y = 0, pos_z = 0;

    Maze laby;

public slots:

    void updateView(char command);

signals:
    void onArrete();

};

#endif // MYGLWIDGET_H
