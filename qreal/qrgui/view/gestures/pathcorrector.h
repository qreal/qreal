#pragma once
#include <QList>
#include <QPointF>

const double sense = 1;
const double speedKoef = 0.0275;

class PathCorrector
{
public:
	static QList<QPoint> correctPath(QList<QPoint> const & path);
	static QList<QPoint> getMousePath(QList<QPoint> const & path);
};
