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
    enum Type {EMPTY,MARKED,FRONTIER};
    enum Direction {N=1,S=3,E=0,W=2};

private:
    int value_=EMPTY;
    bool frontier[4]={true,true,true,true};

    int sphere;
//    bool sortie;

    // sphere
    //GLUquadric* quadriqueSphere1;
    GLUquadric *quad;
    GLuint texture=0;

//    unsigned int r=0, g=0, b=255; // Bleu

public:
    Cell();
    ~Cell();

    int getValue() const {return value_;}
    bool isFrontier(Direction d) const {return frontier[d];}
    void setValue(int type) {value_=type;}
    void setFrontier(Direction d,bool state) {frontier[d]=state;}

    void setSphere();
//    void setSortie() {sortie = true;};

    void unsetSphere();
//    void unsetSortie() {sortie = false;};

    // l = demi largeur du mur, c = demi coté de la cellule
    void display3D(float l, float c, bool ignore_S, bool ignore_E);
    void display2D(QPainter * dessinateur, int off_x, int off_y);
};

#endif // CELL_H
