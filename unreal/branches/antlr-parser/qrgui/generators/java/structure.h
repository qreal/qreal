#include "attribute.h"
#include "method.h"

#include <QtCore/QTextStream>
#include <QList>

class structure
{
public:
    QString type;
    QString name;
    QList<attribute> attributes;
    QList<method> methods;
};
