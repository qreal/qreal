#include "pathcorrector.h"
#include "math.h"
#include <QPoint>


PathCorrector::PathCorrector()
{
}

PathCorrector::PathCorrector(QList<QPoint> const & path)
{
    this->mPath = path;
}

QList<QPoint> PathCorrector::correctPath()
{
    QList<QPoint> newPath;
    if (mPath.size() <= 0)
    {
        return newPath;
    }
    QPoint previousPoint = mPath[0];
    for (int i = 1; i < mPath.size(); i++)
    {
        QPoint currentPoint = mPath[i];
        double speed = sqrt(pow(currentPoint.x() - previousPoint.x(), 2) + pow(currentPoint.y() - previousPoint.y(), 2));
        double b = sense * (1 - 1 / exp(speedKoef * speed));
        previousPoint.setX((int) (b * currentPoint.x() + (1 - b) * previousPoint.x()));
        previousPoint.setY((int) (b * currentPoint.y() + (1 - b) * previousPoint.y()));
        newPath.push_back(previousPoint);
    }
    return newPath;
}
