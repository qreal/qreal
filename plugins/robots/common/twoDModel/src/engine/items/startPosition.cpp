#include "startPosition.h"

#include "src/engine/model/constants.h"

using namespace twoDModel::items;

const QSizeF size = QSizeF(13, 13);
const int lineWidth = 3;

StartPosition::StartPosition(QGraphicsItem *parent)
	: RotateItem(parent)
{
	setX(robotWidth / 2);
	setY(robotHeight / 2);
	RotateItem::init();
}

QRectF StartPosition::boundingRect() const
{
	return QRectF(-size.width() / 2, -size.height() / 2, size.width(), size.height());
}

void StartPosition::drawItem(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)
	painter->save();
	QPen pen(Qt::red);
	pen.setWidth(lineWidth);
	painter->setPen(pen);
	painter->drawLine(-size.width() / 2, -size.height() / 2, size.width() / 2, size.height() / 2);
	painter->drawLine(-size.width() / 2, size.height() / 2, size.width() / 2, -size.height() / 2);
	painter->restore();
}

void StartPosition::serialize(QDomElement &robotElement, QDomDocument &target) const
{
	QDomElement startPositionElement = target.createElement("startPosition");
	startPositionElement.setAttribute("x", scenePos().x());
	startPositionElement.setAttribute("y", scenePos().y());
	startPositionElement.setAttribute("direction", rotation());
	robotElement.appendChild(startPositionElement);
}

void StartPosition::deserialize(const QDomElement &robotElement)
{
	const QDomElement startPositionElement = robotElement.firstChildElement("startPosition");
	if (startPositionElement.isNull()) {
		const QStringList robotPositionParts = robotElement.attribute("position").split(":");
		const QString robotX = robotPositionParts.count() != 2 ? "0" : robotPositionParts[0];
		const QString robotY = robotPositionParts.count() != 2 ? "0" : robotPositionParts[1];
		setX(robotX.toDouble() + robotWidth / 2);
		setY(robotY.toDouble() + robotHeight / 2);
		setRotation(robotElement.attribute("direction").toDouble());
	} else {
		setX(startPositionElement.attribute("x").toDouble());
		setY(startPositionElement.attribute("y").toDouble());
		setRotation(startPositionElement.attribute("direction").toDouble());
	}
}

void StartPosition::drawFieldForResizeItem(QPainter *painter)
{
	Q_UNUSED(painter)
}

void StartPosition::changeDragState(qreal x, qreal y)
{
	Q_UNUSED(x)
	Q_UNUSED(y)
	mDragState = None;
}
