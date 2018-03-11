#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

//#include <QMainWindow>

/*class RenderObject : public QMainWindow
{
    Q_OBJECT
public:
    explicit RenderObject(QWidget *parent = 0);

signals:

public slots:
};*/

/*
Qt headers
*/
#include <QtWidgets/QApplication>
#include <QtGui/QKeyEvent>
#include <QtGui/QWindow>
#include <QList>
#include "MarkerSqr.h"
#include "PolyLine.h"
#include "ellipse.h"

/*
Ogre3D header
*/
#include <Ogre.h>

/*
Changed SdkCameraMan implementation to work with QKeyEvent, QMouseEvent, QWheelEvent
*/
#include "SdkQtCameraMan.h"

/*
With the headers included we now need to inherit from QWindow.
*/
class RenderObject : public QWindow, public Ogre::FrameListener
{
    /*
    A QWindow still inherits from QObject and can have signals/slots; we need to add the appropriate
    Q_OBJECT keyword so that Qt's intermediate compiler can do the necessary wireup between our class
    and the rest of Qt.
    */
    Q_OBJECT

public:
    explicit RenderObject(QWindow *parent = NULL);
    ~RenderObject();

    /*
    We declare these methods virtual to allow for further inheritance.
    */
    virtual void render(QPainter *painter);
    virtual void render();
    virtual void initialize();
    virtual void createScene();


    void setAnimating(bool animating);
    //void kvadrat(Ogre::Vector3 pos);
    //void line_sqr();
    void setCreateLine();
    void cancelCreateLine();
    void cancelModifyLine(QString cnl, int icnl);
    void addNewPoint(QString text, int anp);
    void saveLine( QString name );
    void saveModifyLine(QString name, int snl);
    void deleteLine( QString delpt );
    void removePoint(QString rpt, int i_rpt);
    void movePoint(QString mp, int i_mp);
    void selectPoint(QString sp, int i_sp);
    void createEllipse(int i_ce);
    void cancelCreateEllipse(int i_de);
    void resizeEllipseHoz(int sPos, QString nPos);
    void resizeEllipseVert(int sPos, QString nPos);
    void setEllipseColor(QColor rgb, QString nRgb);
    void saveEllipse(QString name , int i_se);
    void deleteEllipse(int i_dde, QString dde);

public slots:

    virtual void renderLater();
    virtual void renderNow();

    /*
    We use an event filter to be able to capture keyboard/mouse events. More on this later.
    */
    virtual bool eventFilter(QObject *target, QEvent *event);

signals:
    /*
    Event for clicking on an entity.
    */
    void entitySelected(Ogre::Entity* entity);

protected:
    /*
    Ogre3D pointers added here. Useful to have the pointers here for use by the window later.
    */
    Ogre::Root* m_ogreRoot;
    Ogre::RenderWindow* m_ogreWindow;
    Ogre::SceneManager* m_ogreSceneMgr;
    Ogre::Camera* m_ogreCamera;
    Ogre::ColourValue m_ogreBackground;
    OgreQtBites::SdkQtCameraMan* m_cameraMan;

    bool m_update_pending;
    bool m_animating;

    /*
    The below methods are what is actually fired when they keys on the keyboard are hit.
    Similar events are fired when the mouse is pressed or other events occur.
    */
    virtual void keyPressEvent(QKeyEvent * ev);
    virtual void keyReleaseEvent(QKeyEvent * ev);
    virtual void mouseMoveEvent(QMouseEvent* e);
    virtual void wheelEvent(QWheelEvent* e);
    virtual void mousePressEvent(QMouseEvent* em);
    virtual void mouseReleaseEvent(QMouseEvent* e);
    virtual void exposeEvent(QExposeEvent *event);
    virtual bool event(QEvent *event);


    /*
    FrameListener method
    */
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    /*
    Write log messages to Ogre log
    */
    void log(Ogre::String msg);
    void log(QString msg);
private:
    Ogre::SceneNode* node3;
    Ogre::ManualObject* pt;
    Ogre::SceneNode* node3_1;
    QList<Ogre::Vector3> _polyLine;
    QList<Ogre::ManualObject*> listOfSqr;
    MarkerSqr* m_sqr;
    QList<PolyLine*> _listOfPolyLine;
    QList<Ellipse1*> _listOfEllipse;
    int _cpl;
    QString _text;
    QString _mp;
    QString _rpt;
    QString _sp;
    int _i_rpt;
    int _i_sp;
    bool _s_c;
    int _horiz;
    int _sPos;
    int _i_ce;
    int _valHoz;
    int _valVert;
    QColor _rgb;
    //Ogre::ManualObject* kvadrat;
};

#endif // RENDEROBJECT_H
