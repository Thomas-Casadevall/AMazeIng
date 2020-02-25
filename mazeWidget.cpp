#define _USE_MATH_DEFINES

#include "mazeWidget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <cmath>
#include <random>


// Declarations des constantes
const unsigned int WIN_WIDTH  = 1600;
const unsigned int WIN_HEIGHT = 900;
const float MAX_DIMENSION     = 50.0f;


// Constructeur
MazeWidget::MazeWidget(QWidget * parent) : QGLWidget(parent)
{
    // Reglage de la taille/position
    setFixedSize(WIN_WIDTH, WIN_HEIGHT);
    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    // Connexion du timer
    connect(&m_AnimationTimer,  &QTimer::timeout, [&] {
        m_TimeElapsed += 1.0f / 12.0f;
        updateGL();
    });

    m_AnimationTimer.setInterval(10);
    m_AnimationTimer.start();
}


// Fonction d'initialisation
void MazeWidget::initializeGL()
{
    // Reglage de la couleur de fond
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

    // Activation du zbuffer

    // Distance par rapport au soleil, rayon, periode de rotation, periode de révolution
    // 0.0f, 5.0f, 0.0f, 0.0f               // Soleil
    // 7.5f, 0.50f,  58.646f,   87.969f     // Mercure
    // 10.0f, 0.90f, -243.018f, 224.701f    // Venus
    // 13.0f, 0.90f, 0.997f,    365.256f    // Terre
    // 17.5f, 1.50f, 1.025f,    686.960f    // Mars
    // 27.0f, 3.00f, 0.413f,    935.354f    // Jupiter
    // 35.0f, 2.50f, 0.448f,    1757.736f   // Saturne
    // 40.5f, 1.50f, -0.718f,   3687.150f   // Uranus
    // 45.0f, 1.50f, 0.671f,    6224.903f;  // Neptune

    // Ajout des planetes dans le vecteur m_Planetes
}


// Fonction de redimensionnement
void MazeWidget::resizeGL(int width, int height)
{
    // Definition du viewport (zone d'affichage)
    glViewport(0, 0, width, height);

    // Definition de la matrice de projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if(width != 0)
        glOrtho(-MAX_DIMENSION, MAX_DIMENSION, -MAX_DIMENSION * height / static_cast<float>(width), MAX_DIMENSION * height / static_cast<float>(width), -MAX_DIMENSION * 2.0f, MAX_DIMENSION * 2.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


// Fonction d'affichage
void MazeWidget::paintGL()
{
    // Reinitialisation des tampons

    // Definition de la position de la camera

    // Affichage des m_Planetes
}


// Fonction de gestion d'interactions clavier
void MazeWidget::keyPressEvent(QKeyEvent * event)
{
    switch(event->key())
    {
        // Activation/Arret de l'animation
        case Qt::Key_Enter:
        {
            if(m_AnimationTimer.isActive())
                m_AnimationTimer.stop();
            else
                m_AnimationTimer.start();

            break;
        }

        // Sortie de l'application
        case Qt::Key_Escape:
        {
            exit(0);
        }

        // Cas par defaut
        default:
        {
            // Ignorer l'evenement
            event->ignore();
            return;
        }
    }

    // Acceptation de l'evenement et mise a jour de la scene
    event->accept();
    updateGL();
}
