#include "sensorItem.h"
#include <QtGui/QCursor>
#include <QtGui/QGraphicsSceneMouseEvent>

#include <QDebug>

using namespace qReal::interpreters::robots;
using namespace details::d2Model;

int const size = 5;

SensorItem::SensorItem(SensorsConfiguration &configuration, inputPort::InputPortEnum port)
	: QGraphicsItem()
	, mConfiguration(configuration)
	, mPort(port)
	, mDragged(false)
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemClipsChildrenToShape |
		ItemClipsToShape | ItemSendsGeometryChanges);

	setAcceptHoverEvents(true);
	setAcceptDrops(true);
	setCursor(QCursor(Qt::PointingHandCursor));
	setZValue(1);
}

void SensorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *widget)
{
	Q_UNUSED(style)
	Q_UNUSED(widget)

	painter->save();

	QBrush brush;
	brush.setColor(color());
	brush.setStyle(Qt::SolidPattern);

	painter->setBrush(brush);
	painter->setOpacity(0.5);
	painter->setPen(color());
	painter->drawEllipse(boundingRect());

	painter->restore();
}

QRectF SensorItem::boundingRect() const
{
	return QRectF(-size, -size, size * 2, size * 2);
}

void SensorItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mousePressEvent(event);
	mDragged = true;
}

void SensorItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseMoveEvent(event);
	if (mDragged) {
		mConfiguration.setPosition(mPort, (event->scenePos() - mBasePos).toPoint());
		setPos(mBasePos + mConfiguration.position(mPort));
	}
}

void SensorItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseReleaseEvent(event);
	mDragged = false;
}

void SensorItem::setBasePosition(QPointF const &pos)
{
	mBasePos = pos;
	setPos(mBasePos + mConfiguration.position(mPort));
}

QColor SensorItem::color() const
{
	switch (mConfiguration.type(mPort)) {
	case sensorType::touchBoolean:
		return Qt::green;
	case sensorType::colorFull:
		return Qt::blue;
	case sensorType::sonar:
		return Qt::red;
	default:
		Q_ASSERT(!"Unknown sensor type");
		return Qt::black;
	}
}
