#include <QApplication>

#include "parser.h"

int main(int argc, char* argv[])
{
    QStringList l;
    l << "kernel_metamodel.xml" << "requirements_metamodel.xml" << "usecase_metamodel.xml";
    new Parser(l);
    return 0;
}
