#include "arch.h"
#include "math.h"
#include <QtGui/QGraphicsSceneMouseEvent>

Arch::Arch(QRectF rect, int startAngle, int spanAngle, Item* parent = 0)
	:Item(parent)
{
	mPen.setColor(Qt::gray);
	mBrush.setStyle(Qt::NoBrush);
	mDomElementType = pictureType;
	mX1 = rect.left();
	mX2 = rect.right();
	mY1 = rect.top();
	mY2 = rect.bottom();
	mSpanAngle = spanAngle;
	mStartAngle = startAngle;
	mRect = rect;
	mBoundingRect = boundingRect();
}

int Arch::startAngle() const
{
	return mStartAngle;
}

int Arch::spanAngle() const
{
	return mSpanAngle;
}

void Arch::setStartAngle(int start)
{
	mStartAngle = start;
}

void Arch::setSpanAngle(int span)
{
	mSpanAngle = span;
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

void Arch::drawExtractionForItem(QPainter* painter)
{
	mBoundingRect = boundingRect();
	painter->drawPoint(mBoundingRect.topLeft());
	painter->drawPoint(mBoundingRect.topRight());
	painter->drawPoint(mBoundingRect.bottomLeft());
	painter->drawPoint(mBoundingRect.bottomRight());
}

void Arch::drawScalingRects(QPainter* painter)
{
	Q_UNUSED(painter);
}

void Arch::resizeItem(QGraphicsSceneMouseEvent *event)
{
	Q_UNUSED(event);
}

QRectF Arch::sceneBoundingRectCoord(QPointF const &topLeftPicture)
{
	qreal const x1 = scenePos().x() + mRect.x() - topLeftPicture.x();
	qreal const y1 = scenePos().y() + mRect.y() - topLeftPicture.y();
	return QRectF(x1, y1, mRect.width(), mRect.height());
}

QPair<QDomElement, Item::DomElementTypes> Arch::generateItem(QDomDocument &document, QPointF const &topLeftPicture)
{
	QDomElement arch = document.createElement("arc");
	arch.setAttribute("startAngle", mStartAngle);
	arch.setAttribute("spanAngle", mSpanAngle);
	setXandY(arch, sceneBoundingRectCoord(topLeftPicture));

	return QPair<QDomElement, Item::DomElementTypes>(arch, mDomElementType);
}
