#define _USE_MATH_DEFINES

#include "mazeWidget.h"
#include <QWidget>
#include <QApplication>
#include <QDesktopWidget>
#include <cmath>
#include <random>
#include <QDebug>
#include <QObject>

// PI
#define PI 3.14159265

// Declarations des constantes
const unsigned int WIN_WIDTH    = 1600;
const unsigned int WIN_HEIGHT   = 900;
const unsigned int PAS_VUE      = 10;
const double PAS_DEPLACEMENT    = 0.5;


// Constructeur
MazeWidget::MazeWidget(QWidget * parent) : QOpenGLWidget(parent)
{


    // ---Reglage de la taille/position de l'écran
    width = WIN_WIDTH;
    height = WIN_HEIGHT;
    resize(WIN_WIDTH, WIN_HEIGHT);

    move(QApplication::desktop()->screen()->rect().center() - rect().center());


    // ---on génère le labyrinthe
    laby.init(l, c, width_l, height_l);
    laby.generate();


    // ---Minimap
    miniMap = new MiniMap(&laby, c, this);
    miniMap->raise();
    miniMap->show();
    CurrentTimeRef = QDateTime::currentDateTime();

    effect = new QGraphicsOpacityEffect(miniMap);
    effect->setOpacity(0.5);
    miniMap->setGraphicsEffect(effect);

    // Connexion de detectition de fin de partie à
    //connect(laby,  &Maze::nouvellePartie, this,    &MazeWidget::newGame);
}

MazeWidget::~MazeWidget(){

}

// Fonction d'initialisation
void MazeWidget::initializeGL()
{

    // Reglage de la couleur de fond
    glClearColor(0.0, 0.0, 0.0, 0.0); // Couleur à utiliser lorsqu’on va nettoyer la fenêtre (= le fond)

    // Activation des buffers
    glEnable(GL_DEPTH_TEST);
    //    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    GLfloat light0_diffuse[] = { 0.5, 0.5, 0.5 ,1.0 };
    GLfloat light0_ambiant[] = { 0.5, 0.5, 0.5 ,1.0 };
    GLfloat light0_specular[] = { 0.5, 0.5, 0.5 ,1.0 };

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambiant);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

    glEnable(GL_LIGHT0); // Allume la 1ere lumière

    GLfloat light1_diffuse[] = { 0.1, 0.1, 0.1,1.0 };
    //GLfloat light1_ambiant[] = { 1.0, 0.2, 0.2,0.0 };
    GLfloat light1_specular[] = { 0.1, 0.1, 0.1,1.0 };

    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    //glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambiant);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);

    glEnable(GL_LIGHT1); // Allume la 2ème lumière



    //    GLfloat light2_position[] = { 1.0, 1.0, 0.0, 0.0 };
    GLfloat light2_diffuse[] = { 1.0, 1.0, 1.0 };
    GLfloat light2_ambiant[] = { 0.0, 0.0, 0.0 };
    GLfloat light2_specular[] = { 0.0, 0.0, 0.0 };


    //    glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
    glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambiant);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);

    glDisable(GL_LIGHT2); // Allume la 3ème lumière

}


void MazeWidget::newGame(){

    laby.init(l, c, width_l, height_l);
    laby.generate();

}

// Fonction de redimensionnement
void MazeWidget::resizeGL(int w, int h)
{

    // Definition du viewport (zone d'affichage)
    glViewport(0, 0, w, h);

    // on enregistre la taille
    width = w;
    height = h;

    // on resize la minimap en fonction
    miniMap->resize(w/30 * laby.getWidth(), h/30 * laby.getHeight());

}


// Fonction d'affichage
void MazeWidget::paintGL()
{

    // -- Reinitialisation des tampons
    glClear(GL_COLOR_BUFFER_BIT); // Effacer le buffer de couleur
    glClear(GL_DEPTH_BUFFER_BIT); // clear le Z buffer

    // -- Definition de la matrice modelview
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0f, ((float)width)/height, 0.1f, 50.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt( 0, y, 0,    // position de la caméra
               10, 0, 0,  // position du point que fixe la caméra
               0, 1, 0);   // vecteur vertical

    // -- Déplacement et rotation
    glRotated(angleVue, 0, 1, 0);
    glTranslated(-pos_x, pos_y, -pos_z); // ya des - car on déplace le labyrinthe par rapport à la caméra et non l'inverse


    // -- Lumière --

    GLfloat light0_position[] = { (float)laby.geti_sphere(), (float)laby.getj_sphere(), 0.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

    GLfloat light1_position[] = { 10.0, 10.0, 10.0, 0.0 };
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

    GLfloat light2_position[] = { -1.5, 1.0, -1.5, 1.0 };
    glLightfv(GL_LIGHT2, GL_POSITION, light2_position);




    // -- Affichage du labyrinthe 3D
    laby.display3D();



    // ------ Sol et plafond ------
    GLfloat light_plaf_ambient[] = { 0.2, 0.2, 0.2 };
    GLfloat light_plaf_diffuse[] = { 0.0, 0.0, 0.0 };
    GLfloat light_plaf_emission[] = { 0.0, 0.0, 0.0 };

    GLfloat light_sol_ambient[] = { 0.5, 0.5, 0.5 };
    GLfloat light_sol_diffuse[] = { 0.0, 0.0, 0.0 };
    GLfloat light_sol_emission[] = { 0.0, 0.0, 0.0 };

    /*
    glBegin(GL_QUADS);

    glNormal3f(0.0, 1.0, 0.0);


    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, light_sol_emission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, light_sol_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, light_sol_diffuse);

    glVertex3f(- c/2, -1.0f, laby.getWidth() * c - c/2);
    glVertex3f(laby.getHeight() * c - c/2, -1.0f, laby.getWidth() * c - c/2);
    glVertex3f(laby.getHeight() * c - c/2, -1.0f, - c/2);
    glVertex3f(- c/2, -1.0f, - c/2);


    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, light_plaf_emission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, light_plaf_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, light_plaf_diffuse);

    glVertex3f(- c/2, 1.0f, laby.getWidth() * c - c/2);
    glVertex3f(laby.getHeight() * c - c/2, 1.0f, laby.getWidth() * c - c/2);
    glVertex3f(laby.getHeight() * c - c/2, 1.0f, - c/2);
    glVertex3f(- c/2, 1.0f, - c/2);


    glEnd();
*/


    // -- Maj de la minimap
    miniMap->dessine(pos_x, pos_z, angleVue);

}


// Fonction de gestion d'interactions clavier
void MazeWidget::keyPressEventCall(QKeyEvent * event)
{

    switch(event->key())
    {
    // Rien
    case Qt::Key_Return:
        qDebug() << "Entrée";
        break;

        // Sortie de l'application
    case Qt::Key_Escape:
        onArrete();

    case Qt::Key_D:
        CurrentTimeRef = QDateTime::currentDateTime();
        majVue('r');
        break;

    case Qt::Key_Q:
        CurrentTimeRef = QDateTime::currentDateTime();
        majVue('l');
        break;

    case Qt::Key_A:
        CurrentTimeRef = QDateTime::currentDateTime();
        majVue('l');
        break;

    case Qt::Key_S:
        CurrentTimeRef = QDateTime::currentDateTime();
        majVue('b');
        break;

    case Qt::Key_Z:
        CurrentTimeRef = QDateTime::currentDateTime();
        majVue('f');
        break;

    case Qt::Key_W:
        CurrentTimeRef = QDateTime::currentDateTime();
        majVue('f');

        break;

    case Qt::Key_F:
        y = 10 - y;
        repaint();
        break;

        // Cas par defaut
    default:
        // Ignorer l'evenement
        qDebug()<<"event ignoré";
        event->ignore();
        break;

        return;
    }

    // Acceptation de l'evenement
    event->accept();

    //    qDebug() << angleVue;

    //    qDebug() << pos_x << pos_z;
}

void MazeWidget::updateView(char command){

    switch (command) {

    case 'f':
        CurrentTimeRef = QDateTime::currentDateTime();
        if (laby.gestionPos(pos_x + angle2x(), pos_z + angle2z())){
            pos_x += angle2x();
            pos_z += angle2z();
            repaint();
        }
        break;

    case 'b':
        CurrentTimeRef = QDateTime::currentDateTime();
        if (laby.gestionPos(pos_x - angle2x(), pos_z - angle2z())){
            pos_x -= angle2x();
            pos_z -= angle2z();
            repaint();
        }
        break;

    case 'l':
        CurrentTimeRef = QDateTime::currentDateTime();
        if (angleVue < PAS_VUE)
            angleVue = 360 - PAS_VUE + angleVue;
        else
            angleVue-=PAS_VUE;
        repaint();
        break;

    case 'r':
        CurrentTimeRef = QDateTime::currentDateTime();
        if (angleVue > 360 - PAS_VUE)
            angleVue = 360 + PAS_VUE - angleVue;
        else
            angleVue+=PAS_VUE;
        repaint();
        break;

    case '0':
        qDebug() << "Pas de Mouvement à effectuer";

        break;
    default:
        qDebug() << "error";

        break;
    }

}

void MazeWidget::majVue(char command){


    switch (command) {

    case 'f':
        CurrentTimeRef = QDateTime::currentDateTime();
        if (laby.gestionPos(pos_x + angle2x(), pos_z + angle2z())){
            pos_x += angle2x();
            pos_z += angle2z();
        }
        break;

    case 'b':
        CurrentTimeRef = QDateTime::currentDateTime();
        if (laby.gestionPos(pos_x - angle2x(), pos_z - angle2z())){
            pos_x -= angle2x();
            pos_z -= angle2z();
        }
        break;

    case 'l':
        CurrentTimeRef = QDateTime::currentDateTime();
        if (angleVue < PAS_VUE)
            angleVue = 360 - PAS_VUE + angleVue;
        else
            angleVue-=PAS_VUE;
        break;

    case 'r':
        CurrentTimeRef = QDateTime::currentDateTime();
        if (angleVue > 360 - PAS_VUE)
            angleVue = 360 + PAS_VUE - angleVue;
        else
            angleVue+=PAS_VUE;
        break;

    case '0':
        qDebug() << "Pas de Mouvement à effectuer";

        break;
    default:
        qDebug() << "error";
        break;
    }

    repaint();
}

double MazeWidget::angle2x(){
    return cos(angleVue * PI / 180.0) * PAS_DEPLACEMENT;
}

double MazeWidget::angle2z(){
    return sin(angleVue * PI / 180.0) * PAS_DEPLACEMENT;
}
