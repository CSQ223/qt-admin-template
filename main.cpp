#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/resources/images/logo.png"));

    MainWindow w;
//    w.show();
    w.showMaximized();

    return a.exec();
}
