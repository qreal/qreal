#include "pointImpl.h"

using namespace graphicsUtils;

PointImpl::PointImpl()
{
}

QRectF PointImpl::boundingRect(qreal x, qreal y, qreal radius, const int scalingDrift) const
{
	return QRectF(x - radius, y - radius, 2 * radius, 2 * radius).adjusted(-scalingDrift, -scalingDrift, scalingDrift, scalingDrift);
}

void PointImpl::drawItem(QPainter* painter, qreal x, qreal y, qreal radius)
{
	painter->drawEllipse(QRectF(x - radius, y - radius, 2 * radius, 2 * radius));
}

void PointImpl::drawFieldForResizeItem(QPainter* painter)
{
	Q_UNUSED(painter);
}
