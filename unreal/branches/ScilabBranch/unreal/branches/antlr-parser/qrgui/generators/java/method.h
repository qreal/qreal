#include <QList>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>

#include "attribute.h"

class Method
{
public:
    Method(QString declaration);
    QString serializeMe();

    QString visibility;
    bool isAbstract;
    bool isStatic;
    bool isFinal;
    bool isSynchronized;
    bool isNative;
    QString type;
    QString name;
    QList<Attribute> operationFactors;
    QStringList exceptions;
private:
    bool setBoolField(QString &declaration, QString fieldString);
};
