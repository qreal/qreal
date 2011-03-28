#pragma once
#include <QList>
#include <QPoint>
#include <QString>

class Parser
{
public:
    static QString pathToString(QList<QList<QPoint> > const &path);
    static QList<QList<QPoint> > stringToPath(QString const &str);

private:
    static QPoint parsePoint(QString const &str);
    static QString pointToString(QPoint const &p);
};
