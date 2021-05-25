#include "xDCSCPixelFormatAndAoiConfiguration.h"

xDCSCPixelFormatAndAoiConfiguration::xDCSCPixelFormatAndAoiConfiguration(xDCSCamera* p):QObject(),Pylon::CConfigurationEventHandler()
{
    pCamPropRef = p;
}
void xDCSCPixelFormatAndAoiConfiguration::OnGrabStarted( CInstantCamera& camera)
{
}
void xDCSCPixelFormatAndAoiConfiguration::OnCameraDeviceRemoved( CInstantCamera& camera)
{
    emit CameraDeviceRemoved();
}
void xDCSCPixelFormatAndAoiConfiguration::OnGrabStopped( CInstantCamera& camera)
{
    emit GrabbingStopped();
}

void xDCSCPixelFormatAndAoiConfiguration::OnOpened( Pylon::CInstantCamera& camera)
{
    // Allow all the names in the namespace GenApi to be used without qualification.
    using namespace GenApi;
    // Get the camera control object.
    INodeMap &control = camera.GetNodeMap();
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

    // Maximize the Image AOI.
    try {
        if (IsWritable(centerX)) centerX->SetValue(true);
        if (IsWritable(centerY)) centerY->SetValue(true);
    }
    catch (const Pylon::GenericException& e) {
        emit KSignal(ST_ERROR_MSG,new QString(QString("Could not apply configuration. const GenericException caught in OnOpened method msg=")+QString(e.what())));
    }
    try {
        if (IsWritable(exposureTime)) exposureTime->SetValue(_settings["exposure time [ms]"]._value.value<xLimitedInt>()._value*1000.0f);
    }
    catch (const Pylon::GenericException& e) {
        emit KSignal(ST_ERROR_MSG,new QString(QString("Could not apply configuration. const GenericException caught in OnOpened method msg=")+QString(e.what())));
    }

    try {
        // check if in continous mode
        if (IsWritable(setFrameRate)) setFrameRate->SetValue(true);
        if (IsWritable(aquFrameRate)) aquFrameRate->SetValue(_settings["frame rate [fps]"]._value.value<xLimitedInt>()._value);
    }
    catch (const Pylon::GenericException& e) {
        emit KSignal(ST_ERROR_MSG,new QString(QString("Could not apply configuration. const GenericException caught in OnOpened method msg=")+QString(e.what())));
    }

    try {
        width->SetValue(_settings["matrix width"]._value.value<xLimitedInt>()._value);
        height->SetValue(_settings["matrix height"]._value.value<xLimitedInt>()._value);
    }
    catch (const Pylon::GenericException& e) {
        emit KSignal(ST_ERROR_MSG,new QString(QString("Could not apply configuration. const GenericException caught in OnOpened method msg=")+QString(e.what())));
    }

    try {
        gain->SetValue(_settings["gain [db]"]._value.value<xLimitedInt>()._value);
    }
    catch (const Pylon::GenericException& e) {
        emit KSignal(ST_ERROR_MSG,new QString(QString("Could not apply configuration. const GenericException caught in OnOpened method msg=")+QString(e.what())));
    }
    try {
        binningX->SetValue(_settings["horizontal binning"]._value.value<xLimitedInt>()._value);
    }
    catch (const Pylon::GenericException& e) {
        emit KSignal(ST_ERROR_MSG,new QString(QString("Could not apply configuration. const GenericException caught in OnOpened method msg=")+QString(e.what())));
    }
    try {
        binningY->SetValue(_settings["vertical binning"]._value.value<xLimitedInt>()._value);
    }
    catch (const Pylon::GenericException& e) {
        emit KSignal(ST_ERROR_MSG,new QString(QString("Could not apply configuration. const GenericException caught in OnOpened method msg=")+QString(e.what())));
    }
    // vertical binning
    // image mode
    CEnumerationPtr(control.GetNode("PixelFormat"))->FromString("Mono8");
}
