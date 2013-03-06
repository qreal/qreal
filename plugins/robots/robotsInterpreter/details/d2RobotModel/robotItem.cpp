#include "robotItem.h"

using namespace qReal::interpreters::robots;
using namespace details::d2Model;
using namespace graphicsUtils;

int const border = 5;

RobotItem::RobotItem()
	: RotateItem()
	, mImage(QImage(":/icons/robot.png"))
	, mBeepItem(new BeepItem)
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

	setTransformOriginPoint(rotatePoint);
	mBeepItem->setParentItem(this);
	mBeepItem->setPos((robotWidth - beepWavesSize) / 2, (robotHeight - beepWavesSize) / 2);
	mBeepItem->setVisible(false);
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
	mIsOnTheGround = false;
	mDragStart = scenePos();
}

void RobotItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	AbstractItem::mouseMoveEvent(event);
}

void RobotItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	AbstractItem::mouseReleaseEvent(event);
	onLanded();
}

void RobotItem::onLanded()
{
	mIsOnTheGround = true;
	emit changedPosition();
}

void RobotItem::resizeItem(QGraphicsSceneMouseEvent *event)
{
	Q_UNUSED(event);
}

void RobotItem::setPos(QPointF const &newPos)// for moving
{
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
	mBeepItem->setVisible(isNeededBeep);
}

QVariant RobotItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemPositionChange) {
		processPositionChange();
	}
	if (change == ItemTransformChange) {
		processPositionAndAngleChange();
	}

	return AbstractItem::itemChange(change, value);
}

void RobotItem::processPositionChange()
{
	foreach (SensorItem * const sensor, mSensors) {
		sensor->onPositionChanged();
	}
}

void RobotItem::processPositionAndAngleChange()
{
	foreach (SensorItem * const sensor, mSensors) {
		sensor->onPositionChanged();
		sensor->onDirectionChanged();
	}
}

void RobotItem::recoverDragStartPosition()
{
	setPos(mDragStart);
	if (!mIsOnTheGround) {
		onLanded();
	}
}

void RobotItem::addSensorsShapes(QPainterPath &target)
{
	foreach (SensorItem *const sensor, mSensors) {
		target.addEllipse(QRectF(sensor->pos() - QPointF(sensorWidth / 2, sensorWidth / 2)
				, QSizeF(sensorWidth, sensorWidth)));
	}
}

void BeepItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option
		, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)

	drawBeep(painter);
}

QRectF BeepItem::boundingRect() const
{
	return QRectF(0, 0, beepWavesSize, beepWavesSize);
}

void BeepItem::drawBeep(QPainter *painter)
{
	QPointF const center(beepWavesSize / 2, beepWavesSize / 2);

	drawBeepArcs(painter, center, 40);
	drawBeepArcs(painter, center, 50);
	drawBeepArcs(painter, center, 60);
}

void BeepItem::drawBeepArcs(QPainter *painter, QPointF const &center, qreal radius)
{
	painter->save();
	QPen pen;
	pen.setColor(Qt::red);
	pen.setWidth(3);
	painter->setPen(pen);
	qreal const diameter = radius + radius;
	QRectF rect(center.x() - radius, center.y() - radius, diameter, diameter);
	painter->drawArc(rect, 45 * 16, 90 * 16);
	painter->drawArc(rect, 225 * 16, 90 * 16);
	painter->restore();
}
