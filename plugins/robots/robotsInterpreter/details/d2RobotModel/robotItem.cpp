#include "robotItem.h"

using namespace qReal::interpreters::robots;
using namespace details::d2Model;
using namespace graphicsUtils;

int const border = 5;

RobotItem::RobotItem()
	: RotateItem()
	, mImage(QImage(":/icons/robot.png"))
	, mNeededBeep(false)
	, mIsOnTheGround(true)
	, mRotater(NULL)
	, mRectangleImpl()
	, mRobotModel()
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges);

	setAcceptHoverEvents(true);
	setAcceptDrops(true);
	setCursor(QCursor(Qt::PointingHandCursor));
	setZValue(1);
	mX2 = mX1 + robotWidth;
	mY2 = mY1 + robotHeight;

	mPreviousScenePos = QPointF(0, 0);

	mBasePoint = scenePos();

	setTransformOriginPoint(rotatePoint);
}

void RobotItem::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)
	if (mNeededBeep) {
		playBeep();
		drawBeep(painter);
	}
	mRectangleImpl.drawImageItem(painter, mX1, mY1, mX2, mY2, mImage);
}

void RobotItem::drawBeep(QPainter* painter)
{
	qreal centerX = (mX1 + mX2) / 2;
	qreal centerY = (mY1 + mY2) / 2;
	QPointF const center(centerX, centerY);

	drawBeepArcs(painter, center, 40);
	drawBeepArcs(painter, center, 50);
	drawBeepArcs(painter, center, 60);
}

void RobotItem::drawBeepArcs(QPainter* painter, QPointF const &center, qreal radius)
{
	painter->save();
	QPen pen;
	pen.setColor(Qt::red);
	pen.setWidth(3);
	painter->setPen(pen);
	QRectF rect(center.x()-radius, center.y()-radius, radius+radius, radius+radius);
	painter->drawArc(rect, -45*16, 90*16);
	painter->drawArc(rect, 135*16, 90*16);
	painter->restore();
}

void RobotItem::playBeep()
{
	// TODO
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
	mRobotModel->setRotation(angle);
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
	mRotater->setVisible(isSelected());
}

void RobotItem::setNeededBeep(bool isNeededBeep)
{
	mNeededBeep = isNeededBeep;
}
