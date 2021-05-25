#ifndef XVVECTOR3DDLGITEM_H
#define XVVECTOR3DDLGITEM_H

#include "xDCS_types.h"
#include <QWidget>
#include <QVector3D>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QHBoxLayout>

class xVVector3DDlgItem:public QWidget
{
    enum AXIS
    {
        X_AXIS = 0x01,
        Y_AXIS = 0x02,
        Z_AXIS = 0x03
    };

    Q_OBJECT
public:
    xVVector3DDlgItem(QWidget* parent = nullptr);
    void setRange(const double& lb,const double& hb,const int axis=X_AXIS|Y_AXIS|Z_AXIS);
    QVector3D vector(){return _vector;}
    void setVector(const QVector3D& v);
protected slots:
    void vecChanged();
signals:
    void modified();
    void KSignal(const SIG_TYPE& t,void *pData=nullptr);
protected:
    //virtual void resizeEvent(QResizeEvent *event) override;

    QVector3D _vector;
    double _lowLimit[3],_highLimit[3];
    QDoubleSpinBox *pSBX=nullptr;
    QDoubleSpinBox *pSBY=nullptr;
    QDoubleSpinBox *pSBZ=nullptr;
    QHBoxLayout *pBoxLayout = nullptr;
};

#endif // XVVECTOR3DDLGITEM_H
