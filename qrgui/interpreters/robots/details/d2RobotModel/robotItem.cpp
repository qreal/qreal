#include "robotItem.h"
#include <QtGui/QCursor>
#include <QtGui/QApplication>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QGraphicsScene>

#include <QDebug>

using namespace qReal::interpreters::robots;
using namespace details::d2Model;

int const border = 5;

RobotItem::RobotItem()
	: QGraphicsItem()
	, mImage(QImage(":/icons/robot.png"))
	, mIsOnTheGround(true)
	, mRotater(NULL)
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

//	if (mRotater)
//		mRotater->setVisible(isSelected());

	if (isSelected()){
		painter->save();
		painter->setPen(QPen(Qt::blue));
		int const delta = 3;
		QRectF rect = boundingRect();
		double x1 = rect.x() + delta;
		double y1 = rect.y() + delta;
		double x2 = rect.x() + rect.width() - delta;
		double y2 = rect.y() + rect.height() - delta;
		painter->drawRect(QRectF(QPointF(x1, y1), QPointF(x2, y2)));
		painter->restore();
	}
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
		sensor->setBasePosition(scenePos());

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
		sensor->setBasePosition(newPos);

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

void RobotItem::setRotater(Rotater *rotater)
{
	mRotater = rotater;
}
