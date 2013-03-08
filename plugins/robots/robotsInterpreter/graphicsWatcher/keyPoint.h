#pragma once

#include <QGraphicsItem>
#include <QPainter>
#include <QRadialGradient>

class KeyPoint : public QGraphicsItem
{
public:
	KeyPoint();

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option
		, QWidget *widget);

private:
	QRectF mVisualArea;
};

