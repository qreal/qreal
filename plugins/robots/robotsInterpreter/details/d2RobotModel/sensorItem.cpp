#include <QtGui/QCursor>
#include <QtWidgets/QGraphicsSceneMouseEvent>

#include "sensorItem.h"

using namespace qReal::interpreters::robots;
using namespace details::d2Model;
using namespace graphicsUtils;

int const selectionDrift = 7;
QSizeF const portHintSize(10, 15);

SensorItem::SensorItem(SensorsConfiguration &configuration
		, robots::enums::inputPort::InputPortEnum port)
	: RotateItem()
	, mConfiguration(configuration)
	, mPort(port)
	, mDragged(false)
	, mPointImpl()
	, mRotater(nullptr)
	, mImageRect(imageRect())
	, mBoundingRect(mImageRect.adjusted(-selectionDrift, -selectionDrift
			, selectionDrift, selectionDrift))
	, mImage(pathToImage())
	, mPortItem(QString::number(port + 1))
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges);

	setAcceptHoverEvents(true);
	setAcceptDrops(true);
	setCursor(QCursor(Qt::PointingHandCursor));
	setZValue(1);

	mPortItem.setParentItem(this);
	mPortItem.moveBy(-portHintSize.width() - 5, -portHintSize.height() - 5);
	mPortItem.setFlag(ItemIgnoresTransformations);
	mPortItem.hide();
}

void SensorItem::setRotatePoint(QPointF rotatePoint)
{
	mRotatePoint = rotatePoint;
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
	QPen const pen = QPen(Qt::black);
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
	mPortItem.hide();
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

void SensorItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	AbstractItem::hoverEnterEvent(event);
	mPortItem.show();
}

void SensorItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	AbstractItem::hoverLeaveEvent(event);
	mPortItem.hide();
}

void SensorItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	AbstractItem::mouseReleaseEvent(event);
	mDragged = false;
}

QString SensorItem::name() const
{
	switch (mConfiguration.type(mPort)) {
	case robots::enums::sensorType::touchBoolean:
		return "touch";
	case robots::enums::sensorType::colorFull:
	case robots::enums::sensorType::colorNone:
		return "color_empty";
	case robots::enums::sensorType::colorBlue:
		return "color_blue";
	case robots::enums::sensorType::colorGreen:
		return "color_green";
	case robots::enums::sensorType::colorRed:
		return "color_red";
	case robots::enums::sensorType::sonar:
		return "sonar";
	case robots::enums::sensorType::light:
		return "light";
	default:
		Q_ASSERT(!"Unknown sensor type");
		return "";
	}
}

QRectF SensorItem::imageRect() const
{
	switch (mConfiguration.type(mPort)) {
	case robots::enums::sensorType::touchBoolean:
		return QRectF(-12, -5, 25, 10);
	case robots::enums::sensorType::colorFull:
	case robots::enums::sensorType::colorNone:
	case robots::enums::sensorType::colorBlue:
	case robots::enums::sensorType::colorGreen:
	case robots::enums::sensorType::colorRed:
	case robots::enums::sensorType::light:
		return QRectF(-6, -6, 12, 12);
	case robots::enums::sensorType::sonar:
		return QRectF(-20, -10, 40, 20);
	default:
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

void SensorItem::rotate(qreal angle)
{
	mConfiguration.setDirection(mPort, angle);
	setRotation(angle);
}

QRectF SensorItem::rect() const
{
	return mImageRect;
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
	if (mRotater) {
		mRotater->setVisible(isSelected());
	}
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
	mConfiguration.setPosition(mPort, scenePos().toPoint());
	mConfiguration.setDirection(mPort, rotation());
}

SensorItem::PortItem::PortItem(QString const &port)
	: mPort(port)
{
}

void SensorItem::PortItem::paint(QPainter *painter, QStyleOptionGraphicsItem const *option, QWidget *widget)
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
	painter->drawText(boundingRect(), mPort, QTextOption(Qt::AlignCenter));

	painter->restore();
}

QRectF SensorItem::PortItem::boundingRect() const
{
	return QRectF(QPointF(), portHintSize);
}
