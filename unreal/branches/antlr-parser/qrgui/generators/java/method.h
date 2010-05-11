#include <QList>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>

class method
{
public:
    QString visibility;
    bool isAbstract;
    bool isStatic;
    bool isFinal;
    bool isSynchronized;
    bool isNative;
    QString type;
    QString name;
    QStringList operationFactors;
};
