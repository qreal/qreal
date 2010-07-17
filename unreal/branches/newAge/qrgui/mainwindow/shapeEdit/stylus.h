#pragma once
#include "item.h"
#include "line.h"
#include <QtCore/QList>

class Stylus : public Item
{
private:
	qreal mTmpX1;
	qreal mTmpY1;
	QRectF searchMaxMinCoord() const;

public:
	QList<Line *> mListLine;
	Stylus(qreal x1, qreal y1, Item* parent);
	void addLine(qreal x2, qreal y2);

	virtual QRectF boundingRect() const;
	QPainterPath shape() const;
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual void drawExtractionForItem(QPainter* painter);
	virtual void drawFieldForResizeItem(QPainter* painter);
	virtual QPair<QDomElement, Item::DomElementTypes> generateItem(QDomDocument &document, QPointF const &topLeftPicture);
};
