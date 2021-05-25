#include "xVIntValueDlgItem.h"
#include <QResizeEvent>
#include <QHBoxLayout>

xVIntValueDlgItem::xVIntValueDlgItem(QWidget *parent):QWidget(parent)
{
    setGeometry(0,0,100,20);
    pSlider = new QSlider(this);
    pSlider->setGeometry(0,0,40,20);
    pSlider->setOrientation(Qt::Horizontal);
    pSB=new QSpinBox(this);
    pSB->setGeometry(40,0,60,20);

    connect(pSlider,SIGNAL(valueChanged(int)),this,SLOT(setValue(int)));
    connect(pSB,SIGNAL(valueChanged(int)),this,SLOT(setValue(int)));

    QHBoxLayout *pVBox=new QHBoxLayout(this);
    pVBox->setContentsMargins(10,0,0,0);
    pVBox->addWidget(pSlider);
    pVBox->addWidget(pSB);
}

void xVIntValueDlgItem::setValue(const int& v)
{
    QSlider *pCurrentSlider=dynamic_cast<QSlider*>(sender());
    QSpinBox *pCurrentSB=dynamic_cast<QSpinBox*>(sender());

    _value = v;
    if (pCurrentSlider!=pSlider) pSlider->setValue(v);
    if (pCurrentSB!=pSB) pSB->setValue(v);
    emit modified();
}

void xVIntValueDlgItem::setRange(const int& lb,const int& hb)
{
    pSlider->setRange(lb,hb);
    pSB->setRange(lb,hb);
    _lowLimit=lb;
    _highLimit=hb;
}

/*
void xVIntValueDlgItem::resizeEvent(QResizeEvent *event)
{
    QSize size=event->size();
    setGeometry(0,0,size.width(),size.height());
    pSlider->setGeometry(0,0,size.width()-60,size.height());
    pSB->setGeometry(size.width()-60,0,60,size.height());
}
*/
