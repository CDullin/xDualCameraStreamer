#include "xVDoubleValueDlgItem.h"
#include <QResizeEvent>
#include <QHBoxLayout>

xVDoubleValueDlgItem::xVDoubleValueDlgItem(QWidget *parent):QWidget(parent)
{
    setGeometry(0,0,100,20);
    pSlider = new QSlider(this);
    pSlider->setGeometry(0,0,40,20);
    pSlider->setOrientation(Qt::Horizontal);
    pSB=new QDoubleSpinBox(this);
    pSB->setGeometry(40,0,60,20);

    connect(pSlider,SIGNAL(valueChanged(int)),this,SLOT(setValue(int)));
    connect(pSB,SIGNAL(valueChanged(double)),this,SLOT(setValue(double)));

    QHBoxLayout *pVBox=new QHBoxLayout(this);
    pVBox->setContentsMargins(10,0,0,0);
    pVBox->addWidget(pSlider);
    pVBox->addWidget(pSB);
}

void xVDoubleValueDlgItem::setValue(const double& v)
{
    _value = v;
    QSlider *pCurrentSlider=dynamic_cast<QSlider*>(sender());
    QDoubleSpinBox *pCurrentSB=dynamic_cast<QDoubleSpinBox*>(sender());

    if (pCurrentSlider==pSlider) _value/=_sliderReduction;

    if (pCurrentSlider!=pSlider) pSlider->setValue(_value*_sliderReduction);
    if (pCurrentSB!=pSB) pSB->setValue(_value);
    emit modified();
}

void xVDoubleValueDlgItem::setValue(int v)
{
    _value = v;
    QSlider *pCurrentSlider=dynamic_cast<QSlider*>(sender());
    if (pCurrentSlider==pSlider) _value/=_sliderReduction;
    pSB->setValue(_value);
    emit modified();
}

void xVDoubleValueDlgItem::setRange(const double& lb,const double& hb)
{
    pSlider->setRange(lb*_sliderReduction,hb*_sliderReduction);
    pSB->setRange(lb,hb);
    _lowLimit=lb;
    _highLimit=hb;
}

void xVDoubleValueDlgItem::setSliderReduction(const double& r)
{
    _sliderReduction=r;
    pSlider->setRange(_lowLimit*_sliderReduction,_highLimit*_sliderReduction);
    pSB->setSingleStep(1.0/(double)_sliderReduction);
}

/*
void xVDoubleValueDlgItem::resizeEvent(QResizeEvent *event)
{
    QSize size=event->size();
    setGeometry(0,0,size.width(),size.height());
    pSlider->setGeometry(0,0,size.width()-60,size.height());
    pSB->setGeometry(size.width()-60,0,60,size.height());
}
*/
