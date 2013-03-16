#include <math.h>

#include "mathUtils.h"

using namespace mathUtils;

qreal MathUtils::sqr(qreal x)
{
	return x * x;
}

qreal MathUtils::distance(QPointF const &point1, QPointF const &point2)
{
	return sqrt(sqr(point1.x() - point2.x()) + sqr(point1.y() - point2.y()));
}

qreal MathUtils::distance(QRectF const &rectangle, QPointF const &point)
{
	if (rectangle.contains(point)) {
		return 0;
	}
	// if point between horizontal edges
	if (point.y() >= rectangle.top() && point.y() <= rectangle.bottom()) {
		return qMin(qAbs(rectangle.left() - point.x())
				, qAbs(rectangle.right() - point.x()));
	}
	// if point between vertical edges
	if (point.x() >= rectangle.left() && point.x() <= rectangle.right()) {
		return qMin(qAbs(rectangle.left() - point.x())
				, qAbs(rectangle.right() - point.x()));
	}

	return qMin(
			qMin(distance(point, rectangle.topLeft())
					, distance(point, rectangle.topRight()))
			, qMin(distance(point, rectangle.bottomRight())
				, distance(point, rectangle.bottomRight()))
		);
}
