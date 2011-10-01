#pragma once
#include <QtGui/QPainter>
#include <QtGui/QImage>
#include "../utilsDeclSpec.h"

namespace graphicsUtils
{
class QRUTILS_EXPORT PointImpl
{
public:
	PointImpl();
	QRectF boundingRect(qreal x, qreal y, qreal radius, const int scalingDrift) const;
	void drawItem(QPainter* painter, qreal x, qreal y, qreal radius);
	void drawFieldForResizeItem(QPainter* painter);
};
}

