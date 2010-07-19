#pragma once

#include <QtGui/QPainter>

#include "item.h"
#include "path.h"

class Curve : public Path
{
public:
	Curve(QPointF const &start, QPointF const &end, QPointF const &c1);
	void  setCXandCY(qreal x, qreal y);
	QRectF searchMaxMinCoord() const;
	virtual QRectF boundingRect() const;
	QPainterPath shape() const;
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual void drawExtractionForItem(QPainter* painter);
	virtual void drawScalingRects(QPainter* painter);
	virtual void drawFieldForResizeItem(QPainter* painter);

	virtual void changeDragState(qreal x, qreal y);
	virtual void calcResizeItem(QGraphicsSceneMouseEvent *event);

	virtual QPair<QDomElement, Item::DomElementTypes> generateItem(QDomDocument &document, QPointF const &topLeftPicture);

private:
	QPointF mC1;
	QPointF mC2;
	QPainterPath* mCurvePath;
};
