#ifndef Ellipse_H
#define Ellipse_H

#include <QMainWindow>
#include <Ogre.h>
#include "SdkQtCameraMan.h"

class Ellipse1
{
public:
    Ellipse1(Ogre::SceneManager *m_ogreSceneMgr, const QString &name, const Ogre::Vector3 &position, const int &Hoz, const int &Vert, QColor cvet);
    ~Ellipse1();

    void initL(Ogre::Vector3 pos);
    void initF(Ogre::Vector3 pos);
    void changeSizeHoz(int sizeE);
    void changeSizeVert(int sizeE);
    void recolorEllipse1(QColor colour);
    void setNameEllipse1( const QString &name );
    void updateEllipse1GeometryL();
    void updateEllipse1GeometryF();

    QString getNameEllipse1();
    Ogre::String getEllName();
    Ogre::String getEll1Name();

private:
    Ogre::SceneManager* m_ogreSceneMgrEllipse1;
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

#endif // Ellipse_H
