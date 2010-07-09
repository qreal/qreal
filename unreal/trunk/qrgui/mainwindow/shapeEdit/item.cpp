#include "item.h"
#include <QtGui/QPainter>
#include <QtGui/QStyle>
#include <QtGui/QStyleOptionGraphicsItem>

Item::Item(QGraphicsItem* parent) : QGraphicsItem(parent)
{
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemIsMovable, true);
}

void Item::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(widget);
	painter->setPen(mPen);
	drawItem(painter);
	if (option->state & QStyle::State_Selected) {
		painter->save();
		QPen pen(Qt::red);
		pen.setWidth(3);
		painter->setPen(pen);
		drawExtractionForItem(painter);
		painter->restore();
	}
}

void Item::drawExtractionForItem(QPainter* painter)
{
	painter->drawPoint(mX1, mY1);
	painter->drawPoint(mX1, mY2);
	painter->drawPoint(mX2, mY1);
	painter->drawPoint(mX2, mY2);
}

QPen Item::pen() const
{
	return mPen;
}

QBrush Item::brush() const
{
	return mBrush;
}

void Item::setBrush(QBrush const &brush)
{
	mBrush = brush;
}

void Item::setPen(QPen const &pen)
{
	mPen = pen;
}

void Item::setBottomRight(qreal x, qreal y)
{
	mX2 = x;
	mY2 = y;
	update();
}
