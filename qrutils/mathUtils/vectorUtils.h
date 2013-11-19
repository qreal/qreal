#pragma once

#include <QtGui/QVector2D>

namespace mathUtils {

class VectorUtils
{
public:
	static qreal scalarProduct(QVector2D const &vector1, QVector2D const &vector2);
	static qreal vectorProduct(QVector2D const &vector1, QVector2D const &vector2);

};

}
