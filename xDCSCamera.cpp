#include "xDCSCamera.h"
#include "xDCSCamThread.h"
#include "xDCSCPixelFormatAndAoiConfiguration.h"
#include <pylon/PylonIncludes.h>
#include <GenApi.h>
#include <pylon/InstantCamera.h>
#include <QCoreApplication>
#include <QMutex>
#include <math.h>
#include <stdlib.h>

using namespace std;

QMutex mutex;

xDCSCamera::xDCSCamera(QObject *parent) : QObject(parent)
{
    installEventFilter(this);
}

bool xDCSCamera::eventFilter(QObject* watched,QEvent* event)
{
    if (watched==this && event->type()==CAMERA_FRAME)
    {
        xDCSCameraThread_Event *pCamThreadEvent = dynamic_cast<xDCSCameraThread_Event*>(event);
        if (pCamThreadEvent)
        {
            QTime _currentTime = QTime::currentTime();
            pCurrentFrame = pCamThreadEvent->image();

            if (!_recording)
            {
                _measuredFps = 1.0/((float)_timeForLastFrame.msecsTo(_currentTime)/1000.0f);
                emit KSignal(ST_CAM_IMAGE,0);
                _timeForLastFrame = _currentTime;
            }
            else
            {
                if (_imagesToRecord<=0) stopRecording();
                else
                {
                    QImage *pImg = new QImage(pCurrentFrame->copy());
                    _recordBuffer.append(pImg);
                    _recoredTimes.append(QTime::currentTime().msecsSinceStartOfDay());
                    _imagesToRecord--;
                    emit KSignal(ST_INC_PROCESS,0);
                }
            }
        }
    }
}

void xDCSCamera::stopRecording()
{
    if (pCamThread && pCamThread->isRunning())
    {
        pCamThread->stop();
        while (pCamThread->isRunning())
            QThread::msleep(5);
    }
    _recording = false;
    // save
    emit KSignal(ST_RECORDING_FINISHED,0);
}

void xDCSCamera::startRecording()
{
    if (!pCamera) return;
    // Stop Camera
    if (pCamThread && pCamThread->isRunning())
    {
        pCamThread->stop();
        while (pCamThread->isRunning())
            QThread::msleep(5);
    }
    _imagesToRecord = _settings["frame rate [fps]"]._value.value<xLimitedInt>()._value * _settings["total acquisition time [s]"]._value.value<xLimitedDouble>()._value;
    _recording = true;
    _recordBuffer.clear();
    _recoredTimes.clear();
    pCamThread->start();
}

void xDCSCamera::startStopLive(bool b)
{
    if (!pCamThread) return;
    b ? pCamThread->start() : pCamThread->stop();
}

bool xDCSCamera::ConnectToPylonDevice(CDeviceInfo info)
{
    if (_connected) return true;
    PylonInitialize();
    try {
        pCamera=new CInstantCamera ();
        IPylonDevice *pDevice = CTlFactory::GetInstance().CreateDevice(info);
        pCamera->Attach(pDevice ,Cleanup_Delete);

        pCamConfig = new xDCSCPixelFormatAndAoiConfiguration(this);
        connect(pCamConfig,SIGNAL(KSignal(const SIG_TYPE&,void*)),this,SIGNAL(KSignal(const SIG_TYPE&,void*)));
        connect(pCamConfig,SIGNAL(CameraDeviceRemoved()),this,SLOT(CameraDeviceRemoved()));
        connect(pCamConfig,SIGNAL(GrabbingStopped()),this,SIGNAL(GrabbingStopped()));
        // create vtk objects
        pCamera->RegisterConfiguration( pCamConfig, RegistrationMode_Append, Cleanup_Delete);

        _connected = pCamera->IsPylonDeviceAttached();
        if (_connected) {
            pCamera->Open();
            pCamThread = new xDCSCamThread(pCamera,this);
        }
    }
    catch (...)
    {

    }
    return _connected;
}

void xDCSCamera::CameraDeviceRemoved()
{
    emit KSignal(ST_ERROR_MSG,new QString("camera device removed"));
    _connected=false;
    if (pCamThread) delete pCamThread;pCamThread=nullptr;
    if (pCamera) delete pCamera;pCamera=nullptr;
}

void xDCSCamera::KSlot(const SIG_TYPE& _type,void *pData)
{
    switch (_type)
    {
    default:
        // handled
        break;
    }
}

void xDCSCamera::retrieveValuesAndLimitsFromCamera()
{
    if (pCamera)
    {
        // read all current parameter
        using namespace GenApi;
        // Get the camera control object.
        INodeMap &control = pCamera->GetNodeMap();
        // Get the parameters for setting the image area of interest (Image AOI).
        const CIntegerPtr width = control.GetNode("Width");
        const CIntegerPtr height = control.GetNode("Height");
        const CBooleanPtr centerX = control.GetNode("CenterX");
        const CBooleanPtr centerY = control.GetNode("CenterY");
        const CFloatPtr gain = control.GetNode("Gain");
        const CFloatPtr exposureTime = control.GetNode("ExposureTime");
        const CBooleanPtr setFrameRate = control.GetNode("AcquisitionFrameRateEnable");
        const CFloatPtr aquFrameRate = control.GetNode("AcquisitionFrameRateAbs");
        CEnumerationPtr(control.GetNode("ExposureAuto"))->FromString("Off");
        CEnumerationPtr gainAuto(control.GetNode("GainAuto"));
        const CIntegerPtr binningX = control.GetNode("BinningHorizontal");
        const CIntegerPtr binningY = control.GetNode("BinningVertical");

        _settings["running"]._value = pCamThread && pCamThread->isRunning();

        try {
            _settings["matrix width"]._enabled = IsWritable(width);
            _settings["matrix width"]._value = QVariant::fromValue(xLimitedInt(width->GetValue(),width->GetMin(),width->GetMax()));
        }  catch (...) { }
        try {
            _settings["matrix height"]._enabled = IsWritable(height);
            _settings["matrix height"]._value = QVariant::fromValue(xLimitedInt(height->GetValue(),height->GetMin(),height->GetMax()));
        }  catch (...) { }
        try {
            _settings["horizontal binning"]._enabled = IsWritable(binningX);
            _settings["horizontal binning"]._value = QVariant::fromValue(xLimitedInt(binningX->GetValue(),binningX->GetMin(),binningX->GetMax()));
        }  catch (...) { }
        try {
            _settings["vertical binning"]._enabled = IsWritable(binningY);
            _settings["vertical binning"]._value = QVariant::fromValue(xLimitedInt(binningY->GetValue(),binningY->GetMin(),binningY->GetMax()));
        }  catch (...) { }
        try {
            _settings["exposure time [ms]"]._enabled = IsWritable(exposureTime);
            _settings["exposure time [ms]"]._value = QVariant::fromValue(xLimitedInt(exposureTime->GetValue()/1000,exposureTime->GetMin()/1000,exposureTime->GetMax()/1000));
        }  catch (...) { }
        try {
            _settings["frame rate [fps]"]._enabled = IsWritable(gain);
            _settings["frame rate [fps]"]._value = QVariant::fromValue(xLimitedInt(gain->GetValue(),gain->GetMin(),gain->GetMax()));
        }  catch (...) { }
        try {
            _settings["gain [db]"]._enabled = IsWritable(aquFrameRate);
            _settings["gain [db]"]._value = QVariant::fromValue(xLimitedInt(aquFrameRate->GetValue(),aquFrameRate->GetMin(),aquFrameRate->GetMax()));
        }  catch (...) { }
    }
}
