#include <QtGui/QCursor>
#include <QtGui/QGraphicsSceneMouseEvent>

#include "sensorItem.h"

using namespace qReal::interpreters::robots;
using namespace details::d2Model;
using namespace graphicsUtils;

SensorItem::SensorItem(SensorsConfiguration &configuration
		, inputPort::InputPortEnum port)
	: RotateItem()
	, mConfiguration(configuration)
	, mPort(port)
	, mDragged(false)
	, mPointImpl()
	, mRotater(NULL)
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges);

	setAcceptHoverEvents(true);
	setAcceptDrops(true);
	setCursor(QCursor(Qt::PointingHandCursor));
	setZValue(1);
}

void SensorItem::setRotatePoint(QPointF rotatePoint)
{
	mRotatePoint = rotatePoint;
}

void SensorItem::drawItem(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *widget)
{
	Q_UNUSED(style)
	Q_UNUSED(widget)

	QBrush brush;
	brush.setColor(color());
	brush.setStyle(Qt::SolidPattern);

	painter->setBrush(brush);
	painter->setOpacity(0.5);
	painter->setPen(color());

	mPointImpl.drawItem(painter, 0, 0, size);
}

void SensorItem::drawExtractionForItem(QPainter *painter)
{
	if (!isSelected()) {
		return;
	}
	QPen pen = QPen(Qt::black);
	painter->setPen(pen);
	painter->drawEllipse(boundingRect());
}

QRectF SensorItem::boundingRect() const
{
	return mPointImpl.boundingRect(0, 0, size, 0);
}

void SensorItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	AbstractItem::mousePressEvent(event);
	mDragged = true;
}

void SensorItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	AbstractItem::mouseMoveEvent(event);
	if (mDragged) {
		QPointF const offset = event->lastPos() - event->pos();
		moveBy(offset.x(), offset.y());
	}
}

void SensorItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	AbstractItem::mouseReleaseEvent(event);
	mDragged = false;
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

void SensorItem::changeDragState(qreal x, qreal y)
{
	Q_UNUSED(x);
	Q_UNUSED(y);
}

void SensorItem::resizeItem(QGraphicsSceneMouseEvent *event)
{
	Q_UNUSED(event);
}

void SensorItem::rotate(qreal angle)
{
	mConfiguration.setDirection(mPort, angle);
	setRotation(angle);
}

QRectF SensorItem::rect() const
{
	QPointF pos = mConfiguration.position(mPort);
	return  mPointImpl.boundingRect(pos.x(), pos.y(), size, 0);
}

double SensorItem::rotateAngle() const
{
	return mConfiguration.direction(mPort);
}

void SensorItem::setSelected(bool isSelected)
{
	QGraphicsItem::setSelected(isSelected);
}

void SensorItem::setRotater(Rotater *rotater)
{
	mRotater = rotater;
}

void SensorItem::checkSelection()
{
	mRotater->setVisible(isSelected());
}

void SensorItem::addStickyItem(QGraphicsItem *item)
{
	mStickyItems << item;
}

void SensorItem::removeStickyItem(QGraphicsItem *item)
{
	mStickyItems.remove(item);
}

QVariant SensorItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemPositionChange) {
		onPositionChanged();
	}
	if (change == ItemTransformChange) {
		onDirectionChanged();
	}
	return AbstractItem::itemChange(change, value);
}

void SensorItem::onPositionChanged()
{
	mConfiguration.setPosition(mPort, scenePos().toPoint());
}

void SensorItem::onDirectionChanged()
{
	mConfiguration.setDirection(mPort, rotation());
}
