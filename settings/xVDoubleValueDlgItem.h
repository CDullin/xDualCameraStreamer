#ifndef XVDOUBLEVALUEDLGITEM_H
#define XVDOUBLEVALUEDLGITEM_H

#include "xDCS_types.h"
#include <QWidget>
#include <QDoubleSpinBox>
#include <QSlider>

class xVDoubleValueDlgItem:public QWidget
{
Q_OBJECT
public:
    xVDoubleValueDlgItem(QWidget *parent = nullptr);
    void setRange(const double& lb,const double& hb);
    void setSliderReduction(const double& r);
    double value(){return _value;}
public slots:
    void setValue(const double& v);
    void setValue(int v);
signals:
    void modified();
    void KSignal(const SIG_TYPE& t,void *pData=nullptr);
protected:
    //virtual void resizeEvent(QResizeEvent *event) override;
    double _lowLimit,_highLimit,_value,_sliderReduction;
    QDoubleSpinBox *pSB=nullptr;
    QSlider *pSlider=nullptr;
};

#endif // XVDOUBLEVALUEDLGITEM_H
