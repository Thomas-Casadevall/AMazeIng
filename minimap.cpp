#include "minimap.h"

#include <QDebug>
#include <QPainter>
#include <cmath>

// PI
#define PI 3.14159265

MiniMap::MiniMap(Maze* laby_, float c_,QWidget *parent) : QWidget(parent)
{
    laby = laby_;
    c = c_;
}

void MiniMap::dessine(double pos_x_, double pos_y_, int angleVue_) {
    pos_x = pos_x_;
    pos_y = pos_y_;

    angleVue = angleVue_;

    repaint();
}

void MiniMap::paintEvent(QPaintEvent *){

    QPainter dessinateur(this);

    dessinateur.setPen(Qt::red);
    dessinateur.setWindow(0, laby->getHeight() * 10, laby->getWidth() * 10, -laby->getHeight() * 10);


    laby->display2D(& dessinateur);


    dessinateur.setPen(Qt::red);

    // position du joueur
    int posy = pos_y / c * 10 + 4;
    int posx = pos_x / c * 10 + 4;
    dessinateur.drawEllipse(QPoint(posy, posx), 2, 2);

    // longueur de la ligne représentant l'angle de vue
    int longy = 4 * sin(angleVue * PI / 180.0);
    int longx = 4 * cos(angleVue * PI / 180.0);

    // pour l'agrandir si elle est trop petite (à cause des arrondis)
    if ((longx * longx + longy * longy) < 16)
    {
        longy+= (longy >= 0) - (longy <= 0); // +1 si >0, -1 si <0 ou 0 si =0
        longx+= (longx >= 0) - (longx <= 0);
    }

    dessinateur.drawLine(posy, posx, posy + longy, posx + longx);

}
