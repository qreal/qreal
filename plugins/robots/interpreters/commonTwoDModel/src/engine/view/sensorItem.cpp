#include "sensorItem.h"

#include <QtGui/QCursor>
#include <QtWidgets/QGraphicsSceneMouseEvent>

#include <interpreterBase/robotModel/robotParts/touchSensor.h>
#include <interpreterBase/robotModel/robotParts/rangeSensor.h>
#include <interpreterBase/robotModel/robotParts/lightSensor.h>

#include "commonTwoDModel/robotModel/parts/colorSensorFull.h"
#include "commonTwoDModel/robotModel/parts/colorSensorPassive.h"
#include "commonTwoDModel/robotModel/parts/colorSensorRed.h"
#include "commonTwoDModel/robotModel/parts/colorSensorGreen.h"
#include "commonTwoDModel/robotModel/parts/colorSensorBlue.h"

using namespace twoDModel::view;
using namespace graphicsUtils;
using namespace interpreterBase::robotModel;

const int selectionDrift = 7;

SensorItem::SensorItem(model::SensorsConfiguration &configuration
		, const PortInfo &port, const QString &pathToImage, const QRect &imageRect)
	: RotateItem()
	, mConfiguration(configuration)
	, mPort(port)
	, mPointImpl()
	, mImageRect(imageRect.isEmpty() ? this->imageRect() : imageRect)
	, mBoundingRect(mImageRect.adjusted(-selectionDrift, -selectionDrift
			, selectionDrift, selectionDrift))
	, mImage(QImage(pathToImage.isEmpty() ? this->pathToImage() : pathToImage))
	, mPortItem(new PortItem(port))
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges);

	setAcceptHoverEvents(true);
	setAcceptDrops(true);
	setCursor(QCursor(Qt::PointingHandCursor));
	setZValue(1);

	mPortItem->setParentItem(this);
	mPortItem->moveBy(-mPortItem->boundingRect().width() - 5, -mPortItem->boundingRect().height() - 5);
	mPortItem->setFlag(ItemIgnoresTransformations);
	mPortItem->hide();

	RotateItem::init();
}

void SensorItem::drawItem(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *widget)
{
	Q_UNUSED(style)
	Q_UNUSED(widget)

	painter->save();
	painter->setRenderHints(painter->renderHints()
			| QPainter::SmoothPixmapTransform
			| QPainter::HighQualityAntialiasing);
	painter->drawImage(mImageRect, mImage);
	painter->restore();
}

void SensorItem::drawExtractionForItem(QPainter *painter)
{
	if (!isSelected()) {
		return;
	}

	painter->save();
	const QPen pen = QPen(Qt::black);
	painter->setPen(pen);
	painter->setOpacity(0.7);
	painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing);
	painter->drawRoundedRect(mBoundingRect, 4, 4);
	painter->restore();
}

QRectF SensorItem::boundingRect() const
{
	return mBoundingRect;
}

void SensorItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	AbstractItem::mousePressEvent(event);
	mPortItem->hide();
}

void SensorItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	AbstractItem::hoverEnterEvent(event);
	mPortItem->show();
}

void SensorItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	AbstractItem::hoverLeaveEvent(event);
	mPortItem->hide();
}

QString SensorItem::name() const
{
	const DeviceInfo sensor = mConfiguration.type(mPort);
	if (sensor.isA<robotParts::TouchSensor>()) {
		return "touch";
	} else if (sensor.isA<robotParts::ColorSensorFull>()
			|| sensor.isA<robotParts::ColorSensorPassive>())
	{
		return "color_empty";
	} else if (sensor.isA<robotParts::ColorSensorRed>()) {
		return "color_red";
	} else if (sensor.isA<robotParts::ColorSensorGreen>()) {
		return "color_green";
	} else if (sensor.isA<robotParts::ColorSensorBlue>()) {
		return "color_blue";
	} else if (sensor.isA<robotParts::RangeSensor>()) {
		return "sonar";
	} else if (sensor.isA<robotParts::LightSensor>()) {
		return "light";
	} else {
		Q_ASSERT(!"Unknown sensor type");
		return "";
	}
}

QRectF SensorItem::imageRect() const
{
	const DeviceInfo sensor = mConfiguration.type(mPort);
	if (sensor.isA<robotParts::TouchSensor>()) {
		return QRectF(-12, -5, 25, 10);
	} else if (sensor.isA<robotParts::ColorSensor>()
			|| sensor.isA<robotParts::LightSensor>())
	{
		return QRectF(-6, -6, 12, 12);
	}
	if (sensor.isA<robotParts::RangeSensor>()) {
		return QRectF(-20, -10, 40, 20);;
	} else {
		Q_ASSERT(!"Unknown sensor type");
		return QRectF();
	}
}

QString SensorItem::pathToImage() const
{
	return QString(":/icons/sensors/2d_%1.png").arg(name());
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

QVariant SensorItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemPositionHasChanged) {
		mConfiguration.setPosition(mPort, pos().toPoint());
	}

	if (change == ItemRotationHasChanged) {
		mConfiguration.setDirection(mPort, rotation());
	}

	return RotateItem::itemChange(change, value);
}

void SensorItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	QGraphicsItem::contextMenuEvent(event);
}

SensorItem::PortItem::PortItem(const PortInfo &port)
	: mPort(port)
	, mFont("Times", 10, QFont::Normal, true)
	, mBoundingRect(QFontMetrics(mFont).boundingRect(port.name()))
{
}

void SensorItem::PortItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)

	painter->save();

	QPen pen;
	pen.setWidth(2);
	pen.setColor(Qt::yellow);

	QBrush brush;
	brush.setStyle(Qt::SolidPattern);
	brush.setColor(Qt::yellow);

	painter->setPen(pen);
	painter->setBrush(brush);

	painter->drawRoundedRect(boundingRect(), 2, 2);

	pen.setColor(Qt::black);
	painter->setPen(pen);
	painter->setFont(mFont);
	painter->drawText(boundingRect(), mPort.name(), QTextOption(Qt::AlignCenter));

	painter->restore();
}

QRectF SensorItem::PortItem::boundingRect() const
{
	return mBoundingRect;
}
