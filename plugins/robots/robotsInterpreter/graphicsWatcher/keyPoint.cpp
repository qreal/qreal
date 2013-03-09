#include "keyPoint.h"

using namespace qReal::interpreters::robots::sensorsGraph;

KeyPoint::KeyPoint() : mVisualArea(QRectF(-10, -10, 10, 10))
{
	setCacheMode(DeviceCoordinateCache);
	setZValue(1000);
}

QRectF KeyPoint::boundingRect() const
{
	return mVisualArea;
}

void KeyPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)

	painter->setPen(Qt::NoPen);

	QRadialGradient gradient(0, 0, 10);

	gradient.setCenter(0, 0);
	gradient.setFocalPoint(0, 0);
	gradient.setColorAt(1, QColor(Qt::yellow).light(120));
	gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
	painter->setBrush(gradient);

	painter->setPen(Qt::NoPen);
	painter->drawEllipse(mVisualArea);
}
