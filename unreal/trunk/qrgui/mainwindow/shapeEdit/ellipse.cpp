#include "ellipse.h"

Ellipse::Ellipse(qreal x1, qreal y1, qreal x2, qreal y2, Item* parent)
	:Item(parent)
{
	mNeedScalingRect = true;
	mPen.setColor(Qt::blue);
	mBrush.setStyle(Qt::NoBrush);
	mDomElementType = pictureType;
	mX1 = x1;
	mY1 = y1;
	mX2 = x2;
	mY2 = y2;
	mBoundingRect = boundingRect().adjusted(scalingDrift, scalingDrift, -scalingDrift, -scalingDrift);
}

QRectF Ellipse::boundingRect() const
{
	return QRectF(qMin(mX1, mX2), qMin(mY1, mY2), abs(mX2 - mX1), abs(mY2 - mY1)).adjusted(-scalingDrift, -scalingDrift, scalingDrift, scalingDrift);
}

void Ellipse::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	if(mX2 > mX1) {
		if (mY2 > mY1)
			painter->drawEllipse(mX1, mY1, mX2 - mX1, mY2 - mY1);
		else
			painter->drawEllipse(mX1, mY2, mX2 - mX1, mY1 - mY2);
	} else {
		if (mY2 > mY1)
			painter->drawEllipse(mX2, mY1, mX1 - mX2, mY2 - mY1);
		else
			painter->drawEllipse(mX2, mY2, mX1 - mX2, mY1 - mY2);
	}
}

QPair<QDomElement, Item::DomElementTypes> Ellipse::generateItem(QDomDocument &document, QPoint const &topLeftPicture)
{
	QDomElement ellipse = setPenBrushToDoc(document, "ellipse");
	setXandY(ellipse, sceneBoundingRectCoord(topLeftPicture));

	return QPair<QDomElement, Item::DomElementTypes>(ellipse, mDomElementType);
}
