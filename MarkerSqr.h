#ifndef MARKERSQR_H
#define MARKERSQR_H

#include <QMainWindow>
#include <Ogre.h>
#include "SdkQtCameraMan.h"

class MarkerSqr
{
public:
    MarkerSqr( Ogre::SceneManager* m_ogreSceneMgr, const Ogre::Vector3 &position );//Constructor
    ~MarkerSqr();//Destructor
    Ogre::Vector3 getPosition();
    Ogre::String getMarkerName();
    Ogre::String getNodeName();
    void selectKvadrat( Ogre::Vector3 pos );

private:
    void kvadrat( Ogre::Vector3 pos );

    Ogre::SceneManager* m_ogreSceneMgrMarker;
    Ogre::SceneNode* node3;
    Ogre::ManualObject* kvadrat1;
    Ogre::ManualObject* kvadrat2;

    //static int kv;
};

#endif // MARKERSQR_H
