#ifndef XVFILEIMPORTDLGITEM_H
#define XVFILEIMPORTDLGITEM_H

#include <QWidget>
#include <QLineEdit>
#include <QFileInfo>
#include <QToolButton>
#include "xDCS_types.h"

class xVFileImportDlgItem:public QWidget
{
    Q_OBJECT
public:
    xVFileImportDlgItem(QWidget *parent=nullptr);
    void setFileName(const xFileName&);
    xFileName fileInfo();
protected slots:
    void browse();
signals:
    void modified();
    void KSignal(const SIG_TYPE& t,void *pData=nullptr);
protected:
    QLineEdit *pLEdit;
    QToolButton *pBrowseTB;
    xFileName _currentFName;
};

#endif // XVFILEIMPORTDLGITEM_H
