#pragma once

#include <QtCore/QLineF>
#include <QtGui/QVector2D>
#include <QtGui/QPainterPath>

namespace mathUtils {

class Geometry
{
public:
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

	/// Returns if given line intersects given painter path
	static bool intersects(QLineF const &line, QPainterPath const &path);
};

}
