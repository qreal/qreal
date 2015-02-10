#include "statPoint.h"

StatPoint::StatPoint(const QPointF &point, bool propX, bool propY, int initWidth, int initHeight, PortImpl *impl)
	: PortInterface(impl), mPoint(point), mPropX(propX), mPropY(propY), mInitWidth(initWidth), mInitHeight(initHeight)
{}

StatPoint::operator QPointF () const
{
	return mPoint;
}

void StatPoint::operator= (const QPointF &p)
{
	mPoint = p;
	mPropX = false;
	mPropY = false;
	mInitHeight = 1;
	mInitWidth = 1;
}

void StatPoint::paint(QPainter *painter, const QRectF &contents) const
{
	const qreal x = mPoint.x() * (mPropX ? mInitWidth : contents.width());
	const qreal y = mPoint.y() * (mPropY ? mInitHeight : contents.height());

	QLineF pointToDraw(x - 0.1, y - 0.1, x + 0.1, y + 0.1);

	const QColor portColor("#465945");
	const QColor highlightColor("#c3dcc4");

	painter->save();
	QPen pen = painter->pen();
	pen.setColor(highlightColor);
	pen.setWidth(12);
	painter->setPen(pen);
	painter->drawLine(pointToDraw);
	pen.setColor(portColor);
	pen.setWidth(3);
	painter->setPen(pen);
	painter->drawLine(pointToDraw);
	painter->restore();
}

QPointF StatPoint::transformForContents(const QRectF &contents) const
{
	const qreal x = mPoint.x() * (mPropX ? mInitWidth : contents.width());
	const qreal y = mPoint.y() * (mPropY ? mInitHeight : contents.height());

	return QPointF(x, y);
}
