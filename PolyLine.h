#ifndef POLYLINE_H
#define POLYLINE_H

#include <QMainWindow>
#include <Ogre.h>
#include "MarkerSqr.h"

class PolyLine
{

public:
    PolyLine( Ogre::SceneManager *m_ogreSceneMgr, const QString &name );
    ~PolyLine();
    void addPoint( const Ogre::Vector3 &position );
    void addNewMarker( const Ogre::Vector3 &posSqr );
    void setName( const QString &name );
    void cancelNewMarker();
    void saveNewMarker();
    void removeMarker();
    void addIterator( const Ogre::RaySceneQueryResult::iterator &new_iterator);
    void addMoveIterator( const Ogre::RaySceneQueryResult::iterator &move_it);
    void addMoveMarker(const Ogre::Vector3 &posMove);
    void addSelectMarker();
    void updateSelectMarker();
    Ogre::String getMOName();


    QString getName();
    void init();

private:
    void updateGeometry();
    void updateNewGeometry();
    Ogre::ManualObject *_pt;
    Ogre::SceneNode* _node3_1;
    Ogre::SceneManager* m_ogreSceneMgrPL;
    QList<MarkerSqr*> _listOfMarkers;
    QList<MarkerSqr*> _listOfNewMarkers;
    QList<MarkerSqr*> _listOfMarkersCopy;
    QList<MarkerSqr*> _listOfMarkersMove;
    QList<Ogre::Vector3> _listSqrPos;
    QString _name;
    Ogre::RaySceneQueryResult::iterator _new_iterator;
    Ogre::RaySceneQueryResult::iterator _move_it;
};

#endif // POLYLINE_H
