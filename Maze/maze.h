#ifndef MAZE_H
/** An implementation of Prim's algorithm for generating mazes.
 * from <http://weblog.jamisbuck.org/2011/1/10/maze-generation-prim-s-algorithm>
 *
 * C++ implementation by C. Ducottet
**/

#define MAZE_H

#include "cell.h"
#include <vector>
#include <list>
#include <utility>


using namespace std;

using Point=pair<int,int>;

class Maze
{
    vector<vector<Cell>> grid_;

    int width_;
    int height_;

    int i_sphere, j_sphere;
    int i_sortie = -1, j_sortie = -1;

    void addFrontier(Point p,list<Point> & frontier);
    void mark(Point p,list<Point> & frontier);
    list<Point> neighbors(Point p);
    Cell::Direction direction(Point f, Point t);

public:
    Maze(int width = 10,int height = 6);
    int getWidth(){return width_;};
    int getHeight(){return height_;};
    void reinit(int width = 10,int height = 6);
    void display(bool pause=false);
    void display3D(float l, float c);
    void display2D(QPainter * dessinateur);
    void generate(bool show=false);
    void spheretrouve();
};

#endif // MAZE_H
