#include "method.h"

#include <QtCore/QTextStream>
#include <QList>
#include <QtCore/QStringList>

class Structure
{
public:
    Structure(QString declaration);
    QString serializeMe();

    QString type;
    QString name;
    QList<Attribute> attributes;
    QList<Method> methods;
    QStringList extendsClasses;
    QStringList implementsInterfaces;
};
