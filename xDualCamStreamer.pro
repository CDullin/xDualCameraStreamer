QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    settings/xVBoolDlgItem.cpp \
    settings/xVColorDlgItem.cpp \
    settings/xVDoubleValueDlgItem.cpp \
    settings/xVFileImportDlgItem.cpp \
    settings/xVIntValueDlgItem.cpp \
    settings/xVSamplePosDlgItem.cpp \
    settings/xVVector3DDlgItem.cpp \
    settings/xrcolorpickdlg.cpp \
    settings/xvproptable.cpp \
    xDCSCPixelFormatAndAoiConfiguration.cpp \
    xDCSCamThread.cpp \
    xDCSCamera.cpp \
    xDCSDlg.cpp \
    xDCSDlg_display.cpp \
    xDCSDlg_io.cpp \
    xDCS_types.cpp

HEADERS += \
    settings/xVBoolDlgItem.h \
    settings/xVColorDlgItem.h \
    settings/xVCustomTableItems.h \
    settings/xVDoubleValueDlgItem.h \
    settings/xVFileImportDlgItem.h \
    settings/xVIntValueDlgItem.h \
    settings/xVSamplePosDlgItem.h \
    settings/xVVector3DDlgItem.h \
    settings/xrcolorpickdlg.h \
    settings/xvproptable.h \
    xDCSCPixelFormatAndAoiConfiguration.h \
    xDCSCamThread.h \
    xDCSCamera.h \
    xDCSDlg.h \
    xDCS_types.h

FORMS += \
    settings/xrcolorpickdlg.ui \
    xDCSDlg.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -L$$PWD/../../../../../opt/pylon/lib/ -lpylonbase
LIBS += -L$$PWD/../../../../../opt/pylon/lib/ -lGenApi_gcc_v3_1_Basler_pylon
LIBS += -L$$PWD/../../../../../opt/pylon/lib/ -lGCBase_gcc_v3_1_Basler_pylon
LIBS += -L$$PWD/../../../../../opt/pylon/lib/ -lpylonc
LIBS += -L$$PWD/../../../../../opt/pylon/lib/ -lpylonutility

INCLUDEPATH += $$PWD/../../../../../opt/pylon/include
INCLUDEPATH += $$PWD/../../../../../opt/pylon/include/GenApi
INCLUDEPATH += $$PWD/settings

DEPENDPATH += $$PWD/../../../../../opt/pylon/include/GenApi
DEPENDPATH += $$PWD/../../../../../opt/pylon/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../3rd_party/tiff-4.0.10/libtiff/release/ -ltiff
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../3rd_party/tiff-4.0.10/libtiff/debug/ -ltiff
else:unix: LIBS += -L$$PWD/../../3rd_party/tiff-4.0.10/libtiff/ -ltiff

INCLUDEPATH += $$PWD/../../3rd_party/tiff-4.0.10/libtiff
DEPENDPATH += $$PWD/../../3rd_party/tiff-4.0.10/libtiff
