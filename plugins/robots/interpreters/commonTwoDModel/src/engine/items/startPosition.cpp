#include "startPosition.h"

using namespace twoDModel::items;

const QSizeF size = QSizeF(13, 13);
const int lineWidth = 3;

StartPosition::StartPosition(QGraphicsItem *parent)
	: RotateItem(parent)
{
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

void StartPosition::serialize(QDomElement &robotElement) const
{
	robotElement.setAttribute("startPosX", scenePos().x());
	robotElement.setAttribute("startPosY", scenePos().y());
}

void StartPosition::deserialize(const QDomElement &robotElement)
{
	const QStringList robotPositionParts = robotElement.attribute("position").split(":");
	const QString robotX = robotPositionParts.count() != 2 ? "0" : robotPositionParts[0];
	const QString robotY = robotPositionParts.count() != 2 ? "0" : robotPositionParts[1];
	const QString startPositionX = robotElement.hasAttribute("startPosX")
			? robotElement.attribute("startPosX")
			: robotX;
	const QString startPositionY = robotElement.hasAttribute("startPosY")
			? robotElement.attribute("startPosY")
			: robotY;
	setX(startPositionX.toDouble());
	setY(startPositionY.toDouble());
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
