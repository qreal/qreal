#include "robotItem.h"
#include <QtGui/QCursor>
#include <QtGui/QApplication>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QGraphicsScene>
#include <QtGui/QStyleOptionGraphicsItem>

using namespace qReal::interpreters::robots;
using namespace details::d2Model;
using namespace graphicsUtils;

int const border = 5;

RobotItem::RobotItem()
	: AbstractItem()
	, mImage(QImage(":/icons/robot.png"))
	, mIsOnTheGround(true)
	, mRotater(NULL)
	, mRectangleImpl()
	, mRobotModel()
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemClipsChildrenToShape |
			/* ItemClipsToShape |*/ ItemSendsGeometryChanges);

	setAcceptHoverEvents(true);
	setAcceptDrops(true);
	setCursor(QCursor(Qt::PointingHandCursor));
	setZValue(1);
	mX2 = mX1 + robotWidth;
	mY2 = mY1 + robotHeight;

	mPreviousScenePos = QPointF(0, 0);

	mBasePoint = scenePos();
}

void RobotItem::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)
	mRectangleImpl.drawImageItem(painter, mX1, mY1, mX2, mY2, mImage);
}

void RobotItem::drawExtractionForItem(QPainter* painter)
{
	painter->setPen(QPen(Qt::blue));
	painter->drawRect(QRectF(QPointF(mX1, mY1), QPointF(mX2, mY2)));
}

QRectF RobotItem::boundingRect() const
{
		return mRectangleImpl.boundingRect(mX1, mY1, mX2, mY2, border);
}

QRectF RobotItem::calcNecessaryBoundingRect() const
{
	return boundingRect().adjusted(border, border, -border, -border);
}

void RobotItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	AbstractItem::mousePressEvent(event);
	mPreviousPos = QPointF();
	mPreviousAngle = rotateAngle();
	mIsOnTheGround = false;

	mPreviousScenePos = scenePos();
	if (isSelected()) {
		mRotater->reshapeWithMasterItem(scenePos()- mPreviousScenePos);
	}
}

void RobotItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	AbstractItem::mouseMoveEvent(event);

	if (mPreviousPos.isNull()) {
		mPreviousPos = event->scenePos();
		return;
	}
	mBasePoint += scenePos()- mPreviousScenePos;

	foreach (SensorItem *sensor, mSensors) {
		sensor->setDeltaBasePosition(scenePos()- mPreviousScenePos, rotateAngle());
	}

	if (isSelected()) {
		mRotater->reshapeWithMasterItem(scenePos()- mPreviousScenePos);
	}

	mPreviousPos = event->scenePos();
	mPreviousScenePos = scenePos();
	mPreviousAngle = rotateAngle();
}

void RobotItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	AbstractItem::mouseReleaseEvent(event);
	mPreviousPos = QPointF();
	mPreviousAngle = rotateAngle();
	mIsOnTheGround = true;

	if (isSelected()) {
		mRotater->reshapeWithMasterItem(scenePos() - mPreviousScenePos);
	}
	mPreviousScenePos = scenePos();

	emit changedPosition();
}

QPointF RobotItem::basePoint()
{
	return mBasePoint;
}

void RobotItem::resizeItem(QGraphicsSceneMouseEvent *event)
{
	Q_UNUSED(event);
}

void RobotItem::setPos(QPointF const &newPos)// for moving
{
	foreach (SensorItem *sensor, mSensors) {
		sensor->setDeltaBasePosition(newPos - pos(), rotateAngle());
	}

	mRotater->reshapeWithMasterItem(newPos - pos());

	mPreviousAngle = rotateAngle();

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

void RobotItem::setRobotModel(RobotModelInterface *robotModel)
{
	mRobotModel = robotModel;
}

void RobotItem::rotate(qreal angle)
{
	mRobotModel->rotateOn(angle);
}

QRectF RobotItem::rect() const
{
	return boundingRect();
}

double RobotItem::rotateAngle() const
{
	return mRobotModel->rotateAngle();
}

void RobotItem::setSelected(bool isSelected)
{
	QGraphicsItem::setSelected(isSelected);
}

void RobotItem::checkSelection()
{
	if(isSelected())
		mRotater->setVisible(true);
	else
		mRotater->setVisible(false);
}
