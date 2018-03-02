#-------------------------------------------------
#
# Project created by QtCreator 2016-08-31T13:38:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OOP_Cpp_Ogre_prj1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    MarkerSqr.cpp \
    RenderObject.cpp \
    PolyLine.cpp \
    ellipse.cpp

HEADERS  += mainwindow.h \
    SdkQtCameraMan.h \
    MarkerSqr.h \
    RenderObject.h \
    PolyLine.h \
    ellipse.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/include/OGRE \
               /root/Project2/GUIWindArrow \


LIBS += -lOgreMain \
        "-L/root/Project2/build-GUIWindArrow-Desktop-Debug" -lGUIWindArrow
