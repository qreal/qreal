#include "arch.h"
#include "math.h"
#include <QGraphicsSceneMouseEvent>

int Arch::countAngle(qreal x, qreal y, QPointF const &center)
{
	return (atan2((center.y() - y), (x - center.x())))* 360 * 16 / M_PI;
}

int Arch::sign(int x)
{
	return x > 0 ? 1 : -1;
}

void Arch::countSpanAngle(qreal alpha, qreal beta)
{
	if (sign(alpha) == sign(beta)) {
		if (alpha < beta)
			mSpanAngle = abs(beta - alpha);
		else
			mSpanAngle = 2 * 360 * 16 - abs(beta - alpha);
	}
	else {
		if (sign(beta) < 0)
			beta += 2 * 360 * 16;
		mSpanAngle = abs(beta - alpha);
	}
}

Arch::Arch(qreal x1, qreal y1, qreal x2, qreal y2, Item* parent = 0)
	:Item(parent)
{
	mPen.setColor(Qt::gray);
	mBrush.setStyle(Qt::NoBrush);
	mDomElementType = pictureType;
	QPointF center = parent->boundingRect().center();
	int alpha = countAngle(x1, y1, center);
	int beta = countAngle(x2, y2, center);
	mStartAngle = alpha;
	countSpanAngle(alpha, beta);
	mRect = parent->boundingRect();
}

int Arch::startAngle() const
{
	return mStartAngle;
}

int Arch::spanAngle() const
{
	return mSpanAngle;
}

QRectF Arch::boundingRect() const
{
	QPainterPath line;
	QPainterPath path;
	if (mRect.isNull())
		return path.boundingRect();
	if (mSpanAngle != 360 * 16) {
		path.moveTo(mRect.center());
		line.moveTo(mRect.center());
		line.lineTo(x(), y());
		path.arcTo(mRect, mStartAngle / 16.0, mSpanAngle / 16.0);
	} else {
		path.addEllipse(mRect);
	}
	return path.boundingRect();
}

void Arch::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->drawArc(mRect, mStartAngle, mSpanAngle);
}

QPair<QDomElement, Item::DomElementTypes> Arch::generateItem(QDomDocument &document, QPointF const &topLeftPicture)
{
	QDomElement arch = document.createElement("arch");
	arch.setAttribute("startAngle", mStartAngle);
	arch.setAttribute("spanAngle", mSpanAngle);
	setXandY(arch, sceneBoundingRectCoord(topLeftPicture));

	return QPair<QDomElement, Item::DomElementTypes>(arch, mDomElementType);
}
