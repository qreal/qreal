#include "robotItem.h"

#include "src/engine/model/constants.h"

using namespace twoDModel::view;
using namespace graphicsUtils;
using namespace twoDModel::model;
using namespace interpreterBase::robotModel::robotParts;

int const border = 0;

RobotItem::RobotItem(QString const &robotImageFileName, model::RobotModel &robotModel, QObject *parent)
	: QObject(parent)
	, RotateItem()
	, mImage(QImage(robotImageFileName))
	, mBeepItem(new BeepItem)
	, mRotater(nullptr)
	, mRectangleImpl()
	, mRobotModel(robotModel)
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges);

	connect(&mRobotModel, &model::RobotModel::positionChanged, this, &RobotItem::setPos);
	connect(&mRobotModel, &model::RobotModel::rotationChanged, this, &RobotItem::setRotation);
	connect(&mRobotModel, &model::RobotModel::playingSoundChanged, this, &RobotItem::setNeededBeep);

	connect(&mRobotModel.configuration(), &model::SensorsConfiguration::deviceRemoved, this, &RobotItem::removeSensor);
	connect(&mRobotModel.configuration(), &model::SensorsConfiguration::positionChanged
			, this, &RobotItem::updateSensorPosition);
	connect(&mRobotModel.configuration(), &model::SensorsConfiguration::rotationChanged
			, this, &RobotItem::updateSensorRotation);

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
	emit mousePressed();
	AbstractItem::mousePressEvent(event);
	mRobotModel.onRobotLiftedFromGround();
	mDragStart = mRobotModel.position();
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
	mRobotModel.onRobotReturnedOnGround();
	emit changedPosition(this);
}

void RobotItem::resizeItem(QGraphicsSceneMouseEvent *event)
{
	Q_UNUSED(event);
}

QMap<interpreterBase::robotModel::PortInfo, SensorItem *> const &RobotItem::sensors() const
{
	return mSensors;
}

void RobotItem::setPos(QPointF const &newPos)
{
	QGraphicsItem::setPos(newPos);
}

void RobotItem::setRotation(qreal rotation)
{
	QGraphicsItem::setRotation(rotation);
}

void RobotItem::addSensor(interpreterBase::robotModel::PortInfo const &port, SensorItem *sensor)
{
	mSensors[port] = sensor;
	sensor->setParentItem(this);

	Rotater * const rotater = new Rotater();
	rotater->setMasterItem(sensor);
	rotater->setVisible(false);
	sensor->setRotater(rotater);

	sensor->setPos(mRobotModel.configuration().position(port));
	sensor->setRotation(mRobotModel.configuration().direction(port));
}

void RobotItem::removeSensor(interpreterBase::robotModel::PortInfo const &port)
{
	if (!mSensors.contains(port) || !mSensors.value(port)) {
		return;
	}

	scene()->removeItem(mSensors[port]);
	delete mSensors[port];
	mSensors[port] = nullptr;
}

void RobotItem::updateSensorPosition(interpreterBase::robotModel::PortInfo const &port)
{
	if (mSensors[port]) {
		mSensors[port]->setPos(mRobotModel.configuration().position(port));
	}
}

void RobotItem::updateSensorRotation(interpreterBase::robotModel::PortInfo const &port)
{
	if (mSensors[port]) {
		mSensors[port]->setRotation(mRobotModel.configuration().direction(port));
	}
}

void RobotItem::setRotater(Rotater *rotater)
{
	mRotater = rotater;
}

QRectF RobotItem::rect() const
{
	return boundingRect();
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

QVariant RobotItem::itemChange(GraphicsItemChange change, QVariant const &value)
{
	if (change == ItemPositionHasChanged) {
		mRobotModel.setPosition(pos());
	}

	if (change == ItemRotationHasChanged) {
		mRobotModel.setRotation(rotation());
	}

	return AbstractItem::itemChange(change, value);
}

void RobotItem::recoverDragStartPosition()
{
	mRobotModel.setPosition(mDragStart);
}

RobotModel &RobotItem::robotModel()
{
	return mRobotModel;
}

void RobotItem::BeepItem::paint(QPainter *painter, QStyleOptionGraphicsItem const *option
		, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)

	drawBeep(painter);
}

QRectF RobotItem::BeepItem::boundingRect() const
{
	return QRectF(0, 0, beepWavesSize, beepWavesSize);
}

void RobotItem::BeepItem::drawBeep(QPainter *painter)
{
	QPointF const center(beepWavesSize / 2, beepWavesSize / 2);

	drawBeepArcs(painter, center, 40);
	drawBeepArcs(painter, center, 50);
	drawBeepArcs(painter, center, 60);
}

void RobotItem::BeepItem::drawBeepArcs(QPainter *painter, QPointF const &center, qreal radius)
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
