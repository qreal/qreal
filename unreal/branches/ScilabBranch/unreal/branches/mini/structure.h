#include <QList>
#include <QtCore/QString>

#include "attribute.h"
#include "method.h"

class structure
{
public:
    QString type;
    QString name;
    QList<attribute> attributes;
    QList<method> methods;
};
