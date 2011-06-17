#include "robotItem.h"
#include <QtGui/QCursor>
#include <QtGui/QApplication>
#include <QtGui/QGraphicsSceneMouseEvent>

#include <QDebug>

using namespace qReal::interpreters::robots;
using namespace details::d2Model;

int const border = 5;

RobotItem::RobotItem()
	: QGraphicsItem()
	, mImage(QImage(":/icons/robot.png"))
	, mIsOnTheGround(true)
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemClipsChildrenToShape |
		ItemClipsToShape | ItemSendsGeometryChanges);

	setAcceptHoverEvents(true);
	setAcceptDrops(true);
	setCursor(QCursor(Qt::PointingHandCursor));
	setZValue(1);
}

void RobotItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *widget)
{
	Q_UNUSED(style)
	Q_UNUSED(widget)

	painter->drawImage(QPoint(0,0), mImage);
}

QRectF RobotItem::boundingRect() const
{
	return QRect(0,0,robotWidth, robotHeight).adjusted(-border, -border, border, border);
}

void RobotItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mousePressEvent(event);
	mPreviousPos = QPointF();
	mIsOnTheGround = false;
}

void RobotItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseMoveEvent(event);

	if (mPreviousPos.isNull()) {
		mPreviousPos = event->scenePos();
		return;
	}

	foreach (SensorItem *sensor, mSensors)
		sensor->move(event->scenePos().x() - mPreviousPos.x(), event->scenePos().y() - mPreviousPos.y());

	mPreviousPos = event->scenePos();
}

void RobotItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseReleaseEvent(event);
	mPreviousPos = QPointF();
	mIsOnTheGround = true;

	emit changedPosition();
}

void RobotItem::setPos(QPointF const &newPos)
{
	foreach (SensorItem *sensor, mSensors)
		sensor->move(newPos.x() - pos().x(), newPos.y() - pos().y());

	QGraphicsItem::setPos(newPos);
}

void RobotItem::addSensor(SensorItem *sensor)
{
	mSensors.append(sensor);
}

void RobotItem::removeSensor(SensorItem *sensor)
{
	mSensors.removeOne(sensor);
}

void RobotItem::clearSensors()
{
	mSensors.clear();
}

bool RobotItem::isOnTheGround() const
{
	return mIsOnTheGround;
}

