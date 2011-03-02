#pragma once
#include "IKeyManager.h"
#include <QPoint>
#include <QString>
#include <QList>

class Key8Manager : public IKeyManager
{
public:
    Key8Manager();
    QString getKey(QList<QPoint> const & path);

private:
    static const double pi = 3.141592;
};
