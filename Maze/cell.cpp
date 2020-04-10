#include "Maze/cell.h"

#include <QGLWidget>
#include <QFile>


Cell::Cell(){

    sphere = 0;
//    sortie = false;
}

Cell::~Cell(){
qDebug()<<"destruction de Cell";
    if(sphere)
        gluDeleteQuadric(quad);
}

void Cell::display3D(float l, float c, bool ignore_S, bool ignore_E){


    glBegin(GL_QUADS);

    if (isFrontier(S)/* && !ignore_S*/){

        glNormal3f(-1.0, 0.0, 0.0);
        glVertex3f(c - l, 1.0f, -(c + l - 0.0001));
        glVertex3f(c - l, -1.0f, -(c + l - 0.0001));
        glVertex3f(c - l, -1.0f, c + l - 0.0001);
        glVertex3f(c - l, 1.0f, c + l - 0.0001);

        glNormal3f(1.0, 0.0, 0.0);
        glVertex3f(c + l, 1.0f, -(c + l - 0.0001));
        glVertex3f(c + l, -1.0f, -(c + l - 0.0001));
        glVertex3f(c + l, -1.0f, c + l - 0.0001);
        glVertex3f(c + l, 1.0f, c + l - 0.0001);

        glNormal3f(0.0, 0.0, -1.0);
        glVertex3f(c - l, 1.0f, -(c + l - 0.0001));
        glVertex3f(c - l, -1.0f, -(c + l - 0.0001));
        glVertex3f(c + l, -1.0f, -(c + l - 0.0001));
        glVertex3f(c + l, 1.0f, -(c + l - 0.0001));

        glNormal3f(0.0, 0.0, 1.0);
        glVertex3f(c - l, 1.0f, c + l - 0.0001);
        glVertex3f(c - l, -1.0f, c + l - 0.0001);
        glVertex3f(c + l, -1.0f, c + l - 0.0001);
        glVertex3f(c + l, 1.0f, c + l - 0.0001);
    }

    if (isFrontier(E)/* && !ignore_E*/){

        glNormal3f(0.0, 0.0, 1.0);
        glVertex3f(-(c + l - 0.0001), 1.0f, c - l);
        glVertex3f(-(c + l - 0.0001), -1.0f, c - l);
        glVertex3f(c + l - 0.0001, -1.0f, c - l);
        glVertex3f(c + l - 0.0001, 1.0f, c - l);

        glNormal3f(0.0, 0.0, -1.0);
        glVertex3f(-(c + l - 0.0001), 1.0f, c + l);
        glVertex3f(-(c + l - 0.0001), -1.0f, c + l);
        glVertex3f(c + l - 0.0001, -1.0f, c + l);
        glVertex3f(c + l - 0.0001, 1.0f, c + l);

        glNormal3f(1.0, 0.0, 0.0);
        glVertex3f(-(c + l - 0.0001), 1.0f, c + l);
        glVertex3f(-(c + l - 0.0001), -1.0f, c + l);
        glVertex3f(-(c + l - 0.0001), -1.0f, c - l);
        glVertex3f(-(c + l - 0.0001), 1.0f, c - l);

        glNormal3f(-1.0, 0.0, 0.0);
        glVertex3f(c + l - 0.0001, 1.0f, c + l);
        glVertex3f(c + l - 0.0001, -1.0f, c + l);
        glVertex3f(c + l - 0.0001, -1.0f, c - l);
        glVertex3f(c + l - 0.0001, 1.0f, c - l);
    }

    if (isFrontier(N) && ignore_S){

        glNormal3f(1.0, 0.0, 0.0);
        glVertex3f(-(c - l), 1.0f, -(c + l - 0.0001));
        glVertex3f(-(c - l), -1.0f, -(c + l - 0.0001));
        glVertex3f(-(c - l), -1.0f, c + l - 0.0001);
        glVertex3f(-(c - l), 1.0f, c + l - 0.0001);

        glNormal3f(-1.0, 0.0, 0.0);
        glVertex3f(-(c + l), 1.0f, -(c + l - 0.0001));
        glVertex3f(-(c + l), -1.0f, -(c + l - 0.0001));
        glVertex3f(-(c + l), -1.0f, c + l - 0.0001);
        glVertex3f(-(c + l), 1.0f, c + l - 0.0001);

        glNormal3f(0.0, 0.0, 1.0);
        glVertex3f(-(c - l), 1.0f, -(c + l - 0.0001));
        glVertex3f(-(c - l), -1.0f, -(c + l - 0.0001));
        glVertex3f(-(c + l), -1.0f, -(c + l - 0.0001));
        glVertex3f(-(c + l), 1.0f, -(c + l - 0.0001));

        glNormal3f(0.0, 0.0, -1.0);
        glVertex3f(-(c - l), 1.0f, c + l - 0.0001);
        glVertex3f(-(c - l), -1.0f, c + l - 0.0001);
        glVertex3f(-(c + l), -1.0f, c + l - 0.0001);
        glVertex3f(-(c + l), 1.0f, c + l - 0.0001);
    }

    if (isFrontier(W) && ignore_E){

        glNormal3f(0.0, 0.0, -1.0);
        glVertex3f(-(c + l - 0.0001), 1.0f, -(c - l));
        glVertex3f(-(c + l - 0.0001), -1.0f, -(c - l));
        glVertex3f(c + l - 0.0001, -1.0f, -(c - l));
        glVertex3f(c + l - 0.0001, 1.0f, -(c - l));

        glNormal3f(0.0, 0.0, 1.0);
        glVertex3f(-(c + l - 0.0001), 1.0f, -(c + l));
        glVertex3f(-(c + l - 0.0001), -1.0f, -(c + l));
        glVertex3f(c + l - 0.0001, -1.0f, -(c + l));
        glVertex3f(c + l - 0.0001, 1.0f, -(c + l));

        glNormal3f(-1.0, 0.0, 0.0);
        glVertex3f(-(c + l - 0.0001), 1.0f, -(c + l));
        glVertex3f(-(c + l - 0.0001), -1.0f, -(c + l));
        glVertex3f(-(c + l - 0.0001), -1.0f, -(c - l));
        glVertex3f(-(c + l - 0.0001), 1.0f, -(c - l));

        glNormal3f(1.0, 0.0, 0.0);
        glVertex3f(c + l - 0.0001, 1.0f, -(c + l));
        glVertex3f(c + l - 0.0001, -1.0f, -(c + l));
        glVertex3f(c + l - 0.0001, -1.0f, -(c - l));
        glVertex3f(c + l - 0.0001, 1.0f, -(c - l));
    }

    glEnd();


    if (sphere){



// Gestion de la Lumière
//        GLfloat colorAmbient[] = { 0.3, 0.3, 0.3,1.0 };
//        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE, colorAmbient);
//        GLfloat couleurLmp_tab[] = {1.0,1.0,1.0, 1.0};
//        glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION, couleurLmp_tab);
//        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, couleurLmp_tab);


        GLfloat ambient[] = { 0.2, 0.0, 0.0 };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);

        GLfloat emission[] = { 0.2, 0.2, 0.2 };
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);

        GLfloat diffuse[] = { 0.0, 0.0, 0.0 };
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

// ///////////////

        QString filename(":/telecom_logo_quadri_longueur.png");
//        QString filename(":/terre.jpg");
        QImage image = QGLWidget::convertToGLFormat(QImage(filename)); // Charge une image et la convertit en un format attendu par les fonctions d’OpenGL
        glBindTexture( GL_TEXTURE_2D, texture); // Définit la texture courante
        glTexImage2D( // Transmet à OpenGL toutes les caractéristiques de la texture courante : largeur, hauteur, format, etc... et bien sûr l'image
                      GL_TEXTURE_2D,
                      0, // niveau de détail de l’image, avec 0 on a l’image de base
                      4, // nombre de composantes de couleurs par pixel (3 si RGB, 4 si RGBA, …)
                      image.width(),  // largeur de la texture
                      image.height(),  // et hauteur de la texture
                      0, // toujours 0 d’après les spécif
                      GL_RGBA, // format de stockage (GL_RGB, GL_RGBA, …)
                      GL_UNSIGNED_BYTE, // type dans lequel sont stockées les composantes(GL_UNSIGNED_BYTE, GL_BYTE, GL_INT, …)
                      image.bits() ); // adresse de la texture

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);



        gluQuadricTexture(quad,GL_TRUE);

        gluQuadricDrawStyle(quad, GLU_FILL);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture); // On définit la texture courante
        gluSphere(quad, 0.7, 20, 20);
        qDebug()<<ValTest;
        glDisable(GL_TEXTURE_2D);

    }
}

void Cell::display2D(QPainter * dessinateur, int off_x, int off_y){

    if (isFrontier(N)){
        dessinateur->drawLine(off_x, off_y, off_x + 10, off_y);
    }

    if (isFrontier(S)){
        dessinateur->drawLine(off_x, off_y + 10, off_x + 10, off_y + 10);
    }

    if (isFrontier(E)){
        dessinateur->drawLine(off_x + 10, off_y, off_x + 10, off_y + 10);
    }

    if (isFrontier(W)){
        dessinateur->drawLine(off_x, off_y, off_x, off_y + 10);
    }

}

void Cell::setSphere(){

    sphere = 1;
/*
    // chargement de la texture
    QString filename(":/terre.jpg");
//    QString filename(":/telecom_logo_quadri_longueur.png");
    if (QFile::exists(filename))
    {
        //QImage image = QGLWidget::convertToGLFormat(QImage(filename)); // Charge une image et la convertit en un format attendu par les fonctions d’OpenGL
        glGenTextures(1, &texture); // Crée un objet texture et lui associe un numéro qu’on stocke
        glBindTexture( GL_TEXTURE_2D, texture); // Définit la texture courante
        glTexImage2D( // Transmet à OpenGL toutes les caractéristiques de la texture courante : largeur, hauteur, format, etc... et bien sûr l'image
                      GL_TEXTURE_2D,
                      0, // niveau de détail de l’image, avec 0 on a l’image de base
                      4, // nombre de composantes de couleurs par pixel (3 si RGB, 4 si RGBA, …)
                      image.width(),  // largeur de la texture
                      image.height(),  // et hauteur de la texture
                      0, // toujours 0 d’après les spécif
                      GL_RGBA, // format de stockage (GL_RGB, GL_RGBA, …)
                      GL_UNSIGNED_BYTE, // type dans lequel sont stockées les composantes(GL_UNSIGNED_BYTE, GL_BYTE, GL_INT, …)
                      image.bits() ); // adresse de la texture

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }
    */
    quad = gluNewQuadric();
    //gluQuadricTexture(quad,GL_TRUE);


}

void Cell::unsetSphere(){
    qDebug()<<"destruction de sphere";
    sphere = 0;
//    gluDeleteQuadric(quad);

}
