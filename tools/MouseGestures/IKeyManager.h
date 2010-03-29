#pragma once
#include <QString>
#include <QPoint>
#include <QList>

class IKeyManager
{
public:
    virtual QString getKey(QList<QPoint> const & path) = 0;
};
