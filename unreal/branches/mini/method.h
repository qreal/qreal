#include <QList>
#include <QtCore/QString>
#include <QtCore/QStringList>

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
