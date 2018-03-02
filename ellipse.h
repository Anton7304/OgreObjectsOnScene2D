#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <QMainWindow>
#include <Ogre.h>
#include "SdkQtCameraMan.h"

class Ellipse
{
public:
    Ellipse(Ogre::SceneManager *m_ogreSceneMgr, const QString &name, const Ogre::Vector3 &position, const int &Hoz, const int &Vert, QColor cvet);
    ~Ellipse();

    void initL(Ogre::Vector3 pos);
    void initF(Ogre::Vector3 pos);
    void changeSizeHoz(int sizeE);
    void changeSizeVert(int sizeE);
    void recolorEllipse(QColor colour);
    void setNameEllipse( const QString &name );
    void updateEllipseGeometryL();
    void updateEllipseGeometryF();

    QString getNameEllipse();
    Ogre::String getEllName();
    Ogre::String getEll1Name();

private:
    Ogre::SceneManager* m_ogreSceneMgrEllipse;
    QString _nameEll;
    Ogre::ManualObject* ell;
    Ogre::SceneNode* nodeEll;
    Ogre::ManualObject* ell1;
    Ogre::SceneNode* nodeEll1;
    Ogre::ColourValue color;
    Ogre::Vector3 _position;
    int a1;
    int b1;
    QColor _cvet;
    qreal r11;
    qreal g11;
    qreal b11;
};

#endif // ELLIPSE_H
