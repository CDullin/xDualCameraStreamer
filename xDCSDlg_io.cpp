#include "xDCSDlg.h"
#include "ui_xDCSDlg.h"
#include "xDCS_types.h"
#include <QMessageBox>
#include <QTimer>
#include <sys/sysinfo.h>
#include <QDir>
#include <QTextStream>
#include "tiffio.h"
#include <pylon/PylonIncludes.h>

void xDCSDlg::saveSettings()
{
    QFile f("xDCS_settings.dat");
    if (f.open(QFile::WriteOnly))
    {
        QDataStream d(&f);
        d << _settings;
        f.close();
    }
}

void xDCSDlg::loadOrCreateSettings()
{
    QFile f("xDCS_settings.dat");
    if (f.open(QFile::ReadOnly))
    {
        emit KSignal(ST_MSG,new QString("Config-file was found. Retrieving stored settings ..."));

        QDataStream d(&f);
        d >> _settings;
        f.close();
    }
    else
    {
        emit KSignal(ST_WARN_MSG,new QString("No config-file was found. Default settings being used."));

        _settings["total acquisition time [s]"]._id=1;
        _settings["total acquisition time [s]"]._value=QVariant::fromValue(xLimitedDouble(30,1,150,10));
        _settings["export path"]._id=2;
        _settings["export path"]._value=QVariant::fromValue(xFileName(xFileName::FN_OUTPUT_DIR,""));
        _settings["meta data"]._id=3;
        _settings["meta data"]._value=QString("");
        _settings["frame rate [fps]"]._id=4;
        _settings["frame rate [fps]"]._value=QVariant::fromValue(xLimitedInt(100,1,150));
        _settings["matrix width"]._id = 5;
        _settings["matrix width"]._value = QVariant::fromValue(xLimitedInt(640,320,1920));
        _settings["matrix height"]._id = 6;
        _settings["matrix height"]._value = QVariant::fromValue(xLimitedInt(480,200,1200));
        _settings["exposure time [ms]"]._id = 7;
        _settings["exposure time [ms]"]._value = QVariant::fromValue(xLimitedInt(10,5,1024));
        _settings["gain [db]"]._id = 8;
        _settings["gain [db]"]._value = QVariant::fromValue(xLimitedInt(3,0,1024));
        _settings["horizontal binning"]._id = 9;
        _settings["horizontal binning"]._value = QVariant::fromValue(xLimitedInt(2,1,10));
        _settings["vertical binning"]._id = 10;
        _settings["vertical binning"]._value = QVariant::fromValue(xLimitedInt(2,1,10));
    }
}

void xDCSDlg::saveMultiFrameTIFF(QVector<QImage*> *images,QString _fname)
{
    if (images->count()==0) return;

    // continue here
    QFileInfo info(_fname);
    const char* _fileName = _fname.toUtf8().constData();
    TIFF *tif = TIFFOpen(_fileName,"w");
    uint32 imageWidth   = images->at(0)->width();
    uint32 imageLength  = images->at(0)->height();
    uint32 bitsPerPixel = 8;
    uint32 row;

    emit KSignal(ST_SET_PROCESS_RANGE,new QPoint(1,images->count()));
    emit KSignal(ST_SET_PROCESS_TXT,new QString("saving images %v"));

    if (tif)
    {
        long j=0;
        for (long i=0;i<images->count();++i)
        {
            emit KSignal(ST_SET_PROCESS,new long(i+1));

            TIFFSetField(tif, TIFFTAG_PAGENUMBER, i, i);
            TIFFSetField(tif, TIFFTAG_SUBFILETYPE, FILETYPE_PAGE);
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH,imageWidth);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH,imageLength);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE,bitsPerPixel);
            TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL,1);
            TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
            TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
            TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, imageLength);

            for (row = 0; row < imageLength; row++)
                TIFFWriteScanline(tif, images->at(i)->scanLine(row), row, 0);

            TIFFWriteDirectory(tif);
        }

        TIFFClose(tif);
    }
}


void xDCSDlg::saveResults()
{
    QDir d(_settings["export path"]._value.value<xFileName>()._fileName);
    d.mkpath(_settings["export path"]._value.value<xFileName>()._fileName);

    if (!d.exists())
    {
        emit KSignal(ST_WARN_MSG,new QString("No valid export path found. Please specify and start export again!"));
        return;
    }

    if ((!_camA.recordedImages() || _camA.recordedImages()->count()==0) &&
         (!_camB.recordedImages() || _camB.recordedImages()->count()==0))
    {
        emit KSignal(ST_ERROR_MSG,new QString("Non of the cameras contain stored images. Export aborted."));
        return;
    }

    // protocol file
    QFile f(_settings["export path"]._value.value<xFileName>()._fileName+"/protocol.txt");
    if (f.open(QFile::WriteOnly))
    {
        QTextStream t(&f);

        t << "Dual Camera Streaming " << QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss:zzz") << endl;
        t << "meta data = " << _settings["meta data"]._value.toString() << endl;
        t << "total acquisition time [s] = " << _settings["total acquisition time [s]"]._value.value<xLimitedDouble>()._value << endl;
        t << "frame rate [fps] = " << _settings["frame rate [fps]"]._value.value<xLimitedInt>()._value << endl;
        t << "matrix width = " << _settings["matrix width"]._value.value<xLimitedInt>()._value << endl;
        t << "matrix height = " << _settings["matrix height"]._value.value<xLimitedInt>()._value << endl;
        t << "exposure time [ms] = " << _settings["exposure time [ms]"]._value.value<xLimitedInt>()._value << endl;
        t << "gain [db] = " << _settings["gain [db]"]._value.value<xLimitedInt>()._value << endl;
        t << "horizontal binning = " << _settings["horizontal binning"]._value.value<xLimitedInt>()._value << endl;
        t << "vertical binning = " << _settings["vertical binning"]._value.value<xLimitedInt>()._value << endl;

        f.close();
    }

    // camera A time file
    f.setFileName(_settings["export path"]._value.value<xFileName>()._fileName+"/cameraA_timing.csv");
    if (f.open(QFile::WriteOnly))
    {
        QTextStream t(&f);

        for (long j=0;j<_camA.recordedTimes()->count();++j)
            t << _camA.recordedTimes()->at(j) << endl;

        f.close();
    }

    // save images as multiframe TIFF
    saveMultiFrameTIFF(_camA.recordedImages(),_settings["export path"]._value.value<xFileName>()._fileName+"/cameraA.TIF");

    // camera B time file
    f.setFileName(_settings["export path"]._value.value<xFileName>()._fileName+"/cameraB_timing.csv");
    if (f.open(QFile::WriteOnly))
    {
        QTextStream t(&f);

        for (long j=0;j<_camB.recordedTimes()->count();++j)
            t << _camB.recordedTimes()->at(j) << endl;

        f.close();
    }

    // save images as multiframe TIFF
    saveMultiFrameTIFF(_camB.recordedImages(),_settings["export path"]._value.value<xFileName>()._fileName+"/cameraB.TIF");

    QMessageBox::information(this,"Info","Export finished");
}
