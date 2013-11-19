#include "vectorUtils.h"

using namespace mathUtils;

qreal VectorUtils::vectorProduct(QVector2D const &vector1, QVector2D const &vector2)
{
	return vector1.x() * vector2.y() - vector1.y() * vector2.x();
}

qreal VectorUtils::scalarProduct(QVector2D const &vector1, QVector2D const &vector2)
{
	return vector1.x() * vector2.x() + vector1.y() * vector2.y();
}
