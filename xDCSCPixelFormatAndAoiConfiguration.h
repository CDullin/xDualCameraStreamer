#ifndef XDCSCPIXELFORMATANDAOICONFIGURATION_H
#define XDCSCPIXELFORMATANDAOICONFIGURATION_H

#include "xDCS_types.h"
#include <QObject>
#include <pylon/InstantCamera.h>
#ifdef PYLON_WIN_BUILD
#    include <pylon/PylonGUI.h>
#endif

// Namespace for using pylon objects.
using namespace Pylon;

class xDCSCamera;

class xDCSCPixelFormatAndAoiConfiguration : public QObject, public Pylon::CConfigurationEventHandler
{
Q_OBJECT
public:
    xDCSCPixelFormatAndAoiConfiguration(xDCSCamera*);
    virtual void OnOpened( Pylon::CInstantCamera& camera) override;
    virtual void OnGrabStarted( CInstantCamera& camera) override;
    virtual void OnCameraDeviceRemoved( CInstantCamera& camera) override;
    virtual void OnGrabStopped( CInstantCamera& camera) override;

signals:
    void KSignal(const SIG_TYPE&,void* data=nullptr);
    void CameraDeviceRemoved();
    void GrabbingStopped();
protected:
    xDCSCamera *pCamPropRef = nullptr;
};

#endif // XDCSCPIXELFORMATANDAOICONFIGURATION_H
