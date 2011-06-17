#include "sensorItem.h"
#include <QtGui/QCursor>
#include <QtGui/QGraphicsSceneMouseEvent>

#include <QDebug>

using namespace qReal::interpreters::robots;
using namespace details::d2Model;

int const size = 5;

SensorItem::SensorItem(QColor const &color)
	: QGraphicsItem()
	, mColor(color)
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemClipsChildrenToShape |
		ItemClipsToShape | ItemSendsGeometryChanges);

	setAcceptHoverEvents(true);
	setAcceptDrops(true);
	setCursor(QCursor(Qt::PointingHandCursor));
	setZValue(1);

	mRectangle = QRectF(-size, -size, size * 2, size * 2);
}

void SensorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *widget)
{
	Q_UNUSED(style)
	Q_UNUSED(widget)

	painter->save();

	QBrush brush;
	brush.setColor(mColor);
	brush.setStyle(Qt::SolidPattern);

	painter->setBrush(brush);
	painter->setOpacity(0.5);
	painter->setPen(mColor);
	painter->drawEllipse(mRectangle);

	painter->restore();
}

QRectF SensorItem::boundingRect() const
{
	return mRectangle;
}

void SensorItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mousePressEvent(event);
}

void SensorItem::move(qreal dx, qreal dy)
{
	setPos(QPointF(pos().x() + dx, pos().y() + dy));
}

