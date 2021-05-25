#include "xVVector3DDlgItem.h"
#include "xVSamplePosDlgItem.h"

#include <QLabel>
#include <QResizeEvent>

xVVector3DDlgItem::xVVector3DDlgItem(QWidget* parent):QWidget(parent)
{
    pBoxLayout = new QHBoxLayout(this);
    pBoxLayout->setMargin(0);
    pSBX = new QDoubleSpinBox(this);
    pSBX->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
    pSBX->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    pBoxLayout->addWidget(pSBX);
    pBoxLayout->addWidget(new QLabel("x"));
    pSBY = new QDoubleSpinBox(this);
    pSBY->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
    pSBY->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    pBoxLayout->addWidget(pSBY);
    pBoxLayout->addWidget(new QLabel("x"));
    pSBZ = new QDoubleSpinBox(this);
    pSBZ->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
    pSBZ->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    pBoxLayout->addWidget(pSBZ);

    connect(pSBX,SIGNAL(valueChanged(double)),this,SLOT(vecChanged()));
    connect(pSBY,SIGNAL(valueChanged(double)),this,SLOT(vecChanged()));
    connect(pSBZ,SIGNAL(valueChanged(double)),this,SLOT(vecChanged()));
}

void xVVector3DDlgItem::vecChanged()
{
    _vector.setX(pSBX->value());
    _vector.setY(pSBY->value());
    _vector.setZ(pSBZ->value());
    emit modified();
}

void xVVector3DDlgItem::setRange(const double& lb,const double& hb,const int axis)
{
    if (axis & xVSamplePosDlgItem::X_AXIS)
    {
        _lowLimit[0]=lb;
        _highLimit[0]=hb;
        pSBX->setRange(lb,hb);
    }
    if (axis & xVSamplePosDlgItem::Y_AXIS)
    {
        _lowLimit[1]=lb;
        _highLimit[1]=hb;
        pSBY->setRange(lb,hb);
    }
    if (axis & xVSamplePosDlgItem::Z_AXIS)
    {
        _lowLimit[2]=lb;
        _highLimit[2]=hb;
        pSBZ->setRange(lb,hb);
    }
}
void xVVector3DDlgItem::setVector(const QVector3D& v)
{
    _vector=v;
    pSBX->setValue(v.x());
    pSBY->setValue(v.y());
    pSBZ->setValue(v.z());
}
/*
void xVVector3DDlgItem::resizeEvent(QResizeEvent *event)
{
    QSize size=event->size();
    setGeometry(0,0,size.width(),size.height());
    if (pBoxLayout) pBoxLayout->setGeometry(QRect(0,0,size.width(),size.height()));
}
*/
