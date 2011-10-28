#include "ellipse.h"

Ellipse::Ellipse(qreal x1, qreal y1, qreal x2, qreal y2, Item* parent)
	:Item(parent), mRectangleImpl()
{
	mNeedScalingRect = true;
	mPen.setColor(Qt::blue);
	mBrush.setStyle(Qt::NoBrush);
	mDomElementType = pictureType;
	mX1 = x1;
	mY1 = y1;
	mX2 = x2;
	mY2 = y2;
}

Ellipse::Ellipse(Ellipse const &other)
	:Item(), mRectangleImpl()
{
	mNeedScalingRect = other.mNeedScalingRect ;
	mPen = other.mPen;
	mBrush = other.mBrush;
	mDomElementType = pictureType;
	mX1 = other.mX1;
	mX2 = other.mX2;
	mY1 = other.mY1;
	mY2 = other.mY2;
	mListScalePoint = other.mListScalePoint;
	setPos(other.x(), other.y());
}

Item* Ellipse::clone()
{
	Ellipse* item = new Ellipse(*this);
	return item;
}

QRectF Ellipse::boundingRect() const
{
	return mRectangleImpl.boundingRect(mX1, mY1, mX2, mY2, scalingDrift);
}

void Ellipse::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	mRectangleImpl.drawEllipseItem(painter, mX1, mY1, mX2, mY2);
}

QPair<QDomElement, Item::DomElementTypes> Ellipse::generateItem(QDomDocument &document, QPoint const &topLeftPicture)
{
	QDomElement ellipse = setPenBrushToDoc(document, "ellipse");
	setXandY(ellipse, sceneBoundingRectCoord(topLeftPicture));

	return QPair<QDomElement, Item::DomElementTypes>(ellipse, mDomElementType);
}
