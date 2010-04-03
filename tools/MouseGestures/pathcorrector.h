#pragma once
#include <QList>
#include <QPoint>

class PathCorrector
{
public:
    static QList<QPoint> correctPath(QList<QPoint> const & path);

private:
    static const double sense = 1;
    static const double speedKoef = 0.0275;
};
