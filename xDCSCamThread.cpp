#include "xDCSCamThread.h"
#include "xDCSCamera.h"
#include <QCoreApplication>
#include <pylon/PylonIncludes.h>
#include <GenApi.h>
#include <pylon/InstantCamera.h>

xDCSCamThread::xDCSCamThread(Pylon::CInstantCamera* pCam,xDCSCamera* pCPO):QThread()
{
    pCamera=pCam;
    pCamPropObj=pCPO;
}

void xDCSCamThread::run()
{
    _abb = false;
    pCamera->StartGrabbing(GrabStrategy_LatestImages);
    while (!_abb && pCamera)
    {
        CGrabResultPtr ptrGrabResult;
        CPylonImage iimg;
        try {

            pCamera->RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException);

            // Image grabbed successfully?
            if (ptrGrabResult->GrabSucceeded())
            {
                iimg.AttachGrabResultBuffer(ptrGrabResult);
                QImage *img=new QImage((uchar*)iimg.GetBuffer(),iimg.GetWidth(),iimg.GetHeight(),QImage::Format_Grayscale8);
                qApp->postEvent(pCamPropObj,new xDCSCameraThread_Event(img));

            }
            else
            {
                //dlg->emit MSG(QString("Error: %1 %2").arg(ptrGrabResult->GetErrorCode()).arg((const char*)ptrGrabResult->GetErrorDescription()));
            }
        }
        catch (...) {

        }
    }
}
void xDCSCamThread::stop()
{
    _abb = true;
    if (pCamera) pCamera->StopGrabbing();
}

