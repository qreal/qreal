#pragma once

#include <QtCore/QLineF>
#include <QtCore/QList>
#include <QtGui/QVector2D>
#include <QtGui/QPainterPath>

#include <qrutils/utilsDeclSpec.h>
#include "math.h"

namespace utils {

/// Contains some useful methods for working with vectors, lines, pathes, etc.
class QRUTILS_EXPORT Geometry
{
public:
	/// Compares given points with the given precision
	static bool eq(QPointF const &point1, QPointF const &point2, qreal eps = EPS);

	/// Returns a scalar product of two given vectors
	static qreal scalarProduct(QVector2D const &vector1, QVector2D const &vector2);

	/// Returns a module of the vector product of two given vectors
	static qreal vectorProduct(QVector2D const &vector1, QVector2D const &vector2);

	/// Returns a vector that corresponds to a projection of vector 'projection' on a vector 'target'
	static QVector2D projection(QVector2D const &projected, QVector2D const &target);

	/// Returns an intersection point of the given line and a normal to a given line passing through the given point
	static QPointF normalPoint(QLineF const &line, QPointF const &point);

	/// Returns a normal to a given line passing through the given point
	static QLineF normalLine(QLineF const &line, QPointF const &point);

	/// Returns a distance from a given point to a given line
	static qreal distance(QLineF const &line, QPointF const &point);

	/// Returns a distance between two points
	static qreal distance(QPointF const &point1, QPointF const &point2);

	/// Returns if given line intersects given painter path
	static bool intersects(QLineF const &line, QPainterPath const &path);

	/// Returns radius-vector with given rotation angle in degrees
	static QVector2D directionVector(qreal angleInDegrees);

	/// Returns radius-vector with given rotation angle in radians
	static QVector2D directionVectorRad(qreal angleInRadians);

	/// Returns a tangent line slope
	static qreal tangentLineAt(QPainterPath const &path, QPointF const &point);

	/// Returns a percentage on a given path for a given point
	static qreal percentageAt(QPainterPath const &path, QPointF const &point);

	/// Checks if given path consists of points (very short lines) and if
	/// it is returns this points list
	static QList<QPointF> pathToPoints(QPainterPath const &path);

	/// Calculates a square of given polygon. Complexity is O(n)
	static qreal square(QPolygonF const &polygon);

	/// Returns QLineF instance with very big length for simulating the real line
	static QLineF veryLongLine(QPointF const &pointOnLine, QVector2D const &directionVector);

	/// Returns a list of points in whitch the given line intersects the given path with the given precision
	static QList<QPointF> intersection(QLineF const &line, QPainterPath const &path, qreal eps = EPS);

	/// Returns the point in the given list that is closest to the given point
	static QPointF closestPointTo(QList<QPointF> const &points, QPointF const &point);

	/// Returns if the given point belongs to the given segment with the given precision.
	static bool belongs(QPointF const &point, QLineF const &segment, qreal eps = EPS);

	/// Returns if the given point belongs to the given path with the given precision.
	static bool belongs(QPointF const &point, QPainterPath const &path, qreal eps = EPS);

	/// Returns if the given line belongs to the given path with the given precision.
	static bool belongs(QLineF const &line, QPainterPath const &path, qreal eps = EPS);
};

}
