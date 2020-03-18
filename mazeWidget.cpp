#define _USE_MATH_DEFINES

#include "mazeWidget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <cmath>
#include <random>
#include <QDebug>


// Declarations des constantes
const unsigned int WIN_WIDTH  = 1600;
const unsigned int WIN_HEIGHT = 900;
const float MAX_DIMENSION     = 50.0f;


// Constructeur
MazeWidget::MazeWidget(QWidget * parent) : QGLWidget(parent)
{
    // Reglage de la taille/position
    width = WIN_WIDTH;
    height = WIN_HEIGHT;
    resize(WIN_WIDTH, WIN_HEIGHT);
    move(QApplication::desktop()->screen()->rect().center() - rect().center());
}


// Fonction d'initialisation
void MazeWidget::initializeGL()
{
    // Reglage de la couleur de fond
    glClearColor(0.0, 0.0, 0.0, 1.0); // Couleur à utiliser lorsqu’on va nettoyer la fenêtre ( = le fond)

    // Activation du zbuffer
    glEnable(GL_DEPTH_TEST);

}


// Fonction de redimensionnement
void MazeWidget::resizeGL(int w, int h)
{
    // Definition du viewport (zone d'affichage)
    glViewport(0, 0, w, h);

    width = w;
    height = h;

}


// Fonction d'affichage
void MazeWidget::paintGL()
{
    // Reinitialisation des tampons
    glClear(GL_COLOR_BUFFER_BIT); // Effacer le buffer de couleur

    glClear(GL_DEPTH_BUFFER_BIT); // clear le Z buffer

    // Definition de la matrice modelview
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0f, ((float)width)/height, 0.1f, 20.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(  pos_x, pos_y, pos_z,    // position de la caméra
                10, 0, 10,  // position du point que fixe la caméra
                0, 1, 0);   // vecteur vertical

    glBegin(GL_QUADS);

    glColor3ub(0, 0, 255); // Bleu
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glColor3ub(255, 0, 255); // Magenta
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glColor3ub(255, 255, 0); // Jaune
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glColor3ub(0, 255, 255); // Cyan
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glEnd();

}


// Fonction de gestion d'interactions clavier
void MazeWidget::keyPressEventCall(QKeyEvent * event)
{
    switch(event->key())
    {
        // Rien
        case Qt::Key_Enter:
            qDebug()<<"enter";
            majVue('f');
        break;

        // Sortie de l'application
        case Qt::Key_Escape:
            exit(0);

        case Qt::RightArrow:
            majVue('r');
        break;

        case Qt::LeftArrow:
            majVue('l');
        break;

        case Qt::DownArrow:
            majVue('b');
        break;

        case Qt::UpArrow:
            qDebug()<<"up";
            majVue('f');
        break;

        // Cas par defaut
        default:
            // Ignorer l'evenement
            qDebug()<<"event ignoré";
            event->ignore();
        break;

        return;
    }

    // Acceptation de l'evenement et mise a jour de la scene
    event->accept();
    updateGL();
}

void MazeWidget::updateView(char command){

    switch (command) {

    case 'f':
        pos_x += 0.1;
        updateGL();
    break;

    case 'b':
        pos_x -= 0.1;
        updateGL();
    break;

    case 'l':
        updateGL();
    break;

    case 'r':
        updateGL();
    break;

    default:
        qDebug() << "error";
    break;
    }

}

void MazeWidget::majVue(char command){

    switch (command) {

    case 'f':
        pos_x += 0.1;
        updateGL();
    break;

    case 'b':
        pos_x -= 0.1;
        updateGL();
    break;

    case 'l':
        updateGL();
    break;

    case 'r':
        updateGL();
    break;

    default:
        qDebug() << "error";
    break;
    }

}
