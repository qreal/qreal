#include "adopter.h"
#include "math.h"
#include <QStringList>

const QString comma = ", ";
const QString pointDelimeter = " : ";

QString Adopter::pointToString(QPoint const &p)
{
    return QString("%1").arg(p.x()) + comma + QString("%1").arg(p.y());
}

QString Adopter::pathToString(QList<QPoint> const &path)
{
    QString result = "";
    foreach (QPoint point, path)
    {
        result += pointToString(point) + pointDelimeter;
    }
    return result;
}

QList<QPoint> Adopter::stringToPath(QString const &valueStr)
{
    QStringList points = valueStr.split(pointDelimeter, QString::SkipEmptyParts);
    QList<QPoint> result;
    foreach (QString str, points)
    {
        QPoint point = parsePoint(str);
        result.push_back(point);
    }
    return result;
}

QPoint Adopter::parsePoint(QString const &str)
{
    bool isInt;
    int x = str.section(comma, 0, 0).toInt(&isInt, 0);
    int y = str.section(comma, 1, 1).toInt(&isInt, 0);
    return QPoint(x, y);
}
