#pragma once
#include <QString>
#include <QList>
#include <QPoint>

class IGesturesPainter
{
public:
    virtual QString currentElement() = 0;
    virtual void draw(QList<QPoint> const & path) = 0;
    virtual void setElements(QList<QString> const & elements) = 0;
};
