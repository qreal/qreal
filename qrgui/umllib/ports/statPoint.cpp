#include "statPoint.h"

StatPoint::StatPoint(QPointF const &point, bool propX, bool propY, int initWidth, int initHeight, PortImpl *impl)
	: PortInterface(impl), mPoint(point), mPropX(propX), mPropY(propY), mInitWidth(initWidth), mInitHeight(initHeight)
{}

StatPoint::operator QPointF () const
{
	return mPoint;
}

void StatPoint::operator= (QPointF const &p)
{
	mPoint = p;
	mPropX = false;
	mPropY = false;
	mInitHeight = 1;
	mInitWidth = 1;
}

void StatPoint::paint(QPainter *painter, QRectF const &contents) const
{
	qreal const x = mPoint.x() * (mPropX ? mInitWidth : contents.width());
	qreal const y = mPoint.y() * (mPropY ? mInitHeight : contents.height());

	QLineF pointToDraw(x - 0.1, y - 0.1, x + 0.1, y + 0.1);

	QColor const portColor("#465945");
	QColor const highlightColor("#c3dcc4");

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

QPointF StatPoint::transformForContents(QRectF const &contents) const
{
	qreal const x = mPoint.x() * (mPropX ? mInitWidth : contents.width());
	qreal const y = mPoint.y() * (mPropY ? mInitHeight : contents.height());

	return QPointF(x, y);
}
