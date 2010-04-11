#pragma once
#include <QList>
#include <QString>
#include <QPoint>

typedef QList<QPoint> PointVector;

typedef QList<PointVector> PathVector;

struct Entity
{
public:
    QString name;
    PathVector components;
};

typedef QList<Entity> EntityVector;

struct KeyObjectItem
{
public:
    QString object;
    QList<QPoint> correctPath;
    QString key;
};

typedef QList<KeyObjectItem> KeyObjectItems;
