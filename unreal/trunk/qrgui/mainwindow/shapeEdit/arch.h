#pragma once

#include <QtGui/QPainter>

#include "item.h"
#include "ellipse.h"

class Arch : public Item
{
public:
	Arch(qreal x1, qreal y1, qreal x2, qreal y2, Item* parent);
	int startAngle() const;
	int spanAngle() const;
	static int sign(int x);
	static int countAngle(qreal x, qreal y, QPointF const &center);
	void countSpanAngle(qreal alpha, qreal beta);

	virtual QRectF boundingRect() const;
	virtual void drawItem(QPainter* painter);

private:
	int mStartAngle;
	int mSpanAngle;
};
