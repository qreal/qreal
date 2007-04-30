#include <QApplication>

#include "generator.h"

int main(int argc, char* argv[])
{
    QStringList l;
    l << "kernel_metamodel.xml" << "requirements_metamodel.xml";// << "usecase_metamodel.xml" << "class_metamodel.xml" << "statemachines_metamodel.xml";
    new Generator(l);
    return 0;
}
