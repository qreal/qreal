#include "statPoint.h"

StatPoint::StatPoint(QPointF const &point, bool propX, bool propY, int initWidth, int initHeight, PortImpl *impl)
	: mPoint(point), mPropX(propX), mPropY(propY), mInitWidth(initWidth), mInitHeight(initHeight), mImpl(impl)
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

	painter->save();
	QPen pen = painter->pen();
	pen.setColor(QColor("#c3dcc4"));
	pen.setWidth(12);
	painter->setPen(pen);
	painter->drawLine(pointToDraw);
	pen.setColor(QColor("#465945"));
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

QString StatPoint::type() const
{
	return mImpl->type();
}
