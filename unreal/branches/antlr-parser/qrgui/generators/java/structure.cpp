#include "structure.h"

Structure::Structure(QString declaration)
{
    while (declaration.endsWith(" ")) {
        declaration.remove(declaration.length()-1, 1);
    }

    while (declaration.startsWith(" ")) {
        declaration.remove(0, 1);
    }

    int index = declaration.indexOf(" implements ");
    if (index != -1) {
        QString implements = declaration.mid(index + QString(" implements ").length());
        implementsInterfaces = implements.split(" , ");
        declaration.remove(index, declaration.length() - index );
    }

    index = declaration.indexOf(" extends ");
    if (index != -1) {
        QString extends = declaration.mid(index + QString(" extends ").length());
        extendsClasses = extends.split(" ");
        declaration.remove(index, declaration.length() - index);
    }

    QStringList leftToParse = declaration.split(" ");
    if (leftToParse.size() == 3) {
        type = leftToParse.at(1); //class or interface
        name = leftToParse.at(2);
    } else if (leftToParse.size() == 2) {
        type = leftToParse.at(0); //class or interface
        name = leftToParse.at(1);
    }
}

QString Structure::serializeMe()
{
    QString extends;
    Q_FOREACH (QString aClass, extendsClasses) {
        extends += aClass + " ";
    }

    QString implements;
    Q_FOREACH (QString anInterface, implementsInterfaces) {
        implements += anInterface + " ";
    }

    QString methodsString;
    Q_FOREACH (Method aMethod, methods) {
        methodsString += "\n    " + aMethod.serializeMe();
    }

    QString attributesString;
    Q_FOREACH (Attribute anAttribute, attributes) {
        attributesString += "\n    " + anAttribute.serializeMe();
    }

    return type + " " + name + " extends " + extends +
            " implements " + implements + "\nmethods :" + methodsString +
            "\nattributes :" + attributesString;
}
