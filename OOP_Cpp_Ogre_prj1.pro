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

LIBS += -LE:\Ogre3d\build-ogre-1.10.11\bin -lOgreMain

INCLUDEPATH += E:/OGRE/include/OGRE
DEPENDPATH += E:/OGRE/include/OGRE

#INCLUDEPATH += /usr/local/include/OGRE \

#LIBS += -L$(OGRE_HOME)\\lib -lOgreMain
#INCLUDEPATH += $(OGRE_HOME)\\include \
#INCLUDEPATH += $(OGRE_HOME)\\include\\OGRE \
#INCLUDEPATH += $(OGRE_HOME)\\lib \
#               /root/Project2/GUIWindArrow \


#LIBS += -lOgreMain
#        "-L/root/Project2/build-GUIWindArrow-Desktop-Debug" -lGUIWindArrow
