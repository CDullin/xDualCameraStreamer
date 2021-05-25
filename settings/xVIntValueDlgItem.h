#ifndef XVINTVALUEDLGITEM_H
#define XVINTVALUEDLGITEM_H

#include "xDCS_types.h"
#include <QWidget>
#include <QSpinBox>
#include <QSlider>

class xVIntValueDlgItem:public QWidget
{
Q_OBJECT
public:
    xVIntValueDlgItem(QWidget *parent = nullptr);
    void setRange(const int& lb,const int& hb);
    qint64 value(){return _value;}
public slots:
    void setValue(const int& v);
signals:
    void modified();
    void KSignal(const SIG_TYPE& t,void *pData=nullptr);
protected:
    //virtual void resizeEvent(QResizeEvent *event) override;
    qint64 _lowLimit,_highLimit,_value;
    QSpinBox *pSB=nullptr;
    QSlider *pSlider=nullptr;
};


#endif // XVINTVALUEDLGITEM_H
