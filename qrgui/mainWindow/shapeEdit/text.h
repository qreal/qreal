#pragma once

#include <QtWidgets/QGraphicsTextItem>
#include <QtWidgets/QStyleOptionGraphicsItem>
#include <QtGui/QPainter>
#include <QtGui/QFont>

#include "mainWindow/shapeEdit/item.h"

class Text : public Item
{
public:
	Text(bool isDynamic = false);
	Text(int x, int y, const QString &text = "text", bool isDynamic = false);
	Text(const Text &other);
	virtual Item* clone();
	void init(int x, int y, const QString &text);
	bool isDynamicText();
	virtual void setIsDynamicText(bool isDynamic);
	virtual QRectF boundingRect() const;
	virtual QRectF realBoundingRect() const;
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual void drawExtractionForItem(QPainter* painter);
	virtual void drawFieldForResizeItem(QPainter* painter);
	virtual void drawScalingRects(QPainter* painter);
	virtual void changeScalingPointState(qreal x, qreal y);
	QGraphicsTextItem const& getText();
	virtual void setItemZValue(int zValue);

	virtual QPair<QDomElement, Item::DomElementTypes> generateItem(QDomDocument &document, const QPoint &topLeftPicture);

protected:
	int mX1;
	int mY1;
	QRectF mBoundingRect;
	QGraphicsTextItem mText;
	bool mIsDynamicText;
	QRectF mRect;
	void drawForDynamicText(QPainter* painter);
};
