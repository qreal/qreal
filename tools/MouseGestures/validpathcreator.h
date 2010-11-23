#pragma once
#include "GeometricForms.h"
#include <QList>
#include <QPoint>

// по набору линий создает "идеальный путь" (рисуем объект одним росчерком, если это возможно)
class ValidPathCreator
{
public:
	static QList<QPoint> createPath(PathVector const & curves);

private:
	static QList<QPoint> merge(PointVector const &path1, PointVector const &path2, int distance);
	static QList<QPoint> mergeNotCycles(PointVector const &path1, PointVector const &path2, int distance);
	static QList<QPoint> mergeGraphCycle(PointVector const &cycle, PointVector const &graph, int cyclePos, int graphPos);
	static bool isCycle(PointVector const & path);
	static QList<QPoint> connectComponents(PathVector const &components);
	static double getDistance(QList<QPoint> const &path1, QList<QPoint> const &path2);
	static double getDistance(QList<QPoint> const &path, QPoint const &point);
	static QList<QPoint> pushBackPath(QList<QPoint> mainPath, QList<QPoint> const &path, int begin, int end);
	static QList<QPoint> pushBackPoint(QList<QPoint> const &path, QPoint const &point);
};
