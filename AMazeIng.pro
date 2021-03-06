# configuration Qt
QT       += core gui opengl widgets
TEMPLATE  = app

# ajout des libs au linker
win32 {
    win32-msvc* {
        LIBS     += opengl32.lib glu32.lib
    } else {
        LIBS     += -lopengl32 -lglu32
    }
}
else {
        LIBS     += -lGL -lGLU
}


# nom de l'exe genere
TARGET 	  = AMazeIng

# fichiers sources/headers/ressources
SOURCES += \
    Maze/cell.cpp \
    Maze/maze.cpp \
    detectmotion.cpp \
    main.cpp \
    mainwindow.cpp \
    mazeWidget.cpp \
    minimap.cpp

HEADERS += \
    Maze/cell.h \
    Maze/maze.h \
    detectmotion.h \
    mainwindow.h \
    mazeWidget.h \
    minimap.h

INCLUDEPATH +=$$(OPENCV_DIR)\..\..\include

LIBS += -L$$(OPENCV_DIR)\lib \
        -lopencv_core420 \
        -lopencv_highgui420 \
        -lopencv_imgproc420 \
        -lopencv_imgcodecs420 \
        -lopencv_videoio420 \
        -lopencv_features2d420 \
        -lopencv_calib3d420 \
        -lopencv_objdetect420


FORMS += \
    mainwindow.ui

RESOURCES += \
    ressources/ressources.qrc
