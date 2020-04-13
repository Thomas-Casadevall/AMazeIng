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
    // Grille du labyrinthe
    vector<vector<Cell>> grid_;

    // Taille du laby
    int width_;
    int height_;

    // Coordonées de la sphere
    int i_sphere, j_sphere;

    // Coordonées de la sortie
    int i_sortie = -1, j_sortie = -1;

    // direcion de la porte de sortie
    Cell::Direction dir_sort;

    void addFrontier(Point p,list<Point> & frontier);
    void mark(Point p,list<Point> & frontier);
    list<Point> neighbors(Point p);
    Cell::Direction direction(Point f, Point t);

    /*
     * Role : enlève la sphere de la cell et génère la porte de sortie
     * Entrée : vide
     * Sortie : vide
     * */
    void spheretrouve();

    // l = largeur du mur, c = coté de la cellule
    float l, c;

    bool victoire = false;

public:
    Maze(int width = 10,int height = 6);
    ~Maze();
    int getWidth(){return width_;};
    int getHeight(){return height_;};
    int geti_sphere()const{return i_sphere;};
    int getj_sphere()const{return j_sphere;};

    /*
     * Role : change la valeur des paremètres avec ceux donné en paramètre
     * Entree : - float l_ = remplace l
     *          - float c_ = remplace c
     *          - float width = remplace width_
     *          - float height = remplace height_
     * Sortie : vide
     * */
    void init(float l_, float c_, int width, int height);

    /*
     * Role : réatribut les valeurs de taille, remet la grille et le bool victoire à zéro
     * Entrée : - int width = largeur, vaut 10 par défaut
     *          - int height = hauteur, vaut 6 par défaut
     * Sortie : vide
     * */
    void reinit(int width = 10,int height = 6);

    /*
     * Role : Affiche le labyrinthe en ascii
     * Entrée : bool pause = true met pause entre chaque étape, false par défaut
     * Sortie : vide
     * */
    void display(bool pause=false);

    /*
     * Role : affiche le labyrinthe en 3D
     * Entrée : vide
     * Sortie : vide
     * */
    void display3D();

    /*
     * Role : dessine le labyrinthe en 2D
     * Entrée : QPainter * dessinateur = QPainter de la classe sur laquelle dessiner
     * Sortie : vide
     * */
    void display2D(QPainter * dessinateur);

    /*
     * Role : génère aléatoirement le labyrinthe
     * Entrée : bool show = true affiche en ascii les étapes de génération, false par défaut
     * Sortie : vide
     * */
    void generate(bool show=false);

    /*
     * Role : gère la victoire, la collision et la découverte de la sphere
     * Entrée : - double pos_x = position en x du joueur
     *          - double pos_y = position en y du joueur
     * Sortie : bool = true si le déplacement est valide, sinon false
     * */
    bool gestionPos(double pos_x, double pos_y);
    bool getVictoire() {return victoire;};
signals:

    // inutilisé
     void nouvellePartie();
};

#endif // MAZE_H
