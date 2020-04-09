#ifndef MINIMAP_H
#define MINIMAP_H

#include <QWidget>

#include <Maze/cell.h>
#include <Maze/maze.h>

class MiniMap : public QWidget
{
    Q_OBJECT
public:
    explicit MiniMap(Maze* laby_, float c, QWidget *parent = nullptr);

    void dessine(double pos_x, double pos_y, int angleVue_);

signals:

protected:
  //overload from QWidget
  void paintEvent(QPaintEvent* event);

private:

    float c;

    Maze * laby;

    double pos_y, pos_x;
    int angleVue;

};

#endif // MINIMAP_H
