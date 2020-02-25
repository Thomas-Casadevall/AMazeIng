#include <QApplication>
#include <ctime>
#include "mazeWidget.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    // Initialisation du generateur de nombres aleatoires
    srand(time(nullptr));

    // Creation de l'application QT
    QApplication app(argc, argv);
	
	// Creation du widget opengl
    MainWindow fenetre;
    fenetre.show();

    // Execution de l'application QT
    return app.exec();
}
