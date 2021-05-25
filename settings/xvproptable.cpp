#include "xvproptable.h"
#include "xVCustomTableItems.h"
#include <QComboBox>
#include <QApplication>
#include <QHeaderView>

xVPropTable::xVPropTable(QWidget* parent):QTableWidget(parent)
{
    connect(this,SIGNAL(itemChanged(QTableWidgetItem *)),this,SLOT(itemChanged_SLOT(QTableWidgetItem *)));
    verticalHeader()->setVisible(false);
    horizontalHeader()->setVisible(false);
    horizontalHeader()->setStretchLastSection(true);
}


void xVPropTable::KSlot(const SIG_TYPE& type,void *pData)
{
    switch (type)
    {
    default:
        //handled
        break;
    }
}

void xVPropTable::updateLimits()
{
    /*
    for (int r=0;r<rowCount();++r)
    {
        QString key=item(r,0)->text();
        if (pCurrentObj->paramMap()->contains(key))
        {
            QVariant var = (*pCurrentObj->paramMap())["key"]._value;
            bool _enabled = (*pCurrentObj->paramMap())["key"]._enabled;

            if (var.userType()==QMetaType::type("xLimitedInt"))
            {
                xVIntValueDlgItem* pItem = (xVIntValueDlgItem*)item(r,1);
                pItem->setRange(var.value<xLimitedInt>()._lowerLimit,var.value<xLimitedInt>()._upperLimit);
            }
            if (var.userType()==QMetaType::type("xLimitedDouble"))
            {
                xVDoubleValueDlgItem* pItem = (xVDoubleValueDlgItem*)item(r,1);
                pItem->setRange(var.value<xLimitedDouble>()._lowerLimit,var.value<xLimitedDouble>()._upperLimit);
            }

            if (!_enabled)
            {
                if (item(r,0))
                {
                    item(r,0)->setFlags(Qt::NoItemFlags);
                    item(r,0)->setIcon(QIcon(QPixmap("://images/lock_closed.png")));
                }
                if (item(r,1)) item(r,1)->setFlags(Qt::NoItemFlags);
            }
            else
            {
                if (item(r,0))
                {
                    item(r,0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    item(r,0)->setIcon(QIcon(QPixmap("://images/lock_open.png")));
                }
                if (item(r,1)) item(r,1)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
            }
            QWidget *pWdgt = dynamic_cast<QWidget*>(cellWidget(r,1));
            if (pWdgt) pWdgt->setEnabled(_enabled);
        }
    }
    */
}

bool xVPropTable::keyExists(const QString& txt)
{
    bool res=false;
    for (int i=0;i<rowCount() && !res;++i)
        res = item(i,0)->text()==txt;
    return res;
}

void xVPropTable::itemChanged_SLOT(QTableWidgetItem *pItem)
{
    if (pItem && pParamMpRef)
    {
        // find name
        QString name=item(row(pItem),0)->text();
        // now we need to get the value based on the type
        if (pParamMpRef->contains(name))
        {
            switch ((*pParamMpRef)[name]._value.type())
            {
            case QVariant::String : (*pParamMpRef)[name]._value=pItem->text();break;
            }

            // trigger update
            emit paramModified(name);
        }
    }
}

void xVPropTable::customItemChanged()
{
    if (pParamMpRef==0) return;
    // identify key
    QWidget* wdgt = dynamic_cast<QWidget*>(sender());
    if (wdgt)
    {
        long row=-1;
        for (long i=0;i<rowCount();++i) {
            if (cellWidget(i,1)==wdgt)
                row=i;
        }

        //get type from key
        if (row>-1)
        {
            QVariant var=(*pParamMpRef)[item(row,0)->text()]._value;
            int id=var.type();
            switch (var.type())
            {
            case QVariant::Int:
            case QVariant::LongLong:
            case QVariant::UInt:
            case QVariant::ULongLong:
            {
                xVIntValueDlgItem* pItem=dynamic_cast<xVIntValueDlgItem*>(wdgt);
                if (pItem)
                    (*pParamMpRef)[item(row,0)->text()]._value=pItem->value();
            }
                break;
            case QVariant::Double:
            {
                xVDoubleValueDlgItem* pItem = dynamic_cast<xVDoubleValueDlgItem*>(wdgt);
                if (pItem)
                    (*pParamMpRef)[item(row,0)->text()]._value=pItem->value();
            }
                break;
            case QVariant::Bool:
            {
                xVBoolDlgItem* pItem = dynamic_cast<xVBoolDlgItem*>(wdgt);
                if (pItem)
                    (*pParamMpRef)[item(row,0)->text()]._value=pItem->condition();
            }
                break;
            case QVariant::Color:
            {
                xVColorDlgItem* pItem = dynamic_cast<xVColorDlgItem*>(wdgt);
                if (pItem)
                    (*pParamMpRef)[item(row,0)->text()]._value=pItem->color();
            }
                break;
            case QVariant::Vector3D:
            {
                xVVector3DDlgItem* pItem = dynamic_cast<xVVector3DDlgItem*>(wdgt);
                if (pItem)
                    (*pParamMpRef)[item(row,0)->text()]._value=QVariant::fromValue(pItem->vector());
            }
                break;
            case QVariant::UserType:
            {
                if (var.userType()==QMetaType::type("xFileName"))
                {
                    xVFileImportDlgItem* pItem = dynamic_cast<xVFileImportDlgItem*>(wdgt);
                    if (pItem)
                        ((*pParamMpRef)[item(row,0)->text()]._value)=QVariant::fromValue(pItem->fileInfo());
                }
                if (var.userType()==QMetaType::type("x3D_SAMPLE_POS"))
                {
                    xVSamplePosDlgItem* pItem = dynamic_cast<xVSamplePosDlgItem*>(wdgt);
                    if (pItem)
                    {
                        ((*pParamMpRef)[item(row,0)->text()]._value)=QVariant::fromValue(pItem->samplePos());
                    }
                }
                if (var.userType()==QMetaType::type("vtkPiecewiseFunctionPtr"))
                {
                    // direct update via ptr
                }
                if (var.userType()==QMetaType::type("vtkColorTransferFunctionPtr"))
                {
                    // direct update via ptr
                }
                if (var.userType()==QMetaType::type("xLimitedInt"))
                {
                    xVIntValueDlgItem *pItem = dynamic_cast<xVIntValueDlgItem*>(wdgt);
                    if (pItem)
                    {
                        xLimitedInt val = (*pParamMpRef)[item(row,0)->text()]._value.value<xLimitedInt>();
                        val._value = pItem->value();
                        ((*pParamMpRef)[item(row,0)->text()]._value)=QVariant::fromValue(val);
                    }
                }
                if (var.userType()==QMetaType::type("xLimitedDouble"))
                {
                    xVDoubleValueDlgItem *pItem = dynamic_cast<xVDoubleValueDlgItem*>(wdgt);
                    if (pItem)
                    {
                        xLimitedDouble val = (*pParamMpRef)[item(row,0)->text()]._value.value<xLimitedDouble>();
                        val._value = pItem->value();
                        ((*pParamMpRef)[item(row,0)->text()]._value)=QVariant::fromValue(val);
                    }
                }
            }
                break;
            case QVariant::String:
            {
                if (::_optionLsts.contains(item(row,0)->text()))
                {
                    QComboBox *pCB = dynamic_cast<QComboBox*>(wdgt);
                    if (pCB) {
                        *(QString*)((*pParamMpRef)[item(row,0)->text()]._value.data())=pCB->currentText();
                        updateRowVisibility();
                    }
                }
            }
                break;
            }
            paramModified(item(row,0)->text());
        }
    }
}


xPROP_TYPE propForId(QMap<QString,xPROP_TYPE>*mp,const int& id,QString &key)
{
    xPROP_TYPE prop;

    for (QMap<QString,xPROP_TYPE>::iterator it=mp->begin();it!=mp->end();++it)
        if (it->_id==id) {
            prop=it.value();
            key=it.key();
        }
    return prop;
}

void xVPropTable::subgroupChanged()
{
    QComboBox *pCB=dynamic_cast<QComboBox*>(sender());
    if (pCB && pParamMpRef)
    {
        // find row
        long cbr = -1;
        for (long r=0;r<rowCount();++r)
        {
            if (cellWidget(r,1)==pCB)
                cbr=r;
        }
        if (cbr>-1)
        {
            QStringList options = _optionLsts[item(cbr,0)->text()];
            for (long r=0;r<rowCount();++r)
            {
                QString key = item(r,0)->text();
                if (pParamMpRef->contains(key) && !(*pParamMpRef)[key]._subGrp.isEmpty() && options.contains((*pParamMpRef)[key]._subGrp))
                    setRowHidden(r,(*pParamMpRef)[key]._subGrp!=pCB->currentText());
            }
        }
    }
}

void xVPropTable::setOnlyRealDataTypesVisible(bool b)
{
    _onlyShowRealDatatypes =b;
    updateTable();
}

void xVPropTable::updateRowVisibility()
{
    if (pParamMpRef==nullptr) return;
    // get condition table
    QStringList _conditions;
    for (QMap<QString,xPROP_TYPE>::iterator it=pParamMpRef->begin();it!=pParamMpRef->end();++it)
    {
        if (it->_value.type()==QVariant::String && ::_optionLsts.contains(it.key()))
            _conditions.append(it.value()._value.toString());
    }

    for (int i=0;i<rowCount();++i)
    {
        if ((*pParamMpRef).contains(item(i,0)->text()))
        {
            xPROP_TYPE prop=(*pParamMpRef)[item(i,0)->text()];
            if (!prop._subGrp.isEmpty() && !_conditions.contains(prop._subGrp)) setRowHidden(i,true);
            else setRowHidden(i,false);
        }
    }
}

void xVPropTable::updateTable(QMap<QString, xPROP_TYPE> *pParamMp)
{
    if (pParamMp==nullptr) return;

    disconnect(this,SIGNAL(itemChanged(QTableWidgetItem *)),this,SLOT(itemChanged_SLOT(QTableWidgetItem *)));
    pParamMpRef=pParamMp;

    for (int i=0;i<rowCount();++i) {
        for (int j=0;j<columnCount();++j)
            if (cellWidget(i,j)!=nullptr)
            {
                delete cellWidget(i,1);
                //removeCellWidget(i,1);
            }
    }

    setRowCount(0);
    setColumnCount(3);

    // get ordered map
    QVector <int> idVec;
    long r=0;
    for (QMap<QString,xPROP_TYPE>::iterator it=pParamMp->begin();it!=pParamMp->end();++it)
            idVec.append((*it)._id);
    std::sort(idVec.begin(), idVec.end(), [](const int& a, const int& b) {return a < b;});


    // get condition table
    QStringList _conditions;
    for (QMap<QString,xPROP_TYPE>::iterator it=pParamMp->begin();it!=pParamMp->end();++it)
    {
        if (it->_value.type()==QVariant::String && ::_optionLsts.contains(it.key()))
            _conditions.append(it.value()._value.toString());
    }

    QStringList handeledByDialogLst;

    bool _refObjFound=false;
    r=0;
    for (int i=0;i<idVec.count();++i)
    {
        QString key;
        xPROP_TYPE prop=propForId(pParamMp,idVec[i],key);
        setRowCount(r+1);
        qApp->processEvents();
        QTableWidgetItem *pItem0=new QTableWidgetItem(key);
        pItem0->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        pItem0->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        bool handledByDialog = handeledByDialogLst.contains(key);
        (handledByDialog || !prop._enabled) ? pItem0->setIcon(QIcon(QPixmap("://images/lock_closed.png"))) : pItem0->setIcon(QIcon(QPixmap("://images/lock_open.png")));
        setItem(r,0,pItem0);

        bool _realDataType = false;

        if (!handledByDialog)
        {
            switch ((int)prop._value.type())
        {
        case QVariant::Int:
        case QVariant::LongLong:
        case QVariant::UInt:
        case QVariant::ULongLong:
        {
            xVIntValueDlgItem* pItem = new xVIntValueDlgItem();
            pItem->setRange(::_limitMp[key]._lowerLimit,::_limitMp[key]._upperLimit);
            pItem->setValue(prop._value.toLongLong());
            setCellWidget(r,1,pItem);
            connect(pItem,SIGNAL(modified()),this,SLOT(customItemChanged()));
            connect(pItem,SIGNAL(KSignal(const SIG_TYPE& ,void *)),this,SIGNAL(KSignal(const SIG_TYPE& ,void *)));

            _realDataType = true;
        }
        break;
        case QVariant::Double:
        {
            xVDoubleValueDlgItem* pItem = new xVDoubleValueDlgItem();
            pItem->setSliderReduction(::_limitMp[key]._reduction);
            pItem->setRange(::_limitMp[key]._lowerLimit,::_limitMp[key]._upperLimit);
            pItem->setValue(prop._value.toDouble());
            setCellWidget(r,1,pItem);
            connect(pItem,SIGNAL(modified()),this,SLOT(customItemChanged()));
            connect(pItem,SIGNAL(KSignal(const SIG_TYPE& ,void *)),this,SIGNAL(KSignal(const SIG_TYPE& ,void *)));

            _realDataType = true;
        }
        break;
        case QVariant::Bool:
        {
            xVBoolDlgItem* pItem = new xVBoolDlgItem();
            pItem->setCondition(prop._value.toBool());
            setCellWidget(r,1,pItem);
            connect(pItem,SIGNAL(modified()),this,SLOT(customItemChanged()));
            connect(pItem,SIGNAL(KSignal(const SIG_TYPE& ,void *)),this,SIGNAL(KSignal(const SIG_TYPE& ,void *)));

            _realDataType = true;
        }
        break;
        case QVariant::Color:
        {
            xVColorDlgItem* pItem = new xVColorDlgItem();
            pItem->setColor(prop._value.value<QColor>());
            setCellWidget(r,1,pItem);
            connect(pItem,SIGNAL(modified()),this,SLOT(customItemChanged()));
            connect(pItem,SIGNAL(KSignal(const SIG_TYPE& ,void *)),this,SIGNAL(KSignal(const SIG_TYPE& ,void *)));
        }
            break;
        case QVariant::String:
        {
            if (::_optionLsts.contains(key))
            {
                QComboBox *pCB=new QComboBox();
                pCB->insertItems(0,::_optionLsts[key]);
                QString *s=(QString*)(prop._value.data());
                pCB->setCurrentText(*s);
                setCellWidget(r,1,pCB);
                connect(pCB,SIGNAL(activated(int)),this,SLOT(customItemChanged()));
                connect(pCB,SIGNAL(activated(int)),this,SLOT(subgroupChanged()));
            }
            else {
                setItem(r,1,new QTableWidgetItem(prop._value.toString()));
                _realDataType = true;
            }
        }
            break;
        case QVariant::Vector3D:
        {
            xVVector3DDlgItem* pItem = new xVVector3DDlgItem();
            QVector3D pos=prop._value.value<QVector3D>();
            if (::_limitMp.contains(key)) pItem->setRange(::_limitMp[key]._lowerLimit,::_limitMp[key]._upperLimit);
            else pItem->setRange(0,65535);
            pItem->setVector(pos);
            setCellWidget(r,1,pItem);
            connect(pItem,SIGNAL(modified()),this,SLOT(customItemChanged()));
            connect(pItem,SIGNAL(KSignal(const SIG_TYPE& ,void *)),this,SIGNAL(KSignal(const SIG_TYPE& ,void *)));
        }
        break;
        case QVariant::UserType:
        {
            if ((int)prop._value.userType()==QMetaType::type("xLimitedInt"))
            {
                xVIntValueDlgItem* pItem = new xVIntValueDlgItem();
                xLimitedInt val=prop._value.value<xLimitedInt>();
                pItem->setRange(val._lowerLimit,val._upperLimit);
                pItem->setValue(val._value);
                setCellWidget(r,1,pItem);
                connect(pItem,SIGNAL(modified()),this,SLOT(customItemChanged()));
                connect(pItem,SIGNAL(KSignal(const SIG_TYPE& ,void *)),this,SIGNAL(KSignal(const SIG_TYPE& ,void *)));

                _realDataType = true;
            }
            if ((int)prop._value.userType()==QMetaType::type("xLimitedDouble"))
            {
                xVDoubleValueDlgItem* pItem = new xVDoubleValueDlgItem();
                xLimitedDouble val=prop._value.value<xLimitedDouble>();
                pItem->setSliderReduction(val._precision);
                pItem->setRange(val._lowerLimit,val._upperLimit);
                pItem->setValue(val._value);
                setCellWidget(r,1,pItem);
                connect(pItem,SIGNAL(modified()),this,SLOT(customItemChanged()));
                connect(pItem,SIGNAL(KSignal(const SIG_TYPE& ,void *)),this,SIGNAL(KSignal(const SIG_TYPE& ,void *)));

                _realDataType = true;
            }
            if ((int)prop._value.userType()==QMetaType::type("xFileName"))
            {
                xVFileImportDlgItem* pItem = new xVFileImportDlgItem();
                xFileName pInfo=prop._value.value<xFileName>();
                pItem->setFileName(pInfo);
                setCellWidget(r,1,pItem);
                connect(pItem,SIGNAL(modified()),this,SLOT(customItemChanged()));
                connect(pItem,SIGNAL(KSignal(const SIG_TYPE& ,void *)),this,SIGNAL(KSignal(const SIG_TYPE& ,void *)));
            }
            if ((int)prop._value.userType()==QMetaType::type("x3D_SAMPLE_POS"))
            {
                xVSamplePosDlgItem* pItem = new xVSamplePosDlgItem();
                x3D_SAMPLE_POS pos=prop._value.value<x3D_SAMPLE_POS>();
                if (::_limitMp.contains(key)) pItem->setRange(::_limitMp[key]._lowerLimit,::_limitMp[key]._upperLimit);
                else pItem->setRange(0,65535);
                pItem->setPos(pos);
                setCellWidget(r,1,pItem);
                connect(pItem,SIGNAL(modified()),this,SLOT(customItemChanged()));
                connect(pItem,SIGNAL(KSignal(const SIG_TYPE& ,void *)),this,SIGNAL(KSignal(const SIG_TYPE& ,void *)));
            }
        }
            break;
        default: setItem(r,1,new QTableWidgetItem("unsupported data type"));break;
        }
        }
        else
        {
            QTableWidgetItem *pItem=new QTableWidgetItem("<handled by dialog>");
            pItem->setTextAlignment(Qt::AlignCenter);
            pItem->setForeground(Qt::red);
            pItem->setBackground(Qt::yellow);
            setItem(r,1,pItem);
        }

        QWidget *pWdgt=dynamic_cast<QWidget*>(cellWidget(r,1));
        if (pWdgt) pWdgt->setEnabled(prop._enabled);

        qApp->processEvents();

        if (!prop._subGrp.isEmpty() && !_conditions.contains(prop._subGrp))
            setRowHidden(r,true);
        else
            setRowHidden(r,false);
        if (!isRowHidden(r)) setRowHidden(r,!_realDataType && _onlyShowRealDatatypes);
        ++r;
    }

    setColumnHidden(2,!_refObjFound || _objectColumnStaysHidden);
    resizeColumnToContents(0);
    horizontalHeader()->resizeSection(1,150);
    connect(this,SIGNAL(itemChanged(QTableWidgetItem *)),this,SLOT(itemChanged_SLOT(QTableWidgetItem *)));

    subgroupChanged();
}
