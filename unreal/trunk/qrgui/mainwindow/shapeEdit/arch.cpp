#include "arch.h"
#include "math.h"

int Arch::countAngle(qreal x, qreal y, QPointF center)
{
	return (atan2((center.y() - y), (x - center.x())))* 2880 / M_PI;
}

int Arch::sign(int x)
{
	return x / abs(x);
}

void Arch::countSpanAngle(int alfa, int betta)
{
	if (sign(alfa) == sign(betta)) {
		if (alfa < betta)
			mSpanAngle = abs(betta - alfa);
		else
			mSpanAngle = 2 * 2880 - abs(betta - alfa);
	}
	else {
		if (sign(betta) < 0)
			betta += 2 * 2880;
		mSpanAngle = abs(betta - alfa);
	}
}

Arch::Arch(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsEllipseItem* parent = 0)
	:QGraphicsEllipseItem(parent), mParent(parent)
{
	QPointF center = parent->boundingRect().center();
	int alfa = countAngle(x1, y1, center);
	int betta = countAngle(x2, y2, center);
	mStartAngle = alfa;
	countSpanAngle(alfa, betta);
	parent->shape().arcTo(parent->boundingRect(), mStartAngle, mSpanAngle);
	mRect = parent->boundingRect();
}

void Arch::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 )
{
	painter->setPen(QPen(Qt::blue));
	painter->drawArc(mRect, mStartAngle, mSpanAngle);
}

int Arch::startAngle()
{
	return mStartAngle;
}

int Arch::spanAngle()
{
	return mSpanAngle;
}

QRectF	Arch::boundingRect() const
{
	QPainterPath line;
	QPainterPath path;
	if (mParent->rect().isNull())
		return path.boundingRect();
	if (mSpanAngle != 360 * 16) {
		path.moveTo(mParent->rect().center());
		line.moveTo(mParent->rect().center());
		line.lineTo(x(), y());
		path.arcTo(mParent->rect(), mStartAngle / 16.0, mSpanAngle / 16.0);
	} else {
		path.addEllipse(mParent->rect());
	}
	return path.boundingRect();
}
