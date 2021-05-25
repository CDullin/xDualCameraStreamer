#ifndef XDCSCAMTHREAD_H
#define XDCSCAMTHREAD_H

#include <QObject>
#include <QEvent>
#include <QThread>
#include "xDCS_types.h"

#include <pylon/InstantCamera.h>
#ifdef PYLON_WIN_BUILD
#    include <pylon/PylonGUI.h>
#endif

// Namespace for using pylon objects.
using namespace Pylon;
class xDCSCamera;

enum xDCSCameraThread_Event_Types
{
    CAMERA_FRAME = QEvent::User,
    CAMERA_AV_BRIGHTNESS = QEvent::User+1
};

class xDCSCameraThread_Event : public QEvent
{
public:
    xDCSCameraThread_Event(QImage *p):QEvent(QEvent::User)
    {
        pImg = p;
    }
    QImage *image(){return pImg;}
protected:
    QImage *pImg = NULL;
};

class xDCSCamThread:public QThread
{
    Q_OBJECT
public:
    xDCSCamThread(Pylon::CInstantCamera* pCam,xDCSCamera*);
    virtual void run() override;
    void stop();
signals:
    void update();
protected:
    bool _abb = false;
    Pylon::CInstantCamera *pCamera = nullptr;
    xDCSCamera* pCamPropObj = nullptr;
};

#endif // XDCSCAMTHREAD_H
