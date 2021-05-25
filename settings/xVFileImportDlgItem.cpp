#include "xVFileImportDlgItem.h"
#include "xDCS_types.h"
#include <QFileDialog>
#include <QResizeEvent>
#include <QHBoxLayout>
xVFileImportDlgItem::xVFileImportDlgItem(QWidget *parent):QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    setGeometry(0,0,100,20);
    pLEdit = new QLineEdit(this);
    pLEdit->setGeometry(0,0,80,20);
    pLEdit->setAlignment(Qt::AlignRight);
    pLEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    pBrowseTB = new QToolButton(this);
    pBrowseTB->setGeometry(80,0,20,20);
    pBrowseTB->setText("...");
    pBrowseTB->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
    pLEdit->setReadOnly(true);
    connect(pBrowseTB,SIGNAL(clicked()),this,SLOT(browse()));
    QHBoxLayout *pVBox=new QHBoxLayout(this);
    pVBox->setContentsMargins(0,0,0,0);
    pVBox->setSpacing(0);
    pVBox->addWidget(pLEdit);
    pVBox->addWidget(pBrowseTB);
}

void xVFileImportDlgItem::setFileName(const xFileName& fn)
{
    _currentFName = fn;
    pLEdit->setText(fn._fileName);
}

xFileName xVFileImportDlgItem::fileInfo()
{
    return _currentFName;
}
void xVFileImportDlgItem::browse()
{
    QString s;
    switch (_currentFName._type)
    {
    case xFileName::FN_INPUT_FILE: s = QFileDialog::getOpenFileName(0,"chose file",_currentFName._fileName);break;
    case xFileName::FN_OUTPUT_FILE: s = QFileDialog::getSaveFileName(0,"chose file",_currentFName._fileName);break;
    case xFileName::FN_INPUT_DIR: s = QFileDialog::getExistingDirectory(0,"chose directory",_currentFName._fileName);break;
    case xFileName::FN_OUTPUT_DIR: s = QFileDialog::getExistingDirectory(0,"chose directory",_currentFName._fileName);break;
    }
    _currentFName._fileName = s;
    pLEdit->setText(s);
    emit modified();
}
/*
void xVFileImportDlgItem::resizeEvent(QResizeEvent *event)
{
    QSize size=event->size();
    setGeometry(0,0,size.width(),size.height());
    pLEdit->setGeometry(0,0,size.width()-20,size.height());
    pBrowseTB->setGeometry(size.width()-20,0,20,size.height());
}
*/
