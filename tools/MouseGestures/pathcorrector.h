#pragma once
#include <QList>
#include <QPoint>

class PathCorrector
{
public:
    PathCorrector();
    PathCorrector(QList<QPoint> const & path);
    QList<QPoint> correctPath();
private:
    QList<QPoint> mPath;
    static const double sense = 1;
    static const double speedKoef = 0.0275;
};
