#pragma once
#include <QString>
#include <QPointF>
#include <QList>

class IKeyManager
{
public:
    virtual QString getKey(QList<QPoint> const & path) = 0;
};
