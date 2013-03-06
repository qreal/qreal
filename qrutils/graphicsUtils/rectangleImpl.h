#pragma once

#include <QtGui/QPainter>
#include <QtGui/QImage>

#include "../utilsDeclSpec.h"

namespace graphicsUtils
{

class QRUTILS_EXPORT RectangleImpl
{
public:
	RectangleImpl();
	QPainterPath shape(qreal x1, qreal y1, qreal x2, qreal y2, const int drift) const;
	QRectF boundingRect(qreal x1, qreal y1, qreal x2, qreal y2, const int scalingDrift) const;
	void drawRectItem(QPainter *painter, qreal x1, qreal y1, qreal x2, qreal y2);
	void drawEllipseItem(QPainter *painter, qreal x1, qreal y1, qreal x2, qreal y2);
	void drawImageItem(QPainter *painter, qreal x1, qreal y1, qreal x2, qreal y2, QImage const &image);
	void drawImageItemWithMirrored(QPainter *painter, qreal x1, qreal y1, qreal x2, qreal y2, QImage const &myImage);

private:
	QRectF calcRect(qreal x1, qreal y1, qreal x2, qreal y2) const;
};

}
