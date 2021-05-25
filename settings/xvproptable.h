#ifndef XVPROPTABLE_H
#define XVPROPTABLE_H

#include <QTableWidget>
#include <QObject>
#include "xDCS_types.h"
#include "xDCSDlg.h"

class xVPropTable : public QTableWidget
{
    Q_OBJECT
public:
    xVPropTable(QWidget* parent=nullptr);
    void updateTable(QMap<QString,xPROP_TYPE>*);
    void updateTable(){updateTable(pParamMpRef);}
    void setOnlyRealDataTypesVisible(bool);
    bool keyExists(const QString&);
    void setObjectColumnStaysHidden(bool b){_objectColumnStaysHidden=b;}
    xParamMap* paramMap(){return pParamMpRef;}

public slots:
    void KSlot(const SIG_TYPE& t,void *pData=nullptr);

signals:
    void KSignal(const SIG_TYPE& t,void *pData);
    void paramModified(const QString&);

protected slots:
    void itemChanged_SLOT(QTableWidgetItem*);
    void customItemChanged();
    void subgroupChanged();

protected:
    void updateRowVisibility();
    void updateLimits();
    QMap<QString,xPROP_TYPE>* pParamMpRef = nullptr;
    bool _objectColumnStaysHidden = false;
    bool _onlyShowRealDatatypes = false;
};

#endif // XVPROPTABLE_H
