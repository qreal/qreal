#include <QApplication>

#include "parser.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QStringList l;
    l << "kernel_metamodel.xml" << "requirements_metamodel.xml";
    Parser *p = new Parser(l);
    return app.exec();
}
