#include <QList>
#include <QString>

#include "structure.h"

class Package
{
public:
    QString name;
    QList<Package> packages;
    QList<Structure> structures;
};
