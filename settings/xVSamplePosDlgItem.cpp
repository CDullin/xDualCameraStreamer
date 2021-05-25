#include "xVSamplePosDlgItem.h"

#include <QLabel>
#include <QResizeEvent>

xVSamplePosDlgItem::xVSamplePosDlgItem(QWidget* parent):QWidget(parent)
{
    pBoxLayout = new QHBoxLayout(this);
    pBoxLayout->setMargin(0);
    pSBX = new QSpinBox(this);
    pSBX->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
    pSBX->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    pBoxLayout->addWidget(pSBX);
    pBoxLayout->addWidget(new QLabel("x"));
    pSBY = new QSpinBox(this);
    pSBY->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
    pSBY->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    pBoxLayout->addWidget(pSBY);
    pBoxLayout->addWidget(new QLabel("x"));
    pSBZ = new QSpinBox(this);
    pSBZ->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
    pSBZ->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    pBoxLayout->addWidget(pSBZ);

    connect(pSBX,SIGNAL(valueChanged(int)),this,SLOT(posChanged()));
    connect(pSBY,SIGNAL(valueChanged(int)),this,SLOT(posChanged()));
    connect(pSBZ,SIGNAL(valueChanged(int)),this,SLOT(posChanged()));
}

void xVSamplePosDlgItem::posChanged()
{
    _pos.x = pSBX->value();
    _pos.y = pSBY->value();
    _pos.z = pSBZ->value();

    emit modified();
}

void xVSamplePosDlgItem::setPos(const x3D_SAMPLE_POS &p)
{
    _pos=p;
    pSBX->setValue(p.x);
    pSBY->setValue(p.y);
    pSBZ->setValue(p.z);
}

void xVSamplePosDlgItem::setRange(const int& lb,const int& hb,const int axis)
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
/*
void xVSamplePosDlgItem::resizeEvent(QResizeEvent *event)
{
    QSize size=event->size();
    setGeometry(0,0,size.width(),size.height());
    if (pBoxLayout) pBoxLayout->setGeometry(QRect(0,0,size.width(),size.height()));
};
*/
