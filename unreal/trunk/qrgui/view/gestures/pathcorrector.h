#pragma once
#include <QList>
#include <QPointF>

class PathCorrector
{
public:
    static QList<QPointF> correctPath(QList<QPointF> const & path);
    static QList<QPointF> getMousePath(QList<QPointF> const & path);

private:
    static const double sense = 1;
    static const double speedKoef = 0.0275;
};
