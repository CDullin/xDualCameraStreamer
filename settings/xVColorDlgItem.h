#ifndef XVCOLORDLGITEM_H
#define XVCOLORDLGITEM_H

#include <QLabel>
#include <QPixmap>
#include "xDCS_types.h"

class xVColorDlgItem:public QLabel
{
    Q_OBJECT
public:
    xVColorDlgItem(QWidget *parent=nullptr);
    QColor color(){return _color;}
public slots:
    void setColor(const QColor&);
signals:
    void modified();
    void KSignal(const SIG_TYPE& t,void *pData=nullptr);
protected:
    QPixmap generatePixmap();
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    QColor _color;
};

#endif // XVCOLORDLGITEM_H
