#include "frmaptest.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    frmAptest w;
    w.show();

    return a.exec();
}
