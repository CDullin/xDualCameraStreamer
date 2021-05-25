#include "xDCSDlg.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    initAll();
    QApplication a(argc, argv);
    xDCSDlg w;
    w.show();
    return a.exec();
}
