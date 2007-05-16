#include "generator.h"

int main()
{
    QStringList l;
    l   << "kernel_metamodel.xml" 
        << "basicbehaviors_metamodel.xml" 
        << "requirements_metamodel.xml" 
        << "class_metamodel.xml"
        << "usecase_metamodel.xml";
    new Generator(l);
    return 0;
}
