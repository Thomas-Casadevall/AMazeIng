/** An implementation of Prim's algorithm for generating mazes.
 * from <http://weblog.jamisbuck.org/2011/1/10/maze-generation-prim-s-algorithm>
 *
 * C++ implementation by C. Ducottet
**/

#ifndef CELL_H
#define CELL_H

#include <string>

#include <qopengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QPainter>
#include <QDebug>

using namespace std;

class Cell
{
public:
    // type de la cellule utilisé pour la génération
    enum Type {EMPTY,MARKED,FRONTIER};

    // Direction des murs
    enum Direction {N=1,S=3,E=0,W=2};

private:
    // Sauvegarde du type de la cellule
    int value_=EMPTY;

    //Murs de la cellule selon la disposition : {E, N, W, S}
    bool frontier[4]={true,true,true,true};

    // Image de texture pour la sphere
    QImage image;

    // 0 si ne contient pas la sphere et 1 sinon
    int sphere;

    // GLU pour la sphere de sa texture
    GLUquadric *quad;
    GLuint texture=0;

//    unsigned int r=0, g=0, b=255; // Bleu

public:
    Cell();
    ~Cell();
    // valeur pour debugger
    int ValTest=0;

    int getValue() const {return value_;}

    bool isFrontier(Direction d) const {return frontier[d];}

    void setValue(int type) {value_=type;}
    void setFrontier(Direction d,bool state) {frontier[d]=state;}

    /*
     * Role : enregistre la cellule comme ayant la sphere et créé un quadrique
     * Entréé : vide
     * Sortie : vide
     * */
    void setSphere();
//    void setSortie() {sortie = true;};

    /*
     * Role : met le int sphere à 0 pour ne plus afficher la sphere
     * Entrée : vide
     * Sortie : vide
     * */
    void unsetSphere();
//    void unsetSortie() {sortie = false;};

    /*
     * Role : affiche les quads représentant les murs de la cellule avec l'origine comme centre de la cellule
     * Entrée : - int l = demi largeur du mur
     *          - int c = demi coté de la cellule
     *          - bool ignore_S = si true autorise l'affichage du mur Nord
     *          - bool ignore_E = si true autorise l'affichage du mur West
     * Sortie : vide
     * */
    void display3D(float l, float c, bool ignore_S, bool ignore_E);

    /*
     * Role : dessine les murs dans le widget qui l'a appelé
     * Entrée : - QPainter * dessinateur = QPainter du widget qui a appelé cette méthode, indispensable pour dessiner
     *          - int off_x = offset en x représentant le point en haud à gauche de la cellule
     *          - int off_y = offset en y représentant le point en haud à gauche de la cellule
     * */
    void display2D(QPainter * dessinateur, int off_x, int off_y);
};

#endif // CELL_H
