#include <QtCore/QCoreApplication>
#include "api_scilab.h"
#include "stack-c.h"
#include "call_scilab.h"
#include "MatrixWork.cpp"
#include <QFile>
#include <QTextStream>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    

    MatrixWork mw;

    mw.StartWork();

    SendScilabJob("cd D:\\QT\\scilab2c_1.1-3.bin\\scilab2c_1.1");

    SendScilabJob("exec loader.sce");

    SendScilabJob("scilab2c('D:\\QT\\scilab2c_1.1-3.bin\\scilab2c_1.1\\tests\\unit_tests\\test_double_abs.sci', 'D:\\Study')");
    
    return a.exec();

}
