#ifndef MINIMAP_H
#define MINIMAP_H

#include <QWidget>

#include <Maze/cell.h>
#include <Maze/maze.h>


// Auteur : Thomas Casadevall
class MiniMap : public QWidget
{
    Q_OBJECT
public:
    explicit MiniMap(Maze* laby_, float c, QWidget *parent = nullptr);

    /*
     * Role : enregistre les valeur de paramètres d'entrée et met à jour l'affichage
     * Entrée : - double pos_x = position en x du joueur
     *          - double pos_y = position en z du joueur (y étant la hauteur)
     * Sortie : vide
     * */
    void dessine(double pos_x, double pos_y, int angleVue_);

    // si 0 autorise l'affichage de la minimap, sinon non
    int affichageAutorise = 0;

signals:

protected:
    /*
     * Role : surchargé de QWidget, met à jour l'affichage en redessinant
     * Entrée : QPaintEvent* event = créé par Qt lors de l'appel de la méthode
     * Sortie : vide
     * */
  void paintEvent(QPaintEvent* event);

private:

    // taille d'un coté d'une cellule
    float c;

    // pointeur vers le Maez
    Maze * laby;

    // Position du joueur
    double pos_y, pos_x;
    // angle de rotation du joueur
    int angleVue;

};

#endif // MINIMAP_H
