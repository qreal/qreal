/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtCore/QLineF>
#include <QtCore/QList>
#include <QtGui/QVector2D>
#include <QtGui/QPainterPath>

#include <qrutils/utilsDeclSpec.h>
#include "math.h"

namespace mathUtils {

/// Contains some useful methods for working with vectors, lines, paths, etc.
class QRUTILS_EXPORT Geometry
{
public:
	/// Compares given points with the given precision
	static bool eq(const QPointF &point1, const QPointF &point2, qreal eps = EPS);

	/// Returns a scalar product of two given vectors
	static qreal scalarProduct(const QVector2D &vector1, const QVector2D &vector2);

	/// Returns a module of the vector product of two given vectors
	static qreal vectorProduct(const QVector2D &vector1, const QVector2D &vector2);

	/// Returns a vector that corresponds to a projection of vector 'projection' on a vector 'target'
	static QVector2D projection(const QVector2D &projected, const QVector2D &target);

	/// Performs vector rotation and returns the resulting vector.
	/// Vector is rotated by multiplying the rotation matrix.
	static QVector2D rotateVector(const QVector2D &vector, qreal angleInDegrees);

	/// Returns an intersection point of the given line and a normal to a given line passing through the given point
	static QPointF normalPoint(const QLineF &line, const QPointF &point);

	/// Returns a normal to a given line passing through the given point
	static QLineF normalLine(const QLineF &line, const QPointF &point);

	/// Returns a distance from a given point to a given line
	static qreal distance(const QLineF &line, const QPointF &point);

	/// Returns a distance between two points
	static qreal distance(const QPointF &point1, const QPointF &point2);

	/// Returns if given line intersects given painter path
	static bool intersects(const QLineF &line, const QPainterPath &path);

	/// Returns radius-vector with given rotation angle in degrees
	static QVector2D directionVector(qreal angleInDegrees);

	/// Returns radius-vector with given rotation angle in radians
	static QVector2D directionVectorRad(qreal angleInRadians);

	/// Returns a tangent line slope
	static qreal tangentLineAt(const QPainterPath &path, const QPointF &point);

	/// Returns a percentage on a given path for a given point
	static qreal percentageAt(const QPainterPath &path, const QPointF &point);

	/// Checks if given path consists of points (very short lines) and if
	/// it is returns this points list
	static QList<QPointF> pathToPoints(const QPainterPath &path);

	/// Calculates a square of given polygon. Complexity is O(n)
	static qreal square(const QPolygonF &polygon);

	/// Returns QLineF instance with very big length for simulating the real line
	static QLineF veryLongLine(const QPointF &pointOnLine, const QVector2D &directionVector);

	/// Returns a list of points in whitch the given line intersects the given path with the given precision
	static QList<QPointF> intersection(const QLineF &line, const QPainterPath &path, qreal eps = EPS);

	/// Returns the point in the given list that is closest to the given point
	static QPointF closestPointTo(const QList<QPointF> &points, const QPointF &point);

	/// Returns if the given point belongs to the given segment with the given precision.
	static bool belongs(const QPointF &point, const QLineF &segment, qreal eps = EPS);

	/// Returns if the given point belongs to the given path with the given precision.
	static bool belongs(const QPointF &point, const QPainterPath &path, qreal eps = EPS);

	/// Returns if the given line belongs to the given path with the given precision.
	static bool belongs(const QLineF &line, const QPainterPath &path, qreal eps = EPS);
};

}
