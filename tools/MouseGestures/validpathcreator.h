#pragma once
#include "GeometricForms.h"
#include <QList>
#include <QPoint>

// по набору линий создает "идеальный путь" (рисуем объект одним росчерком, если это возможно)
class ValidPathCreator
{
public:
    ValidPathCreator();
    QList<QPoint> createPath(PathVector const & curves);

private:
    QList<QPoint> merge(PointVector path1, PointVector path2, int distance);
    PointVector mergeNotCycles (PointVector path1, PointVector path2, int distance);
    bool isCycle(PointVector const & path);
    QList<QPoint> connectComponents(PathVector paths);
    double getDistance(QList<QPoint> const &path1, QList<QPoint> const &path2);
    double getDistance(QList<QPoint> const &path, QPoint const &point);
    QList<QPoint> pushBackPath(QList<QPoint> mainPath, QList<QPoint> const &path, int begin, int end);
};
