/*
 * handles definition of a color by the user
 * CDullin xPIT 2020
 */


#ifndef XRCOLORPICKDLG_H
#define XRCOLORPICKDLG_H

#include <QDialog>

namespace Ui {
class xrColorPickDlg;
}

class xrColorPickDlg : public QDialog
{
    Q_OBJECT

public:
    explicit xrColorPickDlg(QWidget *parent = nullptr);
    ~xrColorPickDlg();

    void setColor(const QColor&);
    QColor currentColor();

    void setAlphaEnabled(bool b);

protected:
    virtual bool eventFilter(QObject*,QEvent*) override;

protected slots:
    void updateColorLab();
    virtual void accept() override;

signals:
    void colorPicked(const QColor& );

private:
    Ui::xrColorPickDlg *ui;
};

#endif // XRCOLORPICKDLG_H
