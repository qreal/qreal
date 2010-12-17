#include <QtGui/QApplication>
#include "neuralnetworkwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NeuralNetworkWindow w;
    w.show();

    return a.exec();
}
