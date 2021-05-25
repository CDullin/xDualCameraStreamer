#ifndef XDCSCAMERA_H
#define XDCSCAMERA_H

#include <QObject>
#include <QEvent>
#include <QThread>
#include "xDCS_types.h"
#include "xDCSCamThread.h"
#include "xDCSCPixelFormatAndAoiConfiguration.h"

#include <pylon/InstantCamera.h>
#ifdef PYLON_WIN_BUILD
#    include <pylon/PylonGUI.h>
#endif

// Namespace for using pylon objects.
using namespace Pylon;

class xDCSCamera : public QObject
{
    Q_OBJECT
public:
    xDCSCamera(QObject *parent = nullptr);
    bool ConnectToPylonDevice(CDeviceInfo);
    CInstantCamera *camera(){return pCamera;}
    bool eventFilter(QObject*,QEvent*) override;
    QImage* currentFrame(){return pCurrentFrame;}
    void startStopLive(bool);
    float measuredFps(){return _measuredFps;}
    bool isRecording(){return _recording;}
    void startRecording();
    void stopRecording();
    QVector <long>* recordedTimes(){return &_recoredTimes;}
    QVector <QImage*>* recordedImages(){return &_recordBuffer;}

public slots:
    void KSlot(const SIG_TYPE&,void*);

signals:
    void KSignal(const SIG_TYPE&,void*);


protected slots:
    void CameraDeviceRemoved();

protected:
    CInstantCamera *pCamera = nullptr;
    void retrieveValuesAndLimitsFromCamera();
    xDCSCPixelFormatAndAoiConfiguration *pCamConfig = nullptr;
    xDCSCamThread *pCamThread = nullptr;
    QImage *pCurrentFrame = nullptr;
    QTime _timeForLastFrame;
    float _measuredFps;
    bool _recording = false;
    long _imagesToRecord = 0;
    bool _connected = false;
    QVector <QImage*> _recordBuffer;
    QVector <long> _recoredTimes;      // ms to 0:00 of the currentDay
};

#endif // XDCSCAMERA_H
