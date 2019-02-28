#include <QApplication>
#include "core.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QApplication::setOrganizationName("DrSasha");
    //QApplication::setApplicationName("iDeA-Craft");
    Core core(&a);
    return a.exec();
}

