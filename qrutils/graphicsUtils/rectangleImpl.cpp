#include "rectangleImpl.h"

using namespace graphicsUtils;

RectangleImpl::RectangleImpl()
{
}

QRectF RectangleImpl::boundingRect(qreal x1, qreal y1, qreal x2, qreal y2, const int scalingDrift) const
{
	return QRectF(qMin(x1, x2), qMin(y1, y2), abs(x2 - x1), abs(y2 - y1)).adjusted(-scalingDrift, -scalingDrift, scalingDrift, scalingDrift);
}

QRectF RectangleImpl::calcRect(qreal x1, qreal y1, qreal x2, qreal y2)
{
	if(x2 > x1) {
		if (y2 > y1)
			return QRectF(x1, y1, x2 - x1, y2 - y1);
		else
			return QRectF(x1, y2, x2 - x1, y1 - y2);
	} else {
		if (y2 > y1)
			return QRectF(x2, y1, x1 - x2, y2 - y1);
		else
			return QRectF(x2, y2, x1 - x2, y1 - y2);
	}
}

void RectangleImpl::drawRectItem(QPainter* painter, qreal x1, qreal y1, qreal x2, qreal y2)
{
	painter->drawRect(calcRect(x1, y1, x2, y2));
}

void RectangleImpl::drawEllipseItem(QPainter* painter, qreal x1, qreal y1, qreal x2, qreal y2)
{
	painter->drawEllipse(calcRect(x1, y1, x2, y2));
}

void RectangleImpl::drawImageItemWithMirrored(QPainter* painter, qreal x1, qreal y1, qreal x2, qreal y2, QImage myImage)
{
	QImage image(myImage);
	if(x2 > x1) {
		if (y2 < y1)
			image = myImage.mirrored(false, true);
	} else {
		if (y2 > y1)
			image = myImage.mirrored(true, false);
		else
			image = myImage.mirrored(true, true);
	}
	painter->drawImage(QRectF(qMin(x1, x2), qMin(y1, y2), abs(x2 - x1), abs(y2 - y1)), image);
}

void RectangleImpl::drawImageItem(QPainter* painter, qreal x1, qreal y1, qreal x2, qreal y2, QImage myImage)
{
	painter->drawImage(QRectF(qMin(x1, x2), qMin(y1, y2), abs(x2 - x1), abs(y2 - y1)), myImage);
}
