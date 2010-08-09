#pragma once
#include <QString>
#include <QPointF>
#include <QList>

class IKeyManager
{
public:
    virtual QString getKey(QList<QPointF> const & path) = 0;
};
