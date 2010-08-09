#include <QtGui/QApplication>
#include "windows.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Windows w;
    w.show();
    return a.exec();
}
