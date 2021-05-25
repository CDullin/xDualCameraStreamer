#ifndef XDCSDLG_H
#define XDCSDLG_H

#include <QDialog>
#include "xDCS_types.h"
#include "xDCSCamera.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsView>

QT_BEGIN_NAMESPACE
namespace Ui { class xDCSDlg; }
QT_END_NAMESPACE

class xDCSDlg : public QDialog
{
    Q_OBJECT

public:
    xDCSDlg(QWidget *parent = nullptr);
    ~xDCSDlg();

    virtual void accept() override;

public slots:
    void KSlot(const SIG_TYPE&,void*);

private slots:
    void on_pConnectTB_clicked();
    void on_pLiveTB_toggled(bool checked);
    void on_pRecordTB_clicked();
    void on_pStopTB_clicked();
    void dispMemoryConsumption();
    void saveResults();

    void on_pExportTB_clicked();

protected:
    void saveSettings();
    void loadOrCreateSettings();
    void updateViews();
    void updateView(QGraphicsView*,QGraphicsPixmapItem*,xDCSCamera*);
    void saveMultiFrameTIFF(QVector<QImage*> *images,QString fname);

signals:
    void KSignal(const SIG_TYPE&,void*);

private:
    Ui::xDCSDlg *ui;
    xDCSCamera _camA,_camB;
    QGraphicsPixmapItem* pCamAPixItem = nullptr;
    QGraphicsPixmapItem* pCamBPixItem = nullptr;
    QTimer *pStopTimer;
};
#endif // XDCSDLG_H
