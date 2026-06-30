#include <QApplication>
#include "dashboard.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Dashboard dashboard;
    dashboard.show(); //teste
    return app.exec();
}