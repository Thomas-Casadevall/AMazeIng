/** An implementation of Prim's algorithm for generating mazes.
 * from <http://weblog.jamisbuck.org/2011/1/10/maze-generation-prim-s-algorithm>
 *
 * C++ implementation by C. Ducottet
**/

#include<iostream>
#include <vector>
#include <ctime>
#include "cell.h"
#include "maze.h"
#include <math.h>

using namespace std;

Maze::Maze(int width, int height)
    : grid_(height,vector<Cell>(width)), width_(width), height_(height)
{

}

void Maze::init(float l_, float c_, int width, int height){
    l = l_;
    c = c_;
    width_ = width;
    height_ = height;

}

void Maze::reinit(int width, int height)
{
    width_ = width;
    height_ = height;
    grid_=vector<vector<Cell>>(height_,vector<Cell>(width_));
}

void Maze::addFrontier(Point p, list<Point> &frontier)
{
    if (p.first>=0 && p.second>=0 && p.second<height_ && p.first<width_
            && grid_[p.second][p.first].getValue()==0) {
        grid_[p.second][p.first].setValue(Cell::FRONTIER);
        frontier.push_back(p);
    }
}


void Maze::mark(Point p, list<Point> &frontier)
{
    grid_[p.second][p.first].setValue(Cell::MARKED);
    addFrontier(Point(p.first-1, p.second),frontier);
    addFrontier(Point(p.first+1, p.second),frontier);
    addFrontier(Point(p.first, p.second-1),frontier);
    addFrontier(Point(p.first, p.second+1),frontier);
}


list<Point> Maze::neighbors(Point p)
{
    list<Point> n;
    if (p.first>0 && grid_[p.second][p.first-1].getValue()==Cell::MARKED)
        n.push_back(Point(p.first-1, p.second));
    if (p.first+1<width_ && grid_[p.second][p.first+1].getValue()==Cell::MARKED)
        n.push_back(Point(p.first+1, p.second));
    if (p.second>0 && grid_[p.second-1][p.first].getValue()==Cell::MARKED)
        n.push_back(Point(p.first, p.second-1));
    if (p.second+1<height_ && grid_[p.second+1][p.first].getValue()==Cell::MARKED)
        n.push_back(Point(p.first, p.second+1));
    return n;
}


Cell::Direction Maze::direction(Point f, Point t)
{
    if (f.first<t.first) return Cell::E;
    else if (f.first>t.first) return Cell::W;
    else if (f.second<t.second) return Cell::S;
    else return Cell::N;
}


void Maze::display(bool pause)
{
    int i,j;
    string cell[3]={"..","  ","()"};

    if (pause) system("cls"); // use "clear" under linux

    // Print the first line
    for (j=0;j<width_;j++) cout<<"+--";
    cout<<'+'<<endl;

    // Print other lines
    for (i=0;i<height_;i++) {
        // Beginning of line
        cout<<'|';
        // Print cells
        for (j=0;j<width_;j++) {
            cout<<cell[grid_[i][j].getValue()];
            if (grid_[i][j].isFrontier(Cell::E)) cout<<'|';
            else cout<<' ';
        }
        cout<<endl;
        // Beginning of line
        cout<<'+';
        // Print horizontal frontier
        for (j=0;j<width_;j++) {
            if (grid_[i][j].isFrontier(Cell::S)) cout<<"--";
            else cout<<"  ";
            cout<<'+';
        }
        cout<<endl;
    }

    if (pause) {
        cout<<"Press ENTER to continue....."<<endl;
        cin.ignore(1);
    }
}

void Maze::generate(bool show)
{
    list<Point> frontier;

    // Initialize cells if the maze was already generated
    reinit();

    // Initialize random generator
    srand (time(NULL));

    // Mark a random cell and add the frontier cells to the list
    mark(Point(rand() % width_, rand() % height_),frontier);

    // Display
    if (show) display(true);

    while(!frontier.empty()) {

        // Take a random frontier cell f (from)
        auto randPos=frontier.begin();
        advance(randPos,rand() % frontier.size());
        Point f=*randPos;
        frontier.erase(randPos);

        // Take a random neighbor t (to) of that cell
        list<Point> n=neighbors(f);
        randPos=n.begin();
        advance(randPos,rand() % n.size());
        Point t=*randPos;

        // Carve a passage from f to t
        Cell::Direction d=direction(f,t);
        grid_[f.second][f.first].setFrontier(d,false);
        grid_[t.second][t.first].setFrontier(Cell::Direction((d+2)%4),false);

        // Mark the cell and add the frontier cells to the list
        mark(f,frontier);

        // Display
        if (show) display(true);
    }

    i_sphere = rand() % (height_ - 1) + 1; // (height_ - 1) + 1 pour éviter qu'elle soit en 0,0
    j_sphere = rand() % (width_ - 1) + 1;

    grid_[i_sphere][j_sphere].setSphere();
}

void Maze::display3D(){

    GLfloat light_ambient[] = { 0.2, 0.2, 0.2 };
    GLfloat light_diffuse[] = { 0.6, 0.6, 0.6 };
    GLfloat light_emission[] = { 0.0, 0.0, 0.0 };

    for (int i=0;i<height_;i++) {

        for (int j=0;j<width_;j++) {

            glPushMatrix();

            glTranslated(i*c, 0, j*c);

            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, light_emission);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, light_ambient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, light_diffuse);

            grid_[i][j].display3D(l/2, c/2, i==0, j==0);

            glPopMatrix();
        }

    }

}

void Maze::display2D(QPainter * dessinateur){

    for (int i=0;i<height_;i++) {

        for (int j=0;j<width_;j++) {

            glPushMatrix();

            glTranslated(i, 0, j);

            grid_[i][j].display2D(dessinateur, j * 10, i * 10);

            glPopMatrix();
        }

    }
}



void Maze::spheretrouve(){

    grid_[i_sphere][j_sphere].unsetSphere();

    // nord ou sud
    if (rand() % 2){
        i_sortie = (height_ - 1) * (rand() % 2);
        j_sortie = rand() % width_;

        if (i_sortie) // sud
            dir_sort = Cell::S;

        else // nord
            dir_sort = Cell::N;

    }
    // est ou ouest
    else {
        j_sortie = (width_ - 1) * (rand() % 2);
        i_sortie = rand() % height_;

        if (j_sortie) // ouest
            dir_sort = Cell::W;

        else // est
            dir_sort = Cell::E;
    }

//    qDebug() << i_sortie << j_sortie;

    grid_[i_sortie][j_sortie].setFrontier(dir_sort, 0);
}


bool Maze::gestionPos(double pos_x, double pos_y){

    // Sphere
    if (pos_x > (i_sphere * c - 0.4*c) && pos_x < (i_sphere * c + 0.4*c) &&
        pos_y > (j_sphere * c - 0.4*c) && pos_y < (j_sphere * c + 0.4*c)){

        spheretrouve();

        i_sphere = -1;
        j_sphere = -1;
    }

    // victoire
    if (i_sortie >= 0){
        switch (dir_sort) {
            case Cell::N:
            if (pos_x > (i_sortie * c - c/2) && pos_x < (i_sortie * c) &&
                pos_y > (j_sortie * c - c/2 + l) && pos_y < (j_sortie * c + c/2 - l)){
                // call victoire
                return false;
            }
            break;

            case Cell::S:
            if (pos_x > (i_sortie * c) && pos_x < (i_sortie * c + c/2) &&
                pos_y > (j_sortie * c - c/2 + l) && pos_y < (j_sortie * c + c/2 - l)){
                // call victoire
                return false;
            }
            break;

            case Cell::E:
            if (pos_x > (i_sortie * c - c/2 + l) && pos_x < (i_sortie * c + c/2 - l) &&
                pos_y > (j_sortie * c) && pos_y < (j_sortie * c + c/2)){
                // call victoire
                return false;
            }
            break;

            case Cell::W:
            if (pos_x > (i_sortie * c - c/2 + l) && pos_x < (i_sortie * c + c/2 - l) &&
                pos_y > (j_sortie * c - c/2) && pos_y < (j_sortie * c)){
                // call victoire
                return false;
            }
            break;

        }

    }


    // si on sort de la map
    if (pos_x <= (-c/2 + l) || pos_x >= (c*height_ - c/2 - l) ||
        pos_y <= (-c/2 + l) || pos_y >= (c*width_ - c/2 - l))
        return false;

     //colisions
    for (int i=0;i<height_-1;i++) {
        if (pos_x >= (i * c + c/2 - l) && pos_x <= (i * c + c/2 + l))
            if (grid_[i][(round(pos_y/c))].isFrontier(Cell::S))
                return false;
    }

    for (int j=0;j<width_-1;j++) {
        if (pos_y >= (j * c + c/2 - l) && pos_y <= (j * c + c/2 + l))
            if (grid_[(round(pos_x/c))][j].isFrontier(Cell::E))
                return false;
    }

    return true;

}
