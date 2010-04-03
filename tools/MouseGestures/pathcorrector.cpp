#include "pathcorrector.h"
#include "math.h"
#include <QPoint>


QList<QPoint> PathCorrector::correctPath(QList<QPoint> const & path)
{
    QList<QPoint> newPath;
    if (path.size() <= 0)
    {
        return newPath;
    }
    QPoint previousPoint = path[0];
    for (int i = 1; i < path.size(); i++)
    {
        QPoint currentPoint = path[i];
        double speed = sqrt(pow(currentPoint.x() - previousPoint.x(), 2) + pow(currentPoint.y() - previousPoint.y(), 2));
        double b = sense * (1 - 1 / exp(speedKoef * speed));
        previousPoint.setX((int) (b * currentPoint.x() + (1 - b) * previousPoint.x()));
        previousPoint.setY((int) (b * currentPoint.y() + (1 - b) * previousPoint.y()));
        newPath.push_back(previousPoint);
    }
    return newPath;
}
