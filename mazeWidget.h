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


// Classe dediee a l'affichage d'une scene OpenGL
class MazeWidget : public QGLWidget
{
    Q_OBJECT

public:

    // Constructeur
    MazeWidget(QWidget * parent = nullptr);

    // Fonction de gestion d'interactions clavier
    void keyPressEventCall(QKeyEvent * event);

protected:

    // Fonction d'initialisation
    void initializeGL();

    // Fonction de redimensionnement
    void resizeGL(int width, int height);

    // Fonction d'affichage
    void paintGL();

    //Fonction du labyrinthe
    void majVue(char command);
private:

    int width, height;
    float pos_x = 0, pos_y = 0, pos_z = 0;

public slots:
    void updateView(char command);

};

#endif // MYGLWIDGET_H
