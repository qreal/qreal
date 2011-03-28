#include "adopter.h"
#include <QList>
#include <QPoint>
#include <QStringList>

const QString comma = ", ";
const QString pointDelimeter = " : ";
const QString pathDelimeter = " | ";

QList<QList<QPoint> > Parser::stringToPath(QString const &valueStr)
{
    QList<QList<QPoint> > result;
    QStringList paths = valueStr.split(pathDelimeter, QString::SkipEmptyParts);
    foreach(QString pathStr, paths)
    {
        QStringList points = pathStr.split(pointDelimeter, QString::SkipEmptyParts);
        QList<QPoint> path;
        foreach (QString str, points)
        {
            QPoint point = parsePoint(str);
            path.push_back(point);
        }
        result.push_back(path);
    }
    return result;
}

QString Parser::pathToString(QList<QList<QPoint> > const &gesture)
{
    QString result = "";
    foreach (QList<QPoint> path, gesture)
    {
        foreach (QPoint point, path)
        {
            result += pointToString(point) + pointDelimeter;
        }
        result += pathDelimeter;
    }
    return result;
}


QPoint Parser::parsePoint(QString const &str)
{
    bool isInt;
    int x = str.section(comma, 0, 0).toInt(&isInt, 0);
    int y = str.section(comma, 1, 1).toInt(&isInt, 0);
    return QPoint(x, y);
}

QString Parser::pointToString(QPoint const &p)
{
    return QString("%1").arg(p.x()) + comma + QString("%1").arg(p.y());
}
