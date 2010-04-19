#include <QApplication>

#include "yoshitimer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Yoshi Timer");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("Nijenhuis");
    app.setOrganizationDomain("nijenhuis.ca");
    yoshitimer YoshiTimer;
    YoshiTimer.show();
    return app.exec();
}
