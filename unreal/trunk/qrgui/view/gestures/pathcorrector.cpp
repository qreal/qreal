#include "pathcorrector.h"
#include "math.h"
#include <QPointF>

const double mouseSpeed = 5;

QList<QPointF> PathCorrector::getMousePath(QList<QPointF> const & path)
{
    QList<QPointF> mousePath;
    if (path.isEmpty())
        return mousePath;
    mousePath.append(path[0]);
    for (int i = 1; i < path.size(); i++)
    {
        QPointF currentPoint = path[i];
        QPointF previousPoint = path[i - 1];
        int length = currentPoint.x() - previousPoint.x();
        int width = currentPoint.y() - previousPoint.y();
        int number = static_cast<int>((sqrt(pow(length, 2) + pow(width, 2))) / mouseSpeed + 1);
        for (int j = 1; j <= number; j++)
        {
            QPointF point(int(previousPoint.x() + length * j / number),
                         int(previousPoint.y() + width * j / number));
            mousePath.append(point);
        }
    }
    return mousePath;
}

QList<QPointF> PathCorrector::correctPath(QList<QPointF> const & path)
{
    QList<QPointF> newPath;
    if (path.isEmpty())
        return newPath;
    QPointF previousPoint = path[0];
    for (int i = 1; i < path.size(); i++)
    {
        QPointF currentPoint = path[i];
        double speed = (currentPoint - previousPoint).manhattanLength();
        double b = sense * (1 - 1 / exp(speedKoef * speed));
        previousPoint.setX((int) (b * currentPoint.x() + (1 - b) * previousPoint.x()));
        previousPoint.setY((int) (b * currentPoint.y() + (1 - b) * previousPoint.y()));
        newPath.push_back(previousPoint);
    }
    return newPath;
}
