#ifndef XVSAMPLEPOSDLGITEM_H
#define XVSAMPLEPOSDLGITEM_H

#include "xDCS_types.h"
#include <QWidget>
#include <QSpinBox>
#include <QHBoxLayout>

class xVSamplePosDlgItem:public QWidget
{
    Q_OBJECT
public:
    enum AXIS
    {
        X_AXIS = 0x01,
        Y_AXIS = 0x02,
        Z_AXIS = 0x03
    };

    xVSamplePosDlgItem(QWidget* parent = nullptr);
    void setRange(const int& lb,const int& hb,const int axis=X_AXIS|Y_AXIS|Z_AXIS);
    x3D_SAMPLE_POS samplePos(){return _pos;}
    void setPos(const x3D_SAMPLE_POS& p);

protected slots:
    void posChanged();

signals:
    void modified();
    void KSignal(const SIG_TYPE& t,void *pData=nullptr);
protected:
    //virtual void resizeEvent(QResizeEvent *event) override;

    x3D_SAMPLE_POS _pos;
    qint64 _lowLimit[3],_highLimit[3];
    QSpinBox *pSBX=nullptr;
    QSpinBox *pSBY=nullptr;
    QSpinBox *pSBZ=nullptr;
    QHBoxLayout *pBoxLayout = nullptr;
};

#endif // XVSAMPLEPOSDLGITEM_H
