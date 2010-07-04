#pragma once
#include <QGraphicsEllipseItem>
#include <QDebug>
#include <QPainter>

class Arch : public QGraphicsEllipseItem
{
private:
	int mStartAngle;
	int mSpanAngle;
	QRectF mRect;
	QGraphicsEllipseItem* mParent;
public:
	Arch(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsEllipseItem* parent);
	int startAngle();
	int spanAngle();
	int sign(int x);
	int countAngle(qreal x, qreal y, QPointF center);
	void countSpanAngle(int alfa, int betta);
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) ;
	virtual QRectF	boundingRect() const;
};
