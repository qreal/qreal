#pragma once
#include <QList>
#include <QString>
#include <QPoint>

struct Line
{
public:
    QPoint point1;
    QPoint point2;
};

typedef QList<Line> Graph;

struct Entity
{
public:
    QString name;
    Graph lines;
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
