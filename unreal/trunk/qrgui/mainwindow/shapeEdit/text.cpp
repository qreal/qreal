#include "text.h"

#include <QtGui/QCursor>
#include <QDebug>
#include <QStyleOptionGraphicsItem>
#include <QPainter>

Text::Text(qreal x, qreal y, QString const &text, bool isDynamic, QGraphicsTextItem *parent)
	: QGraphicsTextItem(parent), mIsDynamicText(isDynamic)
{
	setTextInteractionFlags(Qt::TextEditorInteraction);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setPos(x, y);
	setHtml(text);
}

void Text::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QGraphicsTextItem::paint(painter, option, widget);
	if (mIsDynamicText)
		drawForDynamicText(painter);
	if (option->state & QStyle::State_Selected) {
		painter->save();
		QPen pen(Qt::red);
		pen.setWidth(3);
		painter->setPen(pen);
		drawExtractionForItem(painter);
		painter->restore();
	}
}

void Text::drawExtractionForItem(QPainter* painter)
{
	mRect = boundingRect();
	painter->drawPoint(mRect.left(), mRect.top());
	painter->drawPoint(mRect.left(), mRect.bottom());
	painter->drawPoint(mRect.right(), mRect.top());
	painter->drawPoint(mRect.right(), mRect.bottom());
}

void Text::drawForDynamicText(QPainter* painter)
{
	QPen pen(Qt::green);
	pen.setWidth(2);
	painter->setPen(pen);
	painter->drawRect(boundingRect());
}

bool Text::isDynamicText()
{
	return mIsDynamicText;
}
