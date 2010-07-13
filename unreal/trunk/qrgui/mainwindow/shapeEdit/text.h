#pragma once

#include <QtGui/QGraphicsTextItem>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QFont>

#include "item.h"

class Text : public Item
{
public:
	Text(qreal x, qreal y, QString const &text = "text", bool isDynamic = false);
	bool isDynamicText();
	virtual QRectF boundingRect() const;
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual void drawExtractionForItem(QPainter* painter);
	QGraphicsTextItem const& getText();

	virtual QPair<QDomElement, Item::DomElementTypes> generateItem(QDomDocument &document, QPointF const &topLeftPicture);

private:
	QGraphicsTextItem mText;
	bool mIsDynamicText;
	QRectF mRect;
	QFont mFont;
	void drawForDynamicText(QPainter* painter);
};
