#include "attribute.h"

Attribute::Attribute(QString declaration)
{
    while (declaration.endsWith(" ")) {
        declaration.remove(declaration.length()-1, 1);
    }

    while (declaration.startsWith(" ")) {
        declaration.remove(0, 1);
    }

    isStatic = setBoolField(declaration, "static");
    isFinal = setBoolField(declaration, "final");
    isVolatile = setBoolField(declaration, "volatile");
    isTransient = setBoolField(declaration, "transient");

    int index = declaration.indexOf(" = ");
    if (index != -1) {
        defaultValue = declaration.mid(index + QString(" = ").length());
        declaration.remove(index, declaration.length() - index);
    }

    index = declaration.indexOf(" [ ]");
    if (index != -1) {
        type = "[]";
        declaration.remove(index, declaration.length() - index);
    }

    QStringList leftToParse = declaration.split(" ");
    if (leftToParse.size() == 3) {
        visibility = leftToParse.at(0);
        type = leftToParse.at(1) + type;
        name = leftToParse.at(2);
    } else if (leftToParse.size() == 2) {
        type = leftToParse.at(0) + type;
        name = leftToParse.at(1);
    }
}

QString Attribute::serializeMe()
{
    QString result;

    result += visibility + " ";
    if (isStatic) {
        result += "static ";
    }
    if (isFinal) {
        result += "final ";
    }
    if (isVolatile) {
        result += "volatile ";
    }
    if (isTransient) {
        result += "transient ";
    }
    result += type + " ";
    result += name + " ";
    if (!defaultValue.isEmpty()) {
        result += defaultValue;
    }

    return result;
}

bool Attribute::setBoolField(QString &declaration, QString fieldString)
{
    int index = declaration.indexOf(" " + fieldString + " ");
    if (index != -1) {
        declaration.remove(index, QString(" " + fieldString).length());
        return true;
    } else {
        return false;
    }
}
