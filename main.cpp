#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("NITSER");
    QCoreApplication::setApplicationName("YoshiTimer");
    MainWindow w;
    w.show();

    return a.exec();
}
