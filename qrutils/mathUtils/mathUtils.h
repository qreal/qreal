#pragma once

#include <QtCore/QRectF>

#include "../utilsDeclSpec.h"

namespace mathUtils
{

class MathUtils
{
public:
	static inline qreal sqr(qreal x);
	static qreal distance(QPointF const &point1, QPointF const &point2);
	static qreal distance(QRectF const &rectangle, QPointF const &point);
};

}
