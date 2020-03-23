#define _USE_MATH_DEFINES

#include "mazeWidget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <cmath>
#include <random>
#include <QDebug>

// PI
#define PI 3.14159265

// Declarations des constantes
const unsigned int WIN_WIDTH    = 1600;
const unsigned int WIN_HEIGHT   = 900;
const float MAX_DIMENSION       = 50.0f;
const unsigned int PAS_VUE      = 10;
const unsigned int PAS_DEPLACEMENT = 1;


// Constructeur
MazeWidget::MazeWidget(QWidget * parent) : QGLWidget(parent)
{
    // Reglage de la taille/position
    width = WIN_WIDTH;
    height = WIN_HEIGHT;
    resize(WIN_WIDTH, WIN_HEIGHT);
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    laby.generate();
}


// Fonction d'initialisation
void MazeWidget::initializeGL()
{
    // Reglage de la couleur de fond
    glClearColor(0.0, 0.0, 0.0, 1.0); // Couleur à utiliser lorsqu’on va nettoyer la fenêtre (= le fond)

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

    gluLookAt(  0, 0, 0,    // position de la caméra
                10, 0, 0,  // position du point que fixe la caméra
                0, 1, 0);   // vecteur vertical

    // déplacement et rotation
    glRotated(angleVue, 0, 1, 0);
    glTranslated(pos_x, pos_y, pos_z);

    laby.display3D();

}


// Fonction de gestion d'interactions clavier
void MazeWidget::keyPressEventCall(QKeyEvent * event)
{
    qDebug() << angleVue;
    switch(event->key())
    {
        // Rien
        case Qt::Key_Return:
            qDebug() << "Entrée";
        break;

        // Sortie de l'application
        case Qt::Key_Escape:
            onArrete();

        case Qt::Key_Right:
            majVue('r');
        break;

        case Qt::Key_Left:
            majVue('l');
        break;

        case Qt::Key_Down:
            majVue('b');
        break;

        case Qt::Key_Up:
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
        if (angleVue < PAS_VUE)
            angleVue = 360 - PAS_VUE + angleVue;
        else
            angleVue-=PAS_VUE;
        updateGL();
    break;

    case 'r':
        if (angleVue > 360 - PAS_VUE)
            angleVue = 360 + PAS_VUE - angleVue;
        else
            angleVue+=PAS_VUE;
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
        pos_x -= angle2x();
        pos_z -= angle2z();
        updateGL();
    break;

    case 'b':
        pos_x += angle2x();
        pos_z += angle2z();
        updateGL();
    break;

    case 'l':
        if (angleVue < PAS_VUE)
            angleVue = 360 - PAS_VUE + angleVue;
        else
            angleVue-=PAS_VUE;
        updateGL();
    break;

    case 'r':
        if (angleVue > 360 - PAS_VUE)
            angleVue = 360 + PAS_VUE - angleVue;
        else
            angleVue+=PAS_VUE;
        updateGL();
    break;

    default:
        qDebug() << "error";
    break;
    }

}

double MazeWidget::angle2x(){
    return cos(angleVue * PI / 180.0) * PAS_DEPLACEMENT;
}

double MazeWidget::angle2z(){
    return sin(angleVue * PI / 180.0) * PAS_DEPLACEMENT;
}

void MazeWidget::drawCube(){
    // Reinitialisation des tampons
    glClear(GL_COLOR_BUFFER_BIT); // Effacer le buffer de couleur

    glClear(GL_DEPTH_BUFFER_BIT); // clear le Z buffer

    // Definition de la matrice modelview
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0f, ((float)width)/height, 0.1f, 20.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(  0, 0, 0,    // position de la caméra
                10, 0, 0,  // position du point que fixe la caméra
                0, 1, 0);   // vecteur vertical

    // déplacement et rotation
    glRotated(angleVue, 0, 1, 0);
    glTranslated(pos_x, pos_y, pos_z);

    // Affichage
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
