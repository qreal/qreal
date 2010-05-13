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
    bool isFinal;
    bool isAbstract;
    QString name;
    QList<Attribute> attributes;
    QList<Method> methods;
    QStringList extendsClasses;
    QStringList implementsInterfaces;
private:
    bool setBoolField(QString &declaration, QString fieldString);
};
