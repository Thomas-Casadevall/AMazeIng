#include "Maze/cell.h"

#include <QGLWidget>
#include <QFile>


Cell::Cell(){

    sphere = 0;
//    sortie = false;
}

Cell::~Cell(){

    if(sphere)
        gluDeleteQuadric(quad);
}

void Cell::display3D(float l, float c, bool ignore_S, bool ignore_E){


    glBegin(GL_QUADS);

//    glColor3ub(r, g, b);

    if (isFrontier(S) && !ignore_S){

//        glColor3ub(255,255,255);    //vert

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

    if (isFrontier(E) && !ignore_E){

//        glColor3ub(255,255,255);    // blanc

        glNormal3f(0.0, 0.0, -1.0);
        glVertex3f(-(c + l - 0.0001), 1.0f, c - l);
        glVertex3f(-(c + l - 0.0001), -1.0f, c - l);
        glVertex3f(c + l - 0.0001, -1.0f, c - l);
        glVertex3f(c + l - 0.0001, 1.0f, c - l);

        glNormal3f(0.0, 0.0, 1.0);
        glVertex3f(-(c + l - 0.0001), 1.0f, c + l);
        glVertex3f(-(c + l - 0.0001), -1.0f, c + l);
        glVertex3f(c + l - 0.0001, -1.0f, c + l);
        glVertex3f(c + l - 0.0001, 1.0f, c + l);

        glNormal3f(-1.0, 0.0, 0.0);
        glVertex3f(-(c + l - 0.0001), 1.0f, c + l);
        glVertex3f(-(c + l - 0.0001), -1.0f, c + l);
        glVertex3f(-(c + l - 0.0001), -1.0f, c - l);
        glVertex3f(-(c + l - 0.0001), 1.0f, c - l);

        glNormal3f(1.0, 0.0, 0.0);
        glVertex3f(c + l - 0.0001, 1.0f, c + l);
        glVertex3f(c + l - 0.0001, -1.0f, c + l);
        glVertex3f(c + l - 0.0001, -1.0f, c - l);
        glVertex3f(c + l - 0.0001, 1.0f, c - l);
    }

    glEnd();


    if (sphere){

        glBindTexture(GL_TEXTURE_2D, texture); // On définit la texture courante
        gluQuadricTexture(quad, GL_TRUE); // On active les textures

        GLfloat color[] = { 0.2, 0.0, 0.0 };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
        GLfloat diffuse[] = { 0.0, 0.0, 0.0 };
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color);

        gluQuadricDrawStyle(quad, GLU_FILL);

        gluSphere(quad, 0.7, 20, 20);

//        GLfloat light2_position[] = { 0.0, 0.0, 0.0, 1.0 };
//        glLightfv(GL_LIGHT2, GL_POSITION, light2_position);

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

    // chargement de la texture
//    QString filename(":/terre.jpg");
    QString filename(":/telecom_logo_quadri_longueur.png");
    if (QFile::exists(filename))
    {
        QImage image = QGLWidget::convertToGLFormat(QImage(filename)); // Charge une image et la convertit en un format attendu par les fonctions d’OpenGL
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

    quad = gluNewQuadric();


//    glEnable(GL_LIGHT2); // Allume la 2ème lumière
//    glDisable(GL_LIGHT2);
}

void Cell::unsetSphere(){
    sphere = 0;
    gluDeleteQuadric(quad);
//    glDisable(GL_LIGHT2);
}

/*
void Cell::display3D(){


//    // texture
//    if (texturation) {
//        glBindTexture(GL_TEXTURE_2D, texture); // On définit la texture courante
//        gluQuadricTexture(quad, GL_TRUE); // On active les textures
//    }


//    GLfloat colorAmbiant_And_Diffuse[] = { r/255.0f, g/255.0f, b/255.0f, 1.0 };
//    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colorAmbiant_And_Diffuse) ;

//    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
//    GLfloat light_diffuse[] = { 0.0, 0.0, 0.0, 1.0 };
//    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, light_specular);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, light_diffuse);
    //glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colorAmbiant_And_Diffuse);

    glColor3ub(r, g, b);
    // Affichage
    glBegin(GL_QUADS);

    glColor3ub(r, g, b);

    if (isFrontier(N)){

        glColor3ub(255,0,0);    //rouge

        // mur
        glNormal3f(1.0, 0.0, 0.0);
        glVertex3f(-0.4f, 1.0f, -0.5f);
        glVertex3f(-0.4f, -1.0f, -0.5f);
        glVertex3f(-0.4f, -1.0f, 0.5f);
        glVertex3f(-0.4f, 1.0f, 0.5f);

        // fermeture du mur
        glNormal3f(0.0, 0.0, 1.0);
        glVertex3f(-0.4f, 1.0f, -0.5f);
        glVertex3f(-0.4f, -1.0f, -0.5f);
        glVertex3f(-0.5f, -1.0f, -0.5f);
        glVertex3f(-0.5f, 1.0f, -0.5f);

        glNormal3f(0.0, 0.0, 1.0);
        glVertex3f(-0.4f, 1.0f, 0.5f);
        glVertex3f(-0.4f, -1.0f, 0.5f);
        glVertex3f(-0.5f, -1.0f, 0.5f);
        glVertex3f(-0.5f, 1.0f, 0.5f);
    }

    if (isFrontier(S)){

        glColor3ub(0,255,0);    //vert

        glNormal3f(1.0, 0.0, 0.0);
        glVertex3f(0.4f, 1.0f, -0.5f);
        glVertex3f(0.4f, -1.0f, -0.5f);
        glVertex3f(0.4f, -1.0f, 0.5f);
        glVertex3f(0.4f, 1.0f, 0.5f);

        glNormal3f(0.0, 0.0, 1.0);
        glVertex3f(0.4f, 1.0f, -0.5f);
        glVertex3f(0.4f, -1.0f, -0.5f);
        glVertex3f(0.5f, -1.0f, -0.5f);
        glVertex3f(0.5f, 1.0f, -0.5f);

        glNormal3f(0.0, 0.0, 1.0);
        glVertex3f(0.4f, 1.0f, 0.5f);
        glVertex3f(0.4f, -1.0f, 0.5f);
        glVertex3f(0.5f, -1.0f, 0.5f);
        glVertex3f(0.5f, 1.0f, 0.5f);
    }

    if (isFrontier(E)){

        glColor3ub(0,0,255);    //bleu

        glNormal3f(0.0, 0.0, 1.0);
        glVertex3f(-0.5f, 1.0f, -0.4f);
        glVertex3f(-0.5f, -1.0f, -0.4f);
        glVertex3f(0.5f, -1.0f, -0.4f);
        glVertex3f(0.5f, 1.0f, -0.4f);

        glNormal3f(1.0, 0.0, 0.0);
        glVertex3f(0.5f, 1.0f, -0.4f);
        glVertex3f(0.5f, -1.0f, -0.4f);
        glVertex3f(0.5f, -1.0f, -0.5f);
        glVertex3f(0.5f, 1.0f, -0.5f);

        glNormal3f(1.0, 0.0, 0.0);
        glVertex3f(-0.5f, 1.0f, -0.4f);
        glVertex3f(-0.5f, -1.0f, -0.4f);
        glVertex3f(-0.5f, -1.0f, -0.5f);
        glVertex3f(-0.5f, 1.0f, -0.5f);
    }

    if (isFrontier(W)){

        glColor3ub(255,255,255);    // blanc

        glNormal3f(0.0, 0.0, 1.0);
        glVertex3f(-0.5f, 1.0f, 0.4f);
        glVertex3f(-0.5f, -1.0f, 0.4f);
        glVertex3f(0.5f, -1.0f, 0.4f);
        glVertex3f(0.5f, 1.0f, 0.4f);

        glNormal3f(1.0, 0.0, 0.0);
        glVertex3f(0.5f, 1.0f, 0.4f);
        glVertex3f(0.5f, -1.0f, 0.4f);
        glVertex3f(0.5f, -1.0f, 0.5f);
        glVertex3f(0.5f, 1.0f, 0.5f);

        glNormal3f(1.0, 0.0, 0.0);
        glVertex3f(-0.5f, 1.0f, 0.4f);
        glVertex3f(-0.5f, -1.0f, 0.4f);
        glVertex3f(-0.5f, -1.0f, 0.5f);
        glVertex3f(-0.5f, 1.0f, 0.5f);
    }

    glEnd();
}*/
