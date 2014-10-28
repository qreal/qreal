#include "rectangle.h"

QRealRectangle::QRealRectangle(qreal x1, qreal y1, qreal x2, qreal y2, Item* parent)
	:Item(parent), mRectangleImpl()
{
	mNeedScalingRect = true;
	mPen.setColor(Qt::black);
	mBrush.setStyle(Qt::NoBrush);
	mDomElementType = pictureType;
	mX1 = x1;
	mY1 = y1;
	mX2 = x2;
	mY2 = y2;
}

QRealRectangle::QRealRectangle(QRealRectangle const &other)
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

Item* QRealRectangle::clone()
{
	QRealRectangle* item = new QRealRectangle(*this);
	return item;
}

QRectF QRealRectangle::boundingRect() const
{
	return mRectangleImpl.boundingRect(mX1, mY1, mX2, mY2, scalingDrift);
}

void QRealRectangle::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	mRectangleImpl.drawRectItem(painter, mX1, mY1, mX2, mY2);
}

QPair<QDomElement, Item::DomElementTypes> QRealRectangle::generateItem(QDomDocument &document
		, QPoint const &topLeftPicture)
{
	QDomElement rectangle = setPenBrushToDoc(document, "rectangle");
	setXandY(rectangle, sceneBoundingRectCoord(topLeftPicture));

	return QPair<QDomElement, Item::DomElementTypes>(rectangle, mDomElementType);
}
