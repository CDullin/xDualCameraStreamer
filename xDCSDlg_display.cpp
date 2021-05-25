#include "xDCSDlg.h"
#include "ui_xDCSDlg.h"

void xDCSDlg::updateViews()
{
    updateView(ui->pCamAGV,pCamAPixItem,&_camA);
    updateView(ui->pCamBGV,pCamBPixItem,&_camB);
}

void xDCSDlg::updateView(QGraphicsView* pGV,QGraphicsPixmapItem *pPixItem, xDCSCamera *pCam)
{
    if (!pGV) return;
    if (!pPixItem)
    {
        pPixItem = new QGraphicsPixmapItem();
        pGV->scene()->addItem(pPixItem);
    }

    QString _errorTxt="";

    if (!pCam->camera() || !pCam->camera()->IsPylonDeviceAttached() || !pCam->camera()->IsOpen())
    {
        _errorTxt="not connected";
    }
    else
    {
        if (pCam->isRecording())
            _errorTxt = "recording ...";
        else
        {
            if (pCam->currentFrame())
            {
                QPixmap pix = QPixmap::fromImage(*pCam->currentFrame());
                QPainter pain(&pix);
                pain.setPen(Qt::white);
                pain.drawText(10,20,QString("%1fps").arg(pCam->measuredFps(),0,'f',1));
                pain.end();
                pPixItem->setPixmap(pix);
            }
            else
                _errorTxt="not running";
        }
    }

    if (!_errorTxt.isEmpty())
    {
        QPixmap pix(300,200);
        pix.fill(Qt::white);
        QFont f=font();
        f.setPixelSize(20);
        QPainter pain(&pix);
        pain.setFont(f);
        pain.drawText(QRectF(0,0,300,200),Qt::AlignCenter,_errorTxt);
        pain.end();
        pPixItem->setPixmap(pix);
        pPixItem->update();
    }
}
