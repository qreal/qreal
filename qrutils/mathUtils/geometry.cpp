#include "geometry.h"

using namespace mathUtils;

bool Geometry::eq(QPointF const &point1, QPointF const &point2, qreal eps)
{
	return Math::eq(point1.x(), point2.x(), eps) && Math::eq(point1.y(), point2.y(), eps);
}

qreal Geometry::scalarProduct(QVector2D const &vector1, QVector2D const &vector2)
{
	return vector1.x() * vector2.x() + vector1.y() * vector2.y();
}

qreal Geometry::vectorProduct(QVector2D const &vector1, QVector2D const &vector2)
{
	return vector1.x() * vector2.y() - vector1.y() * vector2.x();
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

qreal Geometry::distance(QPointF const &point1, QPointF const &point2)
{
	return sqrt(Math::sqr(point1.x() - point2.x()) + Math::sqr(point1.y() - point2.y()));
}

bool Geometry::intersects(QLineF const &line, QPainterPath const &path)
{
	QPainterPath linePath(line.p1());
	linePath.lineTo(line.p2());
	return path.intersects(linePath);
}

QVector2D Geometry::directionVector(qreal angleInDegrees)
{
	return directionVectorRad(angleInDegrees * pi / 180);
}

QVector2D Geometry::directionVectorRad(qreal angleInRadians)
{
	return QVector2D(cos(angleInRadians), sin(angleInRadians));
}

qreal Geometry::tangentLineAt(QPainterPath const &path, QPointF const &point)
{
	qreal const percentage = percentageAt(path, point);
	return path.angleAtPercent(percentage);
}

qreal Geometry::percentageAt(QPainterPath const &path, QPointF const &point)
{
	// Approximating percentage iteratively. First iterating by delta=1 percent increment, searching for
	// the closest point on path. Then repeating with found closest percentage value but with delta=0.1 and so on
	int const approximationLevel = 4;
	qreal result = 0.5;
	qreal delta = 0.01;

	for (int approximationIteration = 0; approximationIteration < approximationLevel; ++approximationIteration) {
		qreal minimalPercentage, minimalDistance = 10e10;
		for (int i = -50; i <= 50; ++i) {
			qreal const percentage = result + delta * i;
			QPointF const pointAtPercentage = percentage >= 0 && percentage <= 1
					? path.pointAtPercent(percentage)
					: QPointF(10e5, 10e5);
			qreal const distanceAtPercentage = distance(pointAtPercentage, point);
			if (distanceAtPercentage < minimalDistance) {
				minimalDistance = distanceAtPercentage;
				minimalPercentage = percentage;
			}
		}

		result = minimalPercentage;
		delta /= 10;
	}

	return result;
}

QList<QPointF> Geometry::pathToPoints(QPainterPath const &path)
{
	qreal const polygonIsPointCondition = 0.001;

	QList<QPolygonF> polygons = path.toFillPolygons();
	QList<QPointF> result;

	foreach (QPolygonF const &polygon, polygons) {
		if (square(polygon) < polygonIsPointCondition) {
			result << polygon[0];
		} else {
			return QList<QPointF>();
		}
	}

	return result;
}

qreal Geometry::square(QPolygonF const &polygon)
{
	qreal result = 0;
	for (int i = 0; i < polygon.size(); ++i) {
		QPointF const p1 = i ? polygon[i-1] : polygon.back();
		QPointF const p2 = polygon[i];
		result += (p1.x() - p2.x()) * (p1.y() + p2.y());
	}

	return fabs(result) / 2;
}

QLineF Geometry::veryLongLine(QPointF const &pointOnLine, QVector2D const &directionVector)
{
	qreal const halfLength = 10000;
	return QLineF(pointOnLine + halfLength * directionVector.toPointF()
			, pointOnLine - halfLength * directionVector.toPointF());
}

QList<QPointF> Geometry::intersection(QLineF const &line, QPainterPath const &path, qreal eps)
{
	QList<QPointF> result;
	QPointF startPoint;
	QPointF endPoint;

	for (int i = 0; i < path.elementCount(); ++i) {
		QPainterPath::Element const element = path.elementAt(i);

		// Checking that element belongs to the wall path
		if (element.isMoveTo()) {
			endPoint = QPointF(element.x, element.y);
			continue;
		}

		startPoint = endPoint;
		endPoint = QPointF(element.x, element.y);
		QLineF currentLine(startPoint, endPoint);
		QPointF intersectionPoint;
		// TODO: consider curve cases
		if (line.intersect(currentLine, &intersectionPoint) != QLineF::NoIntersection
				&& belongs(intersectionPoint, currentLine, eps))
		{
			result << intersectionPoint;
		}
	}

	return result;
}

QPointF Geometry::closestPointTo(QList<QPointF> const &points, QPointF const &point)
{
	qreal minDistance = 10e10;
	QPointF closestPoint;
	foreach (QPointF const &currentPoint, points) {
		qreal const currentDistance = distance(currentPoint, point);
		if (currentDistance < minDistance) {
			closestPoint = currentPoint;
			minDistance = currentDistance;
		}
	}

	return closestPoint;
}

bool Geometry::belongs(QPointF const &point, QLineF const &segment, qreal eps)
{
	if (!Math::between(segment.x1(), segment.x2(), point.x(), eps)
			|| !Math::between(segment.y1(), segment.y2(), point.y(), eps))
	{
		return false;
	}

	if (eq(point, segment.p1(), eps) || eq(point, segment.p2(), eps)) {
		return true;
	}

	if (Math::eq(segment.x1(), segment.x2(), eps)) {
		return Math::eq(point.x(), segment.x1(), eps);
	}

	return Math::eq(QLineF(segment.p1(), point).angle(), QLineF(point, segment.p2()).angle(), eps);
}

bool Geometry::belongs(QPointF const &point, QPainterPath const &path, qreal eps)
{
	QPointF startPoint;
	QPointF endPoint;

	for (int i = 0; i < path.elementCount(); ++i) {
		QPainterPath::Element const element = path.elementAt(i);

		// Checking that element belongs to the wall path
		if (element.isMoveTo()) {
			endPoint = QPointF(element.x, element.y);
			continue;
		}

		startPoint = endPoint;
		endPoint = QPointF(element.x, element.y);
		// TODO: consider curve cases
		if (belongs(point, QLineF(startPoint, endPoint)), eps) {
			return true;
		}
	}

	return false;
}

bool Geometry::belongs(QLineF const &line, QPainterPath const &path, qreal eps)
{
	int const pointsToCheck = 5;
	QVector2D const shift = QVector2D(line.p2() - line.p1()) / (pointsToCheck - 1);
	for (int i = 0; i < pointsToCheck; ++i) {
		if (!belongs(line.p1() + i * shift.toPointF(), path), eps) {
			return false;
		}
	}

	return true;
}
