#pragma once
#include "GeometricForms.h"
#include <QList>
#include <QPoint>

typedef QList<QPoint> PointVector;
typedef QList<PointVector> PathVector;

// по набору линий создает "идеальный путь" (рисуем объект одним росчерком, если это возможно)
class ValidPathCreator
{
public:
    ValidPathCreator();
    QList<QPoint> createPath(Graph const & lines);

private:
    bool canMerge(PointVector const & path1, PointVector const & path2);
    QList<QPoint> merge(PointVector path1, PointVector path2);
    PointVector mergeNotCycles (PointVector path1, PointVector path2);
    PointVector lineToPath(Line const & line);
    bool isCycle(PointVector const & path);
};
