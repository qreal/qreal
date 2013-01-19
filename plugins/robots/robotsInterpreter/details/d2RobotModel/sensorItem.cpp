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
	setFlags(ItemIsSelectable | ItemIsMovable);

	setAcceptHoverEvents(true);
	setAcceptDrops(true);
	setCursor(QCursor(Qt::PointingHandCursor));
	setZValue(1);
	mPreviousScenePos = QPointF(0, 0);
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
	mPreviousScenePos = event->pos();
}

void SensorItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	AbstractItem::mouseMoveEvent(event);
	if (mDragged) {
		mConfiguration.setPosition(mPort, event->scenePos().toPoint());
		QPointF const offset = event->lastPos() - event->pos();
		moveBy(offset.x(), offset.y());

		mPreviousScenePos = scenePos();
	}
}

void SensorItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	AbstractItem::mouseReleaseEvent(event);
	mDragged = false;

	QList<QGraphicsItem *> itemsUnderCursor = scene()->items(event->scenePos()
			, Qt::IntersectsItemShape, Qt::DescendingOrder);
	foreach (QGraphicsItem *itemUnderCursor, itemsUnderCursor) {
		if (mStickyItems.contains(itemUnderCursor)) {
			QPointF const newPos = mapToItem(itemUnderCursor, mapFromScene(scenePos()));
			setParentItem(itemUnderCursor);
			setPos(newPos);
			return;
		}
	}
	QPointF const scenePosition = scenePos();
	setParentItem(NULL);
	setPos(scenePosition);
}

void SensorItem::setBasePosition(QPointF const &pos, qreal dir)
{
	mBasePos = pos;
	mBaseDir = dir;
}

void SensorItem::setDeltaBasePosition(QPointF const &delta, qreal dir)
{
	mBasePos = mBasePos + delta;
	mBaseDir = dir;
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
	qreal oldDir = mConfiguration.direction(mPort);
	mConfiguration.setDirection(mPort, oldDir + angle);
	setRotation(angle);
}

QRectF SensorItem::rect() const
{
	QPointF pos = mBasePos + mConfiguration.position(mPort);
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
