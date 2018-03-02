#include "ellipse.h"
//int a1=10;
//int b1=90;
int x1=-90;
int x2=-90;
int x3=-90;
int x4=-90;
float z1;
float z2;
float z3;
float z4;
int i;
int j;
int o;
int p;

int el1=0;
int el2=0;

Ellipse::Ellipse(Ogre::SceneManager *m_ogreSceneMgr,
                 const QString &name,
                 const Ogre::Vector3 &position,
                 const int &Hoz,
                 const int &Vert,
                 QColor cvet)//Constructor
{
    m_ogreSceneMgrEllipse = m_ogreSceneMgr;
    _nameEll = name;
    _position=position;
    a1=Hoz;
    b1=Vert;
    _cvet=cvet;
    _cvet.getRgbF(&r11, &g11, &b11);
    initL(_position);
    initF(_position);
}

Ellipse::~Ellipse()//Destructor
{
    m_ogreSceneMgrEllipse->destroyManualObject(ell);
    m_ogreSceneMgrEllipse->destroySceneNode(nodeEll);
    m_ogreSceneMgrEllipse->destroyManualObject(ell1);
    m_ogreSceneMgrEllipse->destroySceneNode(nodeEll1);
}

void Ellipse::initL(Ogre::Vector3 pos)//Создание закрашенного объекта типа эллипс
{
    el1++;
    ell = m_ogreSceneMgrEllipse->createManualObject("ellipse"+Ogre::StringConverter::toString( el1 ) );
    updateEllipseGeometryL();
    nodeEll = m_ogreSceneMgrEllipse->createSceneNode("nodeEllipse"+Ogre::StringConverter::toString( el1 ) );
    m_ogreSceneMgrEllipse->getRootSceneNode()->addChild( nodeEll );
    nodeEll->attachObject(ell);
    nodeEll->setPosition( pos );
}

void Ellipse::initF(Ogre::Vector3 pos)//Создание линии типа эллипс
{
    el2++;
    ell1 = m_ogreSceneMgrEllipse->createManualObject("ellipseLine"+Ogre::StringConverter::toString( el2 ) );
    updateEllipseGeometryF();
    nodeEll1 = m_ogreSceneMgrEllipse->createSceneNode("nodeEllipseLine"+Ogre::StringConverter::toString( el2 ) );
    m_ogreSceneMgrEllipse->getRootSceneNode()->addChild( nodeEll1 );
    nodeEll1->attachObject(ell1);
    nodeEll1->setPosition( pos );

}

void Ellipse::changeSizeHoz(int sizeE)//Изменение ширины эллипса
{
    a1 = sizeE;
    updateEllipseGeometryL();
    updateEllipseGeometryF();
    ell1->setMaterialName(0,"redKvadrat");
}

void Ellipse::changeSizeVert(int sizeE)//Изменение высоты эллипса
{
    b1 = sizeE;
    updateEllipseGeometryL();
    updateEllipseGeometryF();
    ell1->setMaterialName(0,"redKvadrat");
}

void Ellipse::recolorEllipse(QColor colour)//Изменение цвета эллипса
{
    _cvet = colour;
    _cvet.getRgbF(&r11, &g11, &b11);
    updateEllipseGeometryL();
    updateEllipseGeometryF();
    ell1->setMaterialName(0,"redKvadrat");
}

void Ellipse::setNameEllipse(const QString &name)//Задание имени эллипса
{
    _nameEll = name;
}

void Ellipse::updateEllipseGeometryL()//Рисование заполненного эллипса
{
    ell->clear();
    ell->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_STRIP);
    ell->colour(Ogre::ColourValue(r11, g11, b11));
    ell->position(0,0,0);
    for(i = 0; i<181; i++)
    {
        z1=-sqrt(b1*b1-(b1*b1*x1*x1/(a1*a1)));
        ell->position(x1, 0, z1);
        x1++;
    }
    for(j = 0; j<181; j++)
    {
        z2=sqrt(b1*b1-(b1*b1*x2*x2/(a1*a1)));
        ell->position(x2, 0, z2);
        ell->index(i+j);
        x2++;
    }
    for(int k=1; k<i+j; k++)
    {
        ell->index(0);
        ell->index(k);
        ell->index(k+1);
    }
    ell->end();
    x1=-90;
    x2=-90;
}

void Ellipse::updateEllipseGeometryF()//Рисовании линии эллипса
{
    ell1->clear();
    ell1->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);
    ell1->colour(1.0f, 1.0f, 1.0f);
    for(o = 0; o<181; o++)
    {
        z3=-sqrt(b1*b1-(b1*b1*x3*x3/(a1*a1)));
        ell1->position(x3, 0, z3);
        x3++;
    }
    for(p = 0; p<181; p++)
    {
        z4=sqrt(b1*b1-(b1*b1*x4*x4/(a1*a1)));
        ell1->position(x4, 0, z4);
        x4++;
    }
    ell1->end();
    x3=-90;
    x4=-90;
}

QString Ellipse::getNameEllipse()//Получение имени эллипса
{
    return _nameEll;
}

Ogre::String Ellipse::getEllName()//Получение имени объекта типа эллипс (закрашенный)
{
    return ell->getName();
}

Ogre::String Ellipse::getEll1Name()//Получение имени объекта типа эллипс (линия)
{
    return ell1->getName();
}


