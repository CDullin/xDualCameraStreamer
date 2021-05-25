#include "xrcolorpickdlg.h"
#include "ui_xrcolorpickdlg.h"

#include <QPainter>
#include <QColor>
#include <QMouseEvent>

// I dialog to pick or define a color with or without alpha channel
// picking a color is based on an image stored as resource
xrColorPickDlg::xrColorPickDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::xrColorPickDlg)
{
    ui->setupUi(this);
    ui->pColorPixLab->installEventFilter(this);

    connect(ui->pRedSB,SIGNAL(valueChanged(int)),this,SLOT(updateColorLab()));
    connect(ui->pGreenSB,SIGNAL(valueChanged(int)),this,SLOT(updateColorLab()));
    connect(ui->pBlueSB,SIGNAL(valueChanged(int)),this,SLOT(updateColorLab()));
    connect(ui->pAlphaSB,SIGNAL(valueChanged(int)),this,SLOT(updateColorLab()));
}

void xrColorPickDlg::setColor(const QColor & c)
{
    ui->pRedSB->setValue(c.red());
    ui->pGreenSB->setValue(c.green());
    ui->pBlueSB->setValue(c.blue());
    ui->pAlphaSB->isEnabled() ? ui->pAlphaSB->setValue(c.alpha()) : ui->pAlphaSB->setValue(255);
}

QColor xrColorPickDlg::currentColor()
{
    QColor c(ui->pRedSB->value(),ui->pGreenSB->value(),ui->pBlueSB->value(),ui->pAlphaSB->value());
    return c;
}

void xrColorPickDlg::accept()
{
    emit colorPicked(currentColor());
    QDialog::accept();
}

void xrColorPickDlg::setAlphaEnabled(bool b)
{
    if (b)
    {
        ui->pAlphaSB->setEnabled(true);
        ui->pAlphaLab->setEnabled(true);
    }
    else
    {
        ui->pAlphaSB->setEnabled(true);
        ui->pAlphaLab->setEnabled(true);
        ui->pAlphaSB->setValue(255);
    }
}

void xrColorPickDlg::updateColorLab()
{
    QPalette pal=palette();
    pal.setColor(QPalette::Window,QColor(ui->pRedSB->value(),ui->pGreenSB->value(),ui->pBlueSB->value(),ui->pAlphaSB->value()));
    ui->pColLab->setPalette(pal);
}

bool xrColorPickDlg::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->pColorPixLab && event->type()==QEvent::MouseButtonPress)
    {
        QMouseEvent *me=dynamic_cast<QMouseEvent*>(event);
        if (me)
        {
            QPointF p=ui->pColorPixLab->mapFromGlobal(me->globalPos());
            QImage img;
            img = ui->pColorPixLab->pixmap()->toImage();
            QColor col = img.pixelColor(p.x(),p.y());

            ui->pRedSB->setValue(col.red());
            ui->pGreenSB->setValue(col.green());
            ui->pBlueSB->setValue(col.blue());

            updateColorLab();
        }
    }

    QDialog::eventFilter(watched,event);
}

xrColorPickDlg::~xrColorPickDlg()
{
    delete ui;
}
