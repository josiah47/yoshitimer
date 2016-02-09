#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("YoshiTimer");

    MainWindow w;
    w.show();

    return a.exec();
}
