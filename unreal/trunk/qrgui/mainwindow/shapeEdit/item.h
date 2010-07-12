#pragma once

#include <QtGui/QGraphicsItem>
#include <QtGui/QPen>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QPainter>


class Item : public QGraphicsItem
{
public:
	enum DragState {
		None,
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight
	};
	Item(QGraphicsItem* parent = 0);
	virtual QRectF boundingRect() const = 0;
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual void drawItem(QPainter* painter) = 0;
	virtual void drawExtractionForItem(QPainter* painter);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

	static void swap(qreal &x, qreal &y);
	QPen pen() const;
	QBrush brush() const;
	void setBrush(const QBrush& brush);
	void setPen(const QPen& pen);
	void setX1andY1(qreal x, qreal y);
	void setX1andY2(qreal x, qreal y);
	void setX2andY1(qreal x, qreal y);
	void setX2andY2(qreal x, qreal y);
	void setNoneDragState();
	void reshapeRectWithShift();
	virtual void changeDragState(qreal x, qreal y);
	virtual void calcResizeItem(QGraphicsSceneMouseEvent *event);
	virtual void resizeItem(QGraphicsSceneMouseEvent *event);

protected:
	DragState mDragState;
	QPen mPen;
	QBrush mBrush;
	QRectF mRect;
	qreal mX1;
	qreal mY1;
	qreal mX2;
	qreal mY2;
};
