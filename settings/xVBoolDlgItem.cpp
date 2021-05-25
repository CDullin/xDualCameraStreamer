#include "xVBoolDlgItem.h"
#include <QResizeEvent>
#include <QHBoxLayout>

xVBoolDlgItem::xVBoolDlgItem(QWidget *parent):QWidget(parent)
{
    setGeometry(0,0,100,20);
    pRBtn0 = new QRadioButton(this);
    pRBtn0->setText("yes");
    pRBtn0->setGeometry(0,0,50,20);
    pRBtn1 = new QRadioButton(this);
    pRBtn1->setGeometry(50,0,50,20);
    pRBtn1->setText("no");
    pRBtn0->setCheckable(true);
    pRBtn0->setChecked(true);
    pRBtn0->setAutoExclusive(true);
    pRBtn1->setCheckable(true);
    pRBtn1->setChecked(false);
    pRBtn1->setAutoExclusive(true);

    connect(pRBtn0,SIGNAL(toggled(bool)),this,SIGNAL(modified()));
    connect(pRBtn1,SIGNAL(toggled(bool)),this,SIGNAL(modified()));

    QHBoxLayout *pVBox=new QHBoxLayout(this);
    pVBox->setContentsMargins(10,0,0,0);
    pVBox->addWidget(pRBtn0);
    pVBox->addWidget(pRBtn1);
}

void xVBoolDlgItem::setCondition(const bool& b)
{
     b ? pRBtn0->setChecked(true) : pRBtn1->setChecked(true);
}

/*
void xVBoolDlgItem::resizeEvent(QResizeEvent *event)
{
    QSize size=event->size();
    setGeometry(0,0,size.width(),size.height());
    pRBtn0->setGeometry(5,0,size.width()/2-10,size.height());
    pRBtn1->setGeometry(size.width()/2+5,0,size.width()/2-10,size.height());
}
*/
