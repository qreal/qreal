#include <QList>
#include <QtCore/QString>

class attribute
{
public:
    QString visibility;
    bool isStatic;
    bool isFinal;
    bool isVolatile;
    bool isTransient;
    QString type;
    QString name;
    QString defaultValue;
};
