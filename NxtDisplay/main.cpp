#include <QtGui/QApplication>

#include "mainwindow.h"
#include "nxtdisplay.h"

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    MainWindow w;
    NxtDisplay x;
    w.show();
    x.show();
    
    return a.exec();
}
