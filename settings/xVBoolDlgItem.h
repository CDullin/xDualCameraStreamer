#ifndef XVBOOLDLGITEM_H
#define XVBOOLDLGITEM_H

#include <QRadioButton>
#include <QWidget>
#include "xDCS_types.h"

class xVBoolDlgItem:public QWidget
{
Q_OBJECT
public:
    xVBoolDlgItem(QWidget *parent = nullptr);
    void setCondition(const bool& b);
    bool condition(){return pRBtn0->isChecked();}
signals:
    void modified();
    void KSignal(const SIG_TYPE& t,void *pData=nullptr);
protected:
    //virtual void resizeEvent(QResizeEvent *event) override;
    QRadioButton *pRBtn0,*pRBtn1;
};


#endif // XVBOOLDLGITEM_H
