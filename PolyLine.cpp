#include "PolyLine.h"
#include "RenderObject.h"
int lineCount = 0;
PolyLine::PolyLine(Ogre::SceneManager *m_ogreSceneMgr , const QString &name)//Constructor
{
    _name = name;
    m_ogreSceneMgrPL = m_ogreSceneMgr;
    init();
}

PolyLine::~PolyLine()//Destructor
{
    m_ogreSceneMgrPL->destroyManualObject(_pt);
    m_ogreSceneMgrPL->destroySceneNode(_node3_1);
    for(int i = 0; i< _listOfMarkers.count(); i++ )
    {
        delete _listOfMarkers[i];
    }
}

void PolyLine::addPoint( const Ogre::Vector3 &position )//добавить точку
{
    _listOfMarkers.push_back( new MarkerSqr( m_ogreSceneMgrPL, position) );

    updateGeometry();
}

void PolyLine::addNewMarker(const Ogre::Vector3 &posSqr)//добавить новую точку
{
    _listOfNewMarkers.push_back( new MarkerSqr( m_ogreSceneMgrPL, posSqr) );
    updateNewGeometry();
}

void PolyLine::setName( const QString &name )
{
    _name = name;
}

void PolyLine::cancelNewMarker()//9 Cancel
{
    for( int i=0; i<_listOfNewMarkers.count(); i++ )//отмена добавления новых точек
    {
        m_ogreSceneMgrPL->destroyManualObject(_listOfNewMarkers[i]->getMarkerName());
    }
    _listOfNewMarkers.clear();
    if(_listOfMarkersCopy.count()>0)//отмена удаления точек
    {
        for( int i=0; i<_listOfMarkers.count(); i++ )
        {
            m_ogreSceneMgrPL->destroyManualObject(_listOfMarkers[i]->getMarkerName());
        }
        _listOfMarkers.clear();
        for( int i=0; i<_listOfMarkersCopy.count(); i++ )
        {
            _listOfMarkers.push_back( new MarkerSqr( m_ogreSceneMgrPL, _listOfMarkersCopy[i]->getPosition()) );
        }
    }
    if(_listSqrPos.count()>0)//отмена перемещения точек
    {
        for( int i=0; i<_listOfMarkers.count(); i++ )
        {
            m_ogreSceneMgrPL->destroyManualObject(_listOfMarkers[i]->getMarkerName());
        }
        _listOfMarkers.clear();
        for( int i=0; i<_listSqrPos.count(); i++)
        {
            _listOfMarkers.push_back( new MarkerSqr( m_ogreSceneMgrPL, _listSqrPos[i]));
        }
    }
    _listOfMarkersCopy.clear();
    _listSqrPos.clear();
    _pt->clear();
    updateGeometry();
    addSelectMarker();
}

void PolyLine::saveNewMarker()//8 Save
{
    for( int i=0; i<_listOfNewMarkers.count(); i++ )
    {
        _listOfMarkers.push_back(_listOfNewMarkers[i]);
    }
    _listOfNewMarkers.clear();
    _listOfMarkersCopy.clear();
    _listSqrPos.clear();
    _pt->clear();
    updateGeometry();
    addSelectMarker();
}

void PolyLine::removeMarker()//удаление точек
{
    if(_listOfMarkersCopy.isEmpty())
    {
    _listOfMarkersCopy = _listOfMarkers;
    }
    for(int i = 0; i< _listOfMarkers.count(); i++ )
    {
        if(_listOfMarkers[i]->getMarkerName() == _new_iterator->movable->getName()) //если имя объекта типа квадрат равно объекту, на который кликнули мышью, то:
        {
            m_ogreSceneMgrPL->destroyManualObject(_listOfMarkers[i]->getMarkerName()); //удалить объект типа квадрат
            _listOfMarkers.removeAt(i);
            break;
        }
    }
    _pt->clear();
    updateGeometry();
}

void PolyLine::addIterator(const Ogre::RaySceneQueryResult::iterator &new_iterator)//итератор для удаления точек
{
    _new_iterator = new_iterator;
}

void PolyLine::addMoveIterator(const Ogre::RaySceneQueryResult::iterator &move_it)//итератор для перемещения точек
{
    _move_it = move_it;//I like to move it, move it! :-)
}

void PolyLine::addMoveMarker(const Ogre::Vector3 &posMove)//перемещение точек
{
    if(_listSqrPos.isEmpty())
    {
        for(int i=0; i<_listOfMarkers.count(); i++)
        {
            _listSqrPos.push_back(_listOfMarkers[i]->getPosition());
        }
    }
    for(int i = 0; i< _listOfMarkers.count(); i++ )
    {
        if(_listOfMarkers[i]->getMarkerName() == _move_it->movable->getName()) //если имя объекта типа квадрат равно объекту, на который кликнули мышью, то:
        {
            m_ogreSceneMgrPL->getSceneNode(_listOfMarkers[i]->getNodeName())->setPosition(posMove);
        }
    }
    _pt->clear();
    updateGeometry();
}

void PolyLine::addSelectMarker()
{
    for(int i = 0; i< _listOfMarkers.count(); i++ )
    {
        m_ogreSceneMgrPL->getManualObject(_listOfMarkers[i]->getMarkerName())->setMaterialName(0,"redKvadrat");
    }
}

void PolyLine::updateSelectMarker()
{
    for(int i = 0; i< _listOfMarkers.count(); i++ )
    {
        m_ogreSceneMgrPL->getManualObject(_listOfMarkers[i]->getMarkerName())->setMaterialName(0,"BaseWhiteNoLighting");
    }
}

Ogre::String PolyLine::getMOName()//вернуть имя объекта типа линия
{
    return _pt->getName();
}

QString PolyLine::getName()
{
    return _name;
}

void PolyLine::updateGeometry()//перерисовать линию
{
    _pt->clear();
    _pt->begin( "BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP );
    _pt->colour(0.7f, 0.7f, 0.7f);
    for(int i = 0; i< _listOfMarkers.count(); i++ )
    {
        _pt->position( _listOfMarkers[i]->getPosition() );
    }
    _pt->end();
}

void PolyLine::updateNewGeometry()//перерисовать линию в режиме Add Point
{
    _pt->begin( "BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP );
    _pt->colour(0.7f, 0.7f, 0.7f);
    if(_listOfMarkers.count()>0)
    {
    _pt->position( _listOfMarkers.last()->getPosition() );
    }
    for(int i = 0; i< _listOfNewMarkers.count(); i++ )
    {
        _pt->position( _listOfNewMarkers[i]->getPosition() );
    }
    _pt->end();
}

void PolyLine::init()//создать объект типа линия
{
    lineCount++;
    _pt = m_ogreSceneMgrPL->createManualObject("PointList"+Ogre::StringConverter::toString( lineCount ) );
    _pt->setDynamic(true); //задать объекту динамический тип
    _pt->estimateVertexCount(1000); //задать объекту резервное количество точек
    _node3_1 = m_ogreSceneMgrPL->getRootSceneNode()->createChildSceneNode("polyLineNode" + Ogre::StringConverter::toString( lineCount ) );
    _node3_1->attachObject( _pt );
    _node3_1->setPosition( Ogre::Vector3::ZERO );
}
