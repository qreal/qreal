#include "method.h"

#include <QDebug>

Method::Method(QString declaration)
{
    while (declaration.endsWith(" ")) {
        declaration.remove(declaration.length()-1, 1);
    }

    while (declaration.startsWith(" ")) {
        declaration.remove(0, 1);
    }

    isAbstract = setBoolField(declaration, "abstract");
    isStatic = setBoolField(declaration, "static");
    isFinal = setBoolField(declaration, "final");
    isSynchronized = setBoolField(declaration, "synchronized");
    isNative = setBoolField(declaration, "native");

    int index = declaration.indexOf(" throws ");
    if (index != -1) {
        QString exceptionsStr = declaration.mid(index + QString(" throws ").length());
        exceptions = exceptionsStr.split(" , ");
        declaration.remove(index, declaration.length() - index);
    }

    index = declaration.indexOf(" ( ");
    if (index != -1) {
        int indexRB = declaration.indexOf(" )");
        int position = index + QString(" ( ").length();
        int length = indexRB - position;
        if (length > 0) {
            QString operationFactorsStr = declaration.mid(position, length);

            QStringList operationFactorsLst = operationFactorsStr.split(" , ");

            Q_FOREACH (QString operationFactor, operationFactorsLst) {
                Attribute attribute(operationFactor);
                operationFactors.append(attribute);
            }
        }

        declaration.remove(index, indexRB - index + QString(" )").length());
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
        visibility = leftToParse.at(0);
        name = leftToParse.at(1);
    }
}

QString Method::serializeMe()
{
    QString result;

    QString operationFactorsString;
    Q_FOREACH (Attribute aFactor, operationFactors) {
        operationFactorsString += "\n" + aFactor.serializeMe() + " ";
    }

    QString exceptionsString;
    Q_FOREACH (QString exception, exceptions) {
        exceptionsString += exception + " ";
    }

    result += visibility + " ";
    if (isAbstract) {
        result += "abstract ";
    }
    if (isStatic) {
        result += "static ";
    }
    if (isFinal) {
        result += "final ";
    }
    if (isSynchronized) {
        result += "synchronized ";
    }
    if (isNative) {
        result += "native ";
    }
    result += type + " ";
    result += name + " ";
    result += "\n      factors: " + operationFactorsString;
    result += "\n      throws: " + exceptionsString;

    return result;
}

bool Method::setBoolField(QString &declaration, QString fieldString)
{
    int index = declaration.indexOf(" " + fieldString + " ");
    if (index != -1) {
        declaration.remove(index, QString(" " + fieldString).length());
        return true;
    } else {
        return false;
    }
}
