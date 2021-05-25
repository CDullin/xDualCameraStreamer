#include "xVColorDlgItem.h"
#include "xrcolorpickdlg.h"

#include <math.h>
#include <stdlib.h>
#include <QPainter>
#include <QResizeEvent>

using namespace std;

QPixmap xVColorDlgItem::generatePixmap()
{
    QPixmap pix(width(),height());
    QLinearGradient grd(QPointF(0,0),QPointF(width(),height()));
    QColor c1(min(255,_color.red()+50),min(255,_color.green()+50),min(255,_color.blue()+50));
    QColor c2(max(0,_color.red()-50),max(0,_color.green()-50),max(0,_color.blue()-50));
    grd.setColorAt(0,c1);
    grd.setColorAt(1,c2);
    QPainter pain(&pix);
    pain.setBrush(grd);
    pain.fillRect(QRectF(0,0,width(),height()),QBrush(grd));
    pain.end();
    return pix;
}

xVColorDlgItem::xVColorDlgItem(QWidget *parent):QLabel(parent)
{
    setGeometry(0,0,100,20);
    setAutoFillBackground(false);
    _color = QColor(Qt::black);
    setPixmap(generatePixmap());
    setScaledContents(true);
}

void xVColorDlgItem::setColor(const QColor& c)
{
    _color = c;
    setPixmap(generatePixmap());
}

void xVColorDlgItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    // open color dialog
    xrColorPickDlg dlg;
    dlg.setColor(_color);
    connect(&dlg,SIGNAL(colorPicked(const QColor&)),this,SLOT(setColor(const QColor&)));
    dlg.exec();
    emit modified();
}
