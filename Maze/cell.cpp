#include "Maze/cell.h"

void Cell::display3D(){

    // Affichage
    glBegin(GL_QUADS);

    glColor3ub(0, 0, 255); // Bleu

    if (isFrontier(N)){

        glVertex3f(-0.4f, 1.0f, -0.5f);
        glVertex3f(-0.4f, -1.0f, -0.5f);
        glVertex3f(-0.4f, -1.0f, 0.5f);
        glVertex3f(-0.4f, 1.0f, 0.5f);
    }

    if (isFrontier(S)){

        glVertex3f(0.4f, 1.0f, -0.5f);
        glVertex3f(0.4f, -1.0f, -0.5f);
        glVertex3f(0.4f, -1.0f, 0.5f);
        glVertex3f(0.4f, 1.0f, 0.5f);
    }

    if (isFrontier(E)){

        glVertex3f(-0.5f, 1.0f, 0.4f);
        glVertex3f(-0.5f, -1.0f, 0.4f);
        glVertex3f(0.5f, -1.0f, 0.4f);
        glVertex3f(0.5f, 1.0f, 0.4f);
    }

    if (isFrontier(W)){

        glVertex3f(-0.5f, 1.0f, -0.4f);
        glVertex3f(-0.5f, -1.0f, -0.4f);
        glVertex3f(0.5f, -1.0f, -0.4f);
        glVertex3f(0.5f, 1.0f, -0.4f);
    }

    glEnd();


//    // texture
//    if (texturation) {
//        glBindTexture(GL_TEXTURE_2D, texture); // On définit la texture courante
//        gluQuadricTexture(quad, GL_TRUE); // On active les textures
//    }

//    // Couleur de l'objet
//    glColor3ub(r, g, b);
//    GLfloat colorAmbiant_And_Diffuse[] = { r/255.0f, g/255.0f, b/255.0f, 1.0 };
//    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colorAmbiant_And_Diffuse);
//    gluQuadricDrawStyle(quad, GLU_FILL);

//    if (source){
//        GLfloat colorEmission[] = { r/255.0f, g/255.0f, b/255.0f, 0.1 };
//        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION , colorEmission);
//    }
//    else{
//        GLfloat colorEmission[] = { 0.0, 0.0, 0.0, 0.0 };
//        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION , colorEmission);
//    }


}


