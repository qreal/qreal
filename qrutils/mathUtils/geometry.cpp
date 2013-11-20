#include "geometry.h"
#include "math.h"

using namespace mathUtils;

qreal Geometry::vectorProduct(QVector2D const &vector1, QVector2D const &vector2)
{
	return vector1.x() * vector2.y() - vector1.y() * vector2.x();
}

qreal Geometry::scalarProduct(QVector2D const &vector1, QVector2D const &vector2)
{
	return vector1.x() * vector2.x() + vector1.y() * vector2.y();
}

QVector2D Geometry::projection(QVector2D const &projected, QVector2D const &target)
{
	QVector2D const normalizedTarget = target.normalized();
	// Scalar product is a projection lenght
	return normalizedTarget * scalarProduct(normalizedTarget, projected);
}

QPointF Geometry::normalPoint(QLineF const &line, QPointF const &point)
{
	qreal const x1 = line.p1().x();
	qreal const y1 = line.p1().y();
	qreal const x2 = line.p2().x();
	qreal const y2 = line.p2().y();
	qreal const x3 = point.x();
	qreal const y3 = point.y();

	if (x1 == x2) {
		return QPointF(x2, y3);
	}

	qreal const x = (x1 * Math::sqr(y2 - y1) + x3 * Math::sqr(x2 - x1) + (x2 - x1) * (y2 - y1) * (y3 - y1))
			/ (Math::sqr(y2 - y1) + Math::sqr(x2 - x1));
	qreal const y = ((y2 - y1) * (x - x1) / (x2 - x1)) + y1;

	return QPointF(x, y);
}

QLineF Geometry::normalLine(QLineF const &line, QPointF const &point)
{
	return QLineF(point, normalPoint(line, point));
}

qreal Geometry::distance(QLineF const &line, QPointF const &point)
{
	return normalLine(line, point).length();
}

bool Geometry::intersects(QLineF const &line, QPainterPath const &path)
{
	QPainterPath linePath(line.p1());
	linePath.lineTo(line.p2());
	return path.intersects(linePath);
}
