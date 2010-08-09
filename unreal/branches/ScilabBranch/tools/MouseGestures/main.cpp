#include <QtGui/QApplication>
#include "mousegestures.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MouseGestures w;
    w.show();
    return a.exec();
}
