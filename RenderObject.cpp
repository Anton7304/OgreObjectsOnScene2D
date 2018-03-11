/**
|=======================================================================|
|=======================================================================|
|----------------------------FOR LINUX ONLY-----------------------------|
|=======================================================================|
|=======================================================================|
*/

#include "RenderObject.h"

/*RenderObject::RenderObject(QWidget *parent) : QMainWindow(parent)
{

}*/

//#include "RenderObject.h"
#include <OgreManualObject.h>
#include <QMessageBox>
#include "mainwindow.h"
#include "MarkerSqr.h"
#include <OgreSceneQuery.h>
//#include <guiwindarrow.h>

int txt1=5;
int indx=0;
int a=0;
int b=0;
int pli=1;
QString txt3=QString::number(pli);

/*
Note that we pass any supplied QWindow parent to the base QWindow class. This is necessary should we
need to use our class within a container.
*/
RenderObject::RenderObject(QWindow *parent)
    : QWindow(parent)
    , m_update_pending(false)
    , m_animating(false)
    , m_ogreRoot(NULL)
    , m_ogreWindow(NULL)
    , m_ogreCamera(NULL)
    , m_cameraMan(NULL)
{
    setTitle("Viewer");//заголовок окна
    setAnimating(true);
    installEventFilter(this);
    m_ogreBackground = Ogre::ColourValue(0.2f, 0.2f, 0.8f);//цвет фона
}

/*
Upon destruction of the QWindow object we destroy the Ogre3D scene.
*/
RenderObject::~RenderObject()
{
    if (m_cameraMan) delete m_cameraMan;
    //delete m_ogreRoot; //delete render, not working correctly
}

/*
In case any drawing surface backing stores (QRasterWindow or QOpenGLWindow) of Qt are supplied to this
class in any way we inform Qt that they will be unused.
*/
void RenderObject::render(QPainter *painter)
{
    qDebug()<<"Render";
    Q_UNUSED(painter);

}

/*
Our initialization function. Called by our renderNow() function once when the window is first exposed.
*/
void RenderObject::initialize()
{
    /*
    As shown Ogre3D is initialized normally; just like in other documentation.
    */

    m_ogreRoot = new Ogre::Root(Ogre::String("plugins.cfg"));
    Ogre::ConfigFile ogreConfig;



//	Commended out for simplicity but should you need to initialize resources you can do so normally.

    ogreConfig.load("resources.cfg");

    Ogre::ConfigFile::SectionIterator seci = ogreConfig.getSectionIterator();
    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
            archName, typeName, secName);
        }
    }
    const Ogre::RenderSystemList& rsList = m_ogreRoot->getAvailableRenderers();
    Ogre::RenderSystem* rs = rsList[0];
    /*
    This list setup the search order for used render system.
    */
    Ogre::StringVector renderOrder;
    renderOrder.push_back("OpenGL");
    renderOrder.push_back("OpenGL 3+");
    for (Ogre::StringVector::iterator iter = renderOrder.begin(); iter != renderOrder.end(); iter++)
    {
        for (Ogre::RenderSystemList::const_iterator it = rsList.begin(); it != rsList.end(); it++)
        {
            if ((*it)->getName().find(*iter) != Ogre::String::npos)
            {
                rs = *it;
                break;
            }
        }
        if (rs != NULL) break;
    }
    if (rs == NULL)
    {
        if (!m_ogreRoot->restoreConfig())
        {
            if (!m_ogreRoot->showConfigDialog())
                OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                    "Abort render system configuration",
                    "RenderObject::initialize");
        }
    }
    /*-----------------------------------WINDOW_MODE---------------------------------
    Setting size and VSync on windows will solve a lot of problems
    */
    QString dimensions = QString("%1 x %2").arg(this->width()).arg(this->height());
    rs->setConfigOption("Video Mode", dimensions.toStdString());
    rs->setConfigOption("Full Screen", "No");
    rs->setConfigOption("VSync", "Yes");
    m_ogreRoot->setRenderSystem(rs);
    m_ogreRoot->initialise(false);

    Ogre::NameValuePairList parameters;
    /*
    Flag within the parameters set so that Ogre3D initializes an OpenGL context on it's own.
    */
    if (rs->getName().find("GL") <= rs->getName().size())
        parameters["currentGLContext"] = Ogre::String("false");

    /*
    We need to supply the low level OS window handle to this QWindow so that Ogre3D knows where to draw
    the scene. Below is a cross-platform method on how to do this.
    If you set both options (externalWindowHandle and parentWindowHandle) this code will work with OpenGL
    and DirectX.
    */

    parameters["externalWindowHandle"] = Ogre::StringConverter::toString((unsigned long)(this->winId()));
    parameters["parentWindowHandle"] = Ogre::StringConverter::toString((unsigned long)(this->winId()));
    /*
    Note below that we supply the creation function for the Ogre3D window the width and height
    from the current QWindow object using the "this" pointer.
    */
    m_ogreWindow = m_ogreRoot->createRenderWindow("QT Window",
        this->width(),
        this->height(),
        false,
        &parameters);
    m_ogreWindow->setVisible(true);

    /*--------------------------------------OGRE3D_CAMERA--------------------------------------------
    The rest of the code in the initialization function is standard Ogre3D scene code. Consult other
    tutorials for specifics.
    */
    m_ogreSceneMgr = m_ogreRoot->createSceneManager(Ogre::ST_GENERIC);
    m_ogreCamera = m_ogreSceneMgr->createCamera("MainCamera");
    m_ogreCamera->setPosition(Ogre::Vector3(0.0f, 590.0f, 0.0f));
    m_ogreCamera->setDirection(Ogre::Vector3(0.0f, 0.0f, 0.0f));
    m_ogreCamera->pitch(Ogre::Degree(270));
    m_ogreCamera->setNearClipDistance(0.1f);//минимальное расстояние от камеры
    m_ogreCamera->setFarClipDistance(2000.0f);//максимальное расстояние от камеры
    //m_ogreCamera->setPolygonMode(Ogre::PM_WIREFRAME);//полигональный режим просмотра объектов
    m_cameraMan = new OgreQtBites::SdkQtCameraMan(m_ogreCamera);   // create a default camera controller
    Ogre::Viewport* pViewPort = m_ogreWindow->addViewport(m_ogreCamera);
    pViewPort->setBackgroundColour(m_ogreBackground);
    m_ogreCamera->setAspectRatio(
        Ogre::Real(m_ogreWindow->getWidth()) / Ogre::Real(m_ogreWindow->getHeight()));
    m_ogreCamera->setAutoAspectRatio(true);
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    createScene();
    m_ogreRoot->addFrameListener(this);
}

void RenderObject::createScene()
{
    /*
    Example scene
    Derive this class for your own purpose and overwite this function to have a working Ogre widget with
    your own content.

    /*-------------------------------------------LIGHT----------------------------------------------------------------------------------------------*/
    Ogre::Light* light = m_ogreSceneMgr->createLight("MainLight");
    light->setType(Ogre::Light::LT_POINT);
    light->setPosition(20.0f, 80.0f, 50.0f);
    Ogre::Light* light2 = m_ogreSceneMgr->createLight("MainLight2");
    light2->setType(Ogre::Light::LT_POINT);
    light2->setPosition(20.0f, 80.0f, -50.0f);
    Ogre::Light* light3 = m_ogreSceneMgr->createLight("MainLight3");
    light3->setType(Ogre::Light::LT_POINT);
    light3->setPosition(-20.0f, 80.0f, 50.0f);
    Ogre::Light* light4 = m_ogreSceneMgr->createLight("MainLight4");
    light4->setType(Ogre::Light::LT_POINT);
    light4->setPosition(20.0f, -80.0f, 50.0f);
    //light->setDiffuseColour(1.0f, 1.0f, 1.0f);
    //light->setDirection(Ogre::Vector3(1,0,0));
}

void RenderObject::render()
{
    /*
    How we tied in the render function for OGre3D with QWindow's render function. This is what gets call
    repeatedly. Note that we don't call this function directly; rather we use the renderNow() function
    to call this method as we don't want to render the Ogre3D scene unless everything is set up first.
    That is what renderNow() does.

    Theoretically you can have one function that does this check but from my experience it seems better
    to keep things separate and keep the render function as simple as possible.
    */
    Ogre::WindowEventUtilities::messagePump();
    m_ogreRoot->renderOneFrame();
}

void RenderObject::renderLater()
{
    /*
    This function forces QWindow to keep rendering. Omitting this causes the renderNow() function to
    only get called when the window is resized, moved, etc. as opposed to all of the time; which is
    generally what we need.
    */
    if (!m_update_pending)
    {
        m_update_pending = true;
        QApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

bool RenderObject::event(QEvent *event)
{
    /*
    QWindow's "message pump". The base method that handles all QWindow events. As you will see there
    are other methods that actually process the keyboard/other events of Qt and the underlying OS.

    Note that we call the renderNow() function which checks to see if everything is initialized, etc.
    before calling the render() function.
    */

    switch (event->type())
    {
    case QEvent::UpdateRequest:
        m_update_pending = false;
        renderNow();
        return true;

    default:
        return QWindow::event(event);
    }
}

/*
Called after the QWindow is reopened or when the QWindow is first opened.
*/
void RenderObject::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        renderNow();
}

/*
The renderNow() function calls the initialize() function when needed and if the QWindow is already
initialized and prepped calls the render() method.
*/
void RenderObject::renderNow()
{
    if (!isExposed())
        return;

    if (m_ogreRoot == NULL)
    {
        initialize();
    }

    render();

    if (m_animating)
        renderLater();
}

/*
Our event filter; handles the resizing of the QWindow. When the size of the QWindow changes note the
call to the Ogre3D window and camera. This keeps the Ogre3D scene looking correct.
*/
bool RenderObject::eventFilter(QObject *target, QEvent *event)
{
    if (target == this)
    {
        if (event->type() == QEvent::Resize)
        {
            if (isExposed() && m_ogreWindow != NULL)
            {
                m_ogreWindow->resize(this->width(), this->height());
            }
        }
    }

    return false;
}

/*
How we handle keyboard and mouse events.
*/
void RenderObject::keyPressEvent(QKeyEvent * ev)
{
    if(m_cameraMan)
        m_cameraMan->injectKeyDown(*ev);
}

void RenderObject::keyReleaseEvent(QKeyEvent * ev)
{
    if(m_cameraMan)
        m_cameraMan->injectKeyUp(*ev);
}

void RenderObject::mouseMoveEvent( QMouseEvent* e )//движение мыши (после клика)
{
    static int lastX = e->x();

    static int lastY = e->y();
    int relX = e->x() - lastX;

    int relY = e->y() - lastY;
    lastX = e->x();
    lastY = e->y();

    QPoint pos = e->pos();//запись точки клика в переменную

    Ogre::Ray mouseRay = m_ogreCamera->getCameraToViewportRay( (Ogre::Real)pos.x() / m_ogreWindow->getWidth(), (Ogre::Real)pos.y() / m_ogreWindow->getHeight() );
    Ogre::Vector3 posPointMove = mouseRay.getPoint(m_ogreCamera->getPosition().y);
//------------------------------------MOVE_CAMERA-----------------------------------------------------------------
    if(m_cameraMan && (e->buttons() & Qt::RightButton) && _i_rpt!=10)
    {
        m_cameraMan->injectMouseMove(relX, relY);
    }

    if((e->buttons() & Qt::RightButton) && _i_rpt==10)//если нажаты правая кнопка мыши и кнопка Move Point, то перемещать точку
    {
        bool mMovableFound = false;

        Ogre::RaySceneQuery* pSceneQuery = m_ogreSceneMgr->createRayQuery(mouseRay);
        pSceneQuery->setSortByDistance(true);

        Ogre::RaySceneQueryResult vResult = pSceneQuery->execute();
        for (size_t ui = 0; ui < vResult.size(); ui++)
        {
            if (vResult[ui].movable)
            {
                if (vResult[ui].movable->getMovableType().compare("Entity") == 0)
                {
                    emit entitySelected((Ogre::Entity*)vResult[ui].movable);
                }
            }
        }

        Ogre::RaySceneQueryResult::iterator it = vResult.begin();
        mMovableFound = false;
        for (it = vResult.begin(); it != vResult.end(); it++)
        {
            for(int i=0; i<_listOfPolyLine.count(); i++)
            {
                if(_listOfPolyLine[i]->getName()==_mp)
                {
                mMovableFound =
                    it->movable &&
                    it->movable->getName() != "" &&
                    it->movable->getName() != "PlayerCam" &&
                    //it->movable->getName() != _listOfPolyLine[i]->getMOName() &&
                    it->movable->getName() != "ellipse1" &&
                    it->movable->getName() != "ellipseLine1" &&
                    it->movable->getName() != "ellipse2" &&
                    it->movable->getName() != "ellipseLine2" &&
                    it->movable->getName() != "ellipse3" &&
                    it->movable->getName() != "ellipseLine3" &&
                    it->movable->getName() != "ellipse4" &&
                    it->movable->getName() != "ellipseLine4" &&
                    it->movable->getName() != "ellipse4" &&
                    it->movable->getName() != "ellipseLine4" &&
                    it->movable->getName() != "ellipse5" &&
                    it->movable->getName() != "ellipseLine5" &&
                    it->movable->getName() != "ellipse6" &&
                    it->movable->getName() != "ellipseLine6" &&
                    it->movable->getName() != "PointList1" &&
                    it->movable->getName() != "PointList2" &&
                    it->movable->getName() != "PointList3" &&
                    it->movable->getName() != "PointList4" &&
                    it->movable->getName() != "PointList5" &&
                    it->movable->getName() != "PointList6" &&
                    it->movable->getName() != "PointList7" &&
                    it->movable->getName() != "PointList8" &&
                    it->movable->getName() != "PointList9" &&
                    it->movable->getName() != "PointList10";//игнорировать выделение линии.
                 }
            }
          if (mMovableFound)
          {
              //qDebug()<<QString::fromStdString( it->movable->getName() );
            node3 = it->movable->getParentSceneNode();
            break;
          }
        }

        for(int i=0; i<_listOfPolyLine.count(); i++)
        {
            if(_listOfPolyLine[i]->getName()==_mp)
            {
                _listOfPolyLine[i]->addMoveIterator(it);//передать переменную итератора в PolyLine
                _listOfPolyLine[i]->addMoveMarker( posPointMove );//передать координату точки в PolyLine
            }
        }
        m_ogreSceneMgr->destroyQuery(pSceneQuery);//удалить запрос pSceneQuery
    }
}

void RenderObject::wheelEvent(QWheelEvent *e)//прокрутка колесом мыши
{
    if(m_cameraMan)
        m_cameraMan->injectWheelMove(*e);
}
//-----------------------------------КЛИК_МЫШЫ------------------------------------------------------------------------------------------------------------------
void RenderObject::mousePressEvent(QMouseEvent* em )//нажатие на кнопку мыши
{
    if(m_cameraMan)
    {
        m_cameraMan->injectMouseDown(*em);
    }

    QPoint pos = em->pos();//запись точки клика в переменную

    Ogre::Ray mouseRay = m_ogreCamera->getCameraToViewportRay( (Ogre::Real)pos.x() / m_ogreWindow->getWidth(), (Ogre::Real)pos.y() / m_ogreWindow->getHeight() );
    Ogre::Vector3 posPoint = mouseRay.getPoint(m_ogreCamera->getPosition().y);
    //------------------------------------ЛЕВАЯ_МЫШ----------------------------------------------------------------------------------------------------------------

    if(em->buttons() & Qt::LeftButton)//если нажата левая кнопка мыши:
    {
        for(int i=0; i<_listOfPolyLine.count(); i++)
        {
            if(_listOfPolyLine[i]->getName()=="default_name")//если элемент не сохранен, то добавить точку
            {
                if(!_listOfPolyLine.isEmpty())
                {
                    _listOfPolyLine[i]->addPoint( posPoint );
                }
            }
            if(_listOfPolyLine[i]->getName()==_text && _i_rpt==6)//если элемент сохранен и выбран, добавить новую точку
            {
                _listOfPolyLine[i]->addNewMarker( posPoint );
            }

        }
        //for(int i=0; i<_listOfEllipse.count(); i++)
        //{
            if(/*_listOfEllipse[i]->getNameEllipse()=="default_ellipse" && */_i_ce == 111)
            {
                if(_listOfEllipse.isEmpty() || (_listOfEllipse.last()->getNameEllipse1()!="default_ellipse"))
                {
                    _listOfEllipse.push_back( new Ellipse1(m_ogreSceneMgr,"default_ellipse",posPoint,10,90,QColor(Qt::white)) );
                }
            }
        //}
    }
//-------------------------------------СРЕДНЯЯ_МЫШ--------------------------------------------------------------------------------------------------------------------
    if(em->buttons() & Qt::MidButton)//если нажата средняя кнопка мыши и нажата кнопка Remove Point, то по клику удалить точку
    {
        if(_i_rpt != 7)
        {
        bool mMovableFound = false;
        QPoint pos = em->pos();//запись точки клика в переменную
        Ogre::Ray mouseRay = m_ogreCamera->getCameraToViewportRay( (Ogre::Real)pos.x() / m_ogreWindow->getWidth(), (Ogre::Real)pos.y() / m_ogreWindow->getHeight() );
        Ogre::RaySceneQuery* pSceneQuery = m_ogreSceneMgr->createRayQuery(mouseRay);
        pSceneQuery->setSortByDistance(true);

        Ogre::RaySceneQueryResult vResult = pSceneQuery->execute();
        Ogre::RaySceneQueryResult::iterator it = vResult.begin();
        mMovableFound = false;
        //----------------------------------------------------
        for (it = vResult.begin(); it != vResult.end(); it++)
        {
//            for(int i=0; i<_listOfPolyLine.count(); i++)
//            {
//                if(_listOfPolyLine[i]->getName()==_rpt)
//                {
                mMovableFound =
                    it->movable &&
                    it->movable->getName() != "" &&
                    it->movable->getName() != "PlayerCam" &&
                    //it->movable->getName() != _listOfPolyLine[i]->getMOName() &&
                    //it->movable->getName() != "ellipse1" &&
                    //it->movable->getName() != "ellipseLine1" &&
//                    it->movable->getName() != "ellipse2" &&
//                    it->movable->getName() != "ellipseLine2" &&
//                    it->movable->getName() != "ellipse3" &&
//                    it->movable->getName() != "ellipseLine3" &&
//                    it->movable->getName() != "ellipse4" &&
//                    it->movable->getName() != "ellipseLine4" &&
//                    it->movable->getName() != "ellipse4" &&
//                    it->movable->getName() != "ellipseLine4" &&
//                    it->movable->getName() != "ellipse5" &&
//                    it->movable->getName() != "ellipseLine5" &&
//                    it->movable->getName() != "ellipse6" &&
//                    it->movable->getName() != "ellipseLine6" &&
                    it->movable->getName() != "PointList1" &&
                    it->movable->getName() != "PointList2" &&
                    it->movable->getName() != "PointList3" &&
                    it->movable->getName() != "PointList4" &&
                    it->movable->getName() != "PointList5" &&
                    it->movable->getName() != "PointList6" &&
                    it->movable->getName() != "PointList7" &&
                    it->movable->getName() != "PointList8" &&
                    it->movable->getName() != "PointList9" &&
                    it->movable->getName() != "PointList10";//игнорировать выделение объектов, которые не относятся к объекту типа квадрат

//                }
//            }
          if (mMovableFound)
          {
            node3 = it->movable->getParentSceneNode();
            break;
          }
        }
        for(int i=0; i<_listOfPolyLine.count(); i++)
        {
            if(_listOfPolyLine[i]->getName()==_rpt)
            {
                //_listOfPolyLine[i]->addIterator(it);
                //_listOfPolyLine[i]->removeMarker();
            }
        }
        if(_listOfEllipse.count()>0)
        {
            for(int j=0; j<_listOfEllipse.count();j++)
            {
                if(_listOfEllipse[j]->getEllName()==it->movable->getName())
                {

                    selectPoint(_listOfEllipse[j]->getNameEllipse1(),14);
                    MainWindow *m_wd = new MainWindow;
                    m_wd->setSelectList(_listOfEllipse[j]->getNameEllipse1());
                }
            }
        }
        //-------------------------------------------------------
        m_ogreSceneMgr->destroyQuery(pSceneQuery);//удалить запрос pSceneQuery
        }
    }
}

void RenderObject::mouseReleaseEvent( QMouseEvent* e )
{
    if(m_cameraMan)
        m_cameraMan->injectMouseUp(*e);
}

/*
Function to keep track of when we should and shouldn't redraw the window; we wouldn't want to do
rendering when the QWindow is minimized. This takes care of those scenarios.
*/
void RenderObject::setAnimating(bool animating)
{
    m_animating = animating;

    if (animating)
        renderLater();
}

void RenderObject::setCreateLine()//создать элемент
{
    _listOfPolyLine.push_back( new PolyLine(m_ogreSceneMgr,"default_name") );
}

void RenderObject::cancelCreateLine()//отменить создание элемента
{
    for(int i=0; i<_listOfPolyLine.count(); i++)
    {
        if(_listOfPolyLine[i]->getName()=="default_name")
        {
            delete _listOfPolyLine[i];
            _listOfPolyLine.removeAt(i);
        }
    }
}

void RenderObject::cancelModifyLine(QString cnl, int icnl)//отмена добавления новых точек к элементу
{
    for(int i=0; i<_listOfPolyLine.count(); i++)
        {
            if(_listOfPolyLine[i]->getName()==cnl)
            {
                _listOfPolyLine[i]->cancelNewMarker();
                _i_rpt = icnl;
            }
        }
}

void RenderObject::addNewPoint(QString text, int anp)//добавление новых точек к элементу
{
    _text = text;
    _i_rpt = anp;
}


void RenderObject::saveLine( QString name )//сохранение элемента
{
    _listOfPolyLine.last()->setName( name );
}

void RenderObject::saveModifyLine( QString name, int snl )//сохранение новых точек, добавленных к элементу
{
    for(int i=0; i<_listOfPolyLine.count(); i++)
    {
        if(_listOfPolyLine[i]->getName()==name)
        {
            _listOfPolyLine[i]->saveNewMarker();
            _i_rpt = snl;
        }
    }
}

void RenderObject::deleteLine(QString delpt)//удаление элемента
{
    for(int i=0; i<_listOfPolyLine.count(); i++)
    {
        if(_listOfPolyLine[i]->getName()==delpt)
        {
            delete _listOfPolyLine[i];
            _listOfPolyLine.removeAt(i);
        }
    }
}

void RenderObject::removePoint(QString rpt, int i_rpt)//удаление точек
{
    for(int i=0; i<_listOfPolyLine.count(); i++)
    {
        if(_listOfPolyLine[i]->getName()==rpt)
        {
            _i_rpt = i_rpt;
            _rpt = rpt;
        }
    }
}

void RenderObject::movePoint(QString mp, int i_mp)//перемещение точек
{
    for(int i=0; i<_listOfPolyLine.count(); i++)
    {
        if(_listOfPolyLine[i]->getName()==mp)
        {
            _i_rpt = i_mp;
            _mp = mp;
        }
    }
}

void RenderObject::selectPoint(QString sp, int i_sp)
{
    //qDebug()<<"count="<<_listOfPolyLine.count();
    for(int i=0; i<_listOfPolyLine.count(); i++)//For PolyLine
    {
        if(_listOfPolyLine[i]->getName()==sp)
        {
            _i_sp = i_sp;
            _sp = sp;
            //_listOfPolyLine[i]->updateSelectMarker();
            _listOfPolyLine[i]->addSelectMarker();
        }
        if(_listOfPolyLine[i]->getName()!=sp)
        {
            _listOfPolyLine[i]->updateSelectMarker();
        }
    }
    for(int j=0; j<_listOfEllipse.count(); j++)//For Ellipse
    {
        if(_listOfEllipse[j]->getNameEllipse1()==sp)
        {
            m_ogreSceneMgr->getManualObject(_listOfEllipse[j]->getEll1Name())->setMaterialName(0,"redKvadrat");
        }
        else
        {
            m_ogreSceneMgr->getManualObject(_listOfEllipse[j]->getEll1Name())->setMaterialName(0,"BaseWhiteNoLighting");
        }
    }
}

void RenderObject::createEllipse(int i_ce)
{
    _i_ce = i_ce;
//    _horiz = horiz;
//    _sPos = sPos;
//    _listOfEllipse.push_back( new Ellipse(m_ogreSceneMgr,"default_ellipse",posE) );
}

void RenderObject::cancelCreateEllipse(int i_de)
{
    for(int i=0; i<_listOfEllipse.count(); i++)
    {
        if(_listOfEllipse[i]->getNameEllipse1()=="default_ellipse")
        {
            delete _listOfEllipse[i];
            _listOfEllipse.removeAt(i);
            _i_ce = i_de;
        }
    }

}

void RenderObject::resizeEllipseHoz(int sPos, QString nPos)
{
    //_listOfEllipse.push_back( new Ellipse(m_ogreSceneMgr,"default_ellipse2") );
    for(int i=0; i<_listOfEllipse.count(); i++)
    {
        if(_listOfEllipse[i]->getNameEllipse1()==nPos)
        {
            _sPos = sPos;
            _listOfEllipse[i]->changeSizeHoz(_sPos);
        }
    }
}

void RenderObject::resizeEllipseVert(int sPos, QString nPos)
{
    for(int i=0; i<_listOfEllipse.count(); i++)
    {
        if(_listOfEllipse[i]->getNameEllipse1()==nPos)
        {
            _sPos = sPos;
            _listOfEllipse[i]->changeSizeVert(_sPos);
        }
    }
}

void RenderObject::setEllipseColor(QColor rgb, QString nRgb)
{
    for(int i=0; i<_listOfEllipse.count(); i++)
    {
        if(_listOfEllipse[i]->getNameEllipse1()==nRgb)
        {
            _rgb = rgb;
            _listOfEllipse[i]->recolorEllipse1(_rgb);
        }
    }
}

void RenderObject::saveEllipse(QString name, int i_se)
{
    if(_listOfEllipse.isEmpty())
    {
        QMessageBox::warning(0,"Предупреждение", "             Внимание!\n"
                                          "Нет объектов для сохранения!");
    }
    for(int i=0; i<_listOfEllipse.count(); i++)
    {
        if(_listOfEllipse[i]->getNameEllipse1()!="default_ellipse")//исправить
        {
//            QMessageBox::warning(0,"Предупреждение", "              Внимание!\n"
//                                              "Нет объектов для сохранения!!");
        }
        else
        {
            _listOfEllipse.last()->setNameEllipse1(name);
            _i_ce = i_se;
        }
    }

}

void RenderObject::deleteEllipse(int i_dde, QString dde)
{
    for(int i=0; i<_listOfEllipse.count(); i++)
    {
        if(_listOfEllipse[i]->getNameEllipse1()==dde)
        {
            delete _listOfEllipse[i];
            _listOfEllipse.removeAt(i);
            _i_ce = i_dde;
        }
    }
}

bool RenderObject::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    m_cameraMan->frameRenderingQueued(evt);
    return true;
}

void RenderObject::log(Ogre::String msg)
{
    if(Ogre::LogManager::getSingletonPtr() != NULL) Ogre::LogManager::getSingletonPtr()->logMessage(msg);
}

void RenderObject::log(QString msg)
{
    log(Ogre::String(msg.toStdString().c_str()));
}
