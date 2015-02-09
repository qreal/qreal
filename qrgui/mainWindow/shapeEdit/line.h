#pragma once

#include <qrutils/graphicsUtils/lineImpl.h>

#include "mainWindow/shapeEdit/item.h"

class Line : public Item
{
	Q_INTERFACES(AbstractItem)
public:
	Line(qreal x1, qreal y1, qreal x2, qreal y2, Item* parent = 0);
	Line(const Line &other);
	virtual Item* clone();
	QLineF line() const;
	QPainterPath shape() const;
	virtual QRectF boundingRect() const;
	virtual QRectF realBoundingRect() const;
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual void drawExtractionForItem(QPainter* painter);
	virtual void drawScalingRects(QPainter* painter);
	virtual void changeScalingPointState(qreal x, qreal y);
	virtual void resizeItem(QGraphicsSceneMouseEvent *event);
	virtual void reshapeRectWithShift();

	QPair<QPair<QString, QString>, QPair<QString, QString> > setXandYBefore(const QRect &rect);
	void setXandY(QDomElement& dom, QPair<QPair<QString, QString>, QPair<QString, QString> > pair);
	virtual QPair<QDomElement, Item::DomElementTypes> generateItem(QDomDocument &document
			, const QPoint &topLeftPicture);

private:
	graphicsUtils::LineImpl mLineImpl;
};
