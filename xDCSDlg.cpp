#include "xDCSDlg.h"
#include "ui_xDCSDlg.h"
#include <QObject>
#include <QMessageBox>
#include <QTimer>
#include <sys/sysinfo.h>
#include <QDir>
#include <QTextStream>
#include "tiffio.h"
#include <pylon/PylonIncludes.h>

xDCSDlg::xDCSDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::xDCSDlg)
{

    ui->setupUi(this);
    connect(this,SIGNAL(KSignal(const SIG_TYPE&,void*)),this,SLOT(KSlot(const SIG_TYPE&,void*)));
    connect(&_camA,SIGNAL(KSignal(const SIG_TYPE&,void*)),this,SLOT(KSlot(const SIG_TYPE&,void*)));
    connect(&_camB,SIGNAL(KSignal(const SIG_TYPE&,void*)),this,SLOT(KSlot(const SIG_TYPE&,void*)));

    loadOrCreateSettings();

    ui->pPropTable->updateTable(&_settings);
    ui->pCamAGV->setScene(new QGraphicsScene());
    ui->pCamBGV->setScene(new QGraphicsScene());

    updateViews();
    // memory survailance
    QTimer *pMemoryTimer = new QTimer(this);
    pMemoryTimer->setInterval(3000);
    connect(pMemoryTimer,SIGNAL(timeout()),this,SLOT(dispMemoryConsumption()));
    pMemoryTimer->start();
    dispMemoryConsumption();
    ui->pMemProgBar->setFormat("free memory %v MB");

    // catch stop recording
    pStopTimer = new QTimer(this);
    pStopTimer->setInterval(500);
    pStopTimer->setSingleShot(true);
    connect(pStopTimer,SIGNAL(timeout()),this,SLOT(saveResults()));

    PylonInitialize();

}

void xDCSDlg::dispMemoryConsumption()
{
    struct sysinfo info;
    sysinfo(&info);
    ui->pMemProgBar->setMaximum(info.totalram/1000000);
    ui->pMemProgBar->setValue(info.freeram/1000000);

    float ratio = (double)info.freeram/(double)info.totalram;

    ui->pMemProgBar->setStyleSheet(this->styleSheet());
    if (ratio < 0.33)
    {
        if (ratio < 0.1)
            ui->pMemProgBar->setStyleSheet("background:red");
        else
            ui->pMemProgBar->setStyleSheet("background:yellow");
    }
}


void xDCSDlg::KSlot(const SIG_TYPE& _type,void* pData)
{
    QObject *pSObj=sender();
    switch (_type)
    {
    case ST_RECORDING_FINISHED:
    {
        if (_settings["auto export"]._value.toBool()) pStopTimer->start();
        ui->pStopTB->setEnabled(false);
        emit KSignal(ST_MSG,new QString("recording finished"));
    }
        break;
    case ST_SET_PROCESS_RANGE:
    {
        QPoint *pPnt=(QPoint*)pData;
        ui->pProgBar->setRange(pPnt->x(),pPnt->y());
        delete pPnt;
    }
        break;
    case ST_SET_PROCESS:
    {
        long *pVal=(long*)pData;
        ui->pProgBar->setValue(*pVal);
        delete pVal;
    }
        break;
    case ST_SET_PROCESS_TXT:
    {
        QString *pTxt=(QString*)pData;
        ui->pProgBar->setFormat(*pTxt);
        delete pTxt;
    }
        break;
    case ST_INC_PROCESS:
    {
        ui->pProgBar->setValue(ui->pProgBar->value()+1);
    }
        break;
    case ST_CAM_IMAGE:
        if (pSObj==&_camA) updateView(ui->pCamAGV,pCamAPixItem,&_camA);
        else updateView(ui->pCamBGV,pCamBPixItem,&_camB);
        break;
    case ST_MSG:
    {
        QString *pTxt=(QString*)pData;
        ui->pMSGBrwoser->setTextBackgroundColor(Qt::white);
        ui->pMSGBrwoser->setTextColor(Qt::black);
        ui->pMSGBrwoser->append(*pTxt);
        delete pTxt;
    }
        break;
    case ST_ERROR_MSG:
    {
        QString *pTxt=(QString*)pData;
        ui->pMSGBrwoser->setTextBackgroundColor(Qt::red);
        ui->pMSGBrwoser->setTextColor(Qt::yellow);
        ui->pMSGBrwoser->append(*pTxt);
        delete pTxt;
    }
        break;
    case ST_WARN_MSG:
    {
        QString *pTxt=(QString*)pData;
        ui->pMSGBrwoser->setTextBackgroundColor(Qt::yellow);
        ui->pMSGBrwoser->setTextColor(Qt::black);
        ui->pMSGBrwoser->append(*pTxt);
        delete pTxt;
    }
        break;
    default:
        // unhandled
        if (pSObj!=this)
            emit KSignal(_type,pData);
        break;
    }
}


void xDCSDlg::accept()
{
    if (QMessageBox::question(this,"Question","Do you really like to close the program?")==QMessageBox::Yes)
    {
        saveSettings();
        QDialog::accept();
    }
}

xDCSDlg::~xDCSDlg()
{
    delete ui;
}

void xDCSDlg::on_pConnectTB_clicked()
{
    emit KSignal(ST_MSG,new QString("try to connect to cameras"));

    CTlFactory& TlFactory = CTlFactory::GetInstance();
    DeviceInfoList_t lstDevices;
    TlFactory.EnumerateDevices( lstDevices );

    if (lstDevices.size()<1 || !_camA.ConnectToPylonDevice(lstDevices[0]))
        emit KSignal(ST_ERROR_MSG,new QString("couldn't connect camera A"));
    if (lstDevices.size()<2 || !_camB.ConnectToPylonDevice(lstDevices[1]))
        emit KSignal(ST_ERROR_MSG,new QString("couldn't connect camera B"));
    updateViews();
}


void xDCSDlg::on_pLiveTB_toggled(bool checked)
{
    if (checked)
    {
        if (_camA.isRecording() || _camB.isRecording())
        {
            emit KSignal(ST_MSG,new QString("cannot switch to live mode. camera is recording"));
        }
        else
        {
            emit KSignal(ST_MSG,new QString("switching to live mode"));
            _camA.startStopLive(checked);
            _camB.startStopLive(checked);
        }
    }
    else
    {
        emit KSignal(ST_MSG,new QString("stopping live mode"));
        _camA.startStopLive(checked);
        _camB.startStopLive(checked);
    }
}


void xDCSDlg::on_pRecordTB_clicked()
{    
    emit KSignal(ST_MSG,new QString("start recording"));
    long frameNr = 2 * _settings["frame rate [fps]"]._value.value<xLimitedInt>()._value * _settings["total acquisition time [s]"]._value.value<xLimitedDouble>()._value;
    emit KSignal(ST_SET_PROCESS_RANGE,new QPoint(0,frameNr-1));
    emit KSignal(ST_SET_PROCESS_TXT,new QString("%p% frames recorded"));
    emit KSignal(ST_SET_PROCESS,new long(0));
    _camA.startRecording();
    _camB.startRecording();
    updateViews();
}

void xDCSDlg::on_pStopTB_clicked()
{
    emit KSignal(ST_MSG,new QString("stopping recording"));
    _camA.stopRecording();
    _camB.stopRecording();
    updateViews();
}


void xDCSDlg::on_pExportTB_clicked()
{
    saveResults();
}

