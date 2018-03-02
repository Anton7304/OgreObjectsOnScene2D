#include "MarkerSqr.h"
#include "RenderObject.h"

int kv = 0;

int kv1 = 0;

MarkerSqr::MarkerSqr( Ogre::SceneManager* m_ogreSceneMgr, const Ogre::Vector3 &position )//Constructor
{
    m_ogreSceneMgrMarker = m_ogreSceneMgr;
    kvadrat( position );
}

MarkerSqr::~MarkerSqr()//Destructor
{
    m_ogreSceneMgrMarker->destroyManualObject(kvadrat1);
    m_ogreSceneMgrMarker->destroySceneNode(node3);
}


Ogre::Vector3 MarkerSqr::getPosition()//получить координаты квадрата (точки)
{
    return node3->getPosition();
}

Ogre::String MarkerSqr::getMarkerName()//получить имя квадрата (точки)
{
    return kvadrat1->getName();//node3->getName();
}

Ogre::String MarkerSqr::getNodeName()//получить имя узла сцены квадрата (точки)
{
    return node3->getName();
}

void MarkerSqr::kvadrat( Ogre::Vector3 pos )//нарисовать квадрат
{

    kv++;

    QString txt2;
    txt2=QString::number(kv);
    kvadrat1 = m_ogreSceneMgrMarker->createManualObject("kvadrat"+txt2.toStdString());//создать объект типа manualObject

    // specify the material (by name) and rendering type
    kvadrat1->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_FAN);
    kvadrat1->colour(0.9f, 0.9f, 0.9f);
    kvadrat1->position(10, 0, 10);
    kvadrat1->position(10, 0, -10);
    kvadrat1->position(-10, 0,-10);
    kvadrat1->position(-10, 0, 10);
    kvadrat1->index(0);
    kvadrat1->index(1);
    kvadrat1->index(2);
    kvadrat1->index(3);
    kvadrat1->index(0);
    kvadrat1->end();
    //kvadrat1->setMaterialName(0,"OgreSkin");


    node3 = m_ogreSceneMgrMarker->createSceneNode("kvadratnode"+txt2.toStdString());//создать узел сцены
    m_ogreSceneMgrMarker->getRootSceneNode()->addChild( node3 );//добавить узел сцены на экран

    node3->setPosition( pos );//задать узлу сцены позицию на экране
    node3->attachObject( kvadrat1 );//нарисовать созданный квадрат на узле

    //kvadrat1->setQueryFlags(1);
}

void MarkerSqr::selectKvadrat(Ogre::Vector3 pos)//not using
{
    kv1++;
    QString txt2;
    txt2=QString::number(kv1);
    kvadrat2 = m_ogreSceneMgrMarker->createManualObject("kvadratselect"+txt2.toStdString());//создать объект типа manualObject

    // specify the material (by name) and rendering type
    kvadrat2->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_FAN);
    kvadrat2->colour(0.9f, 0.1f, 0.1f);
    kvadrat2->position(10, 0, 10);
    kvadrat2->position(10, 0, -10);
    kvadrat2->position(-10, 0,-10);
    kvadrat2->position(-10, 0, 10);
    kvadrat2->index(0);
    kvadrat2->index(1);
    kvadrat2->index(2);
    kvadrat2->index(3);
    kvadrat2->index(0);
    kvadrat2->end();

    node3 = m_ogreSceneMgrMarker->createSceneNode("kvadratselectnode"+txt2.toStdString());//создать узел сцены
    m_ogreSceneMgrMarker->getRootSceneNode()->addChild( node3 );//добавить узел сцены на экран

    node3->setPosition( pos );//задать узлу сцены позицию на экране
    node3->attachObject( kvadrat2 );//нарисовать созданный квадрат на узле
}




