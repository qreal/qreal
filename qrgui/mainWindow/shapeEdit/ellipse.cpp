/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "ellipse.h"

QRealEllipse::QRealEllipse(qreal x1, qreal y1, qreal x2, qreal y2, Item* parent)
		: Item(parent), mRectangleImpl()
{
	mNeedScalingRect = true;
	setPen(QPen(Qt::blue));
	setBrush(QBrush(QColor(), Qt::NoBrush));
	mDomElementType = pictureType;
	setX1(x1);
	setY1(y1);
	setX2(x2);
	setY2(y2);
}

QRealEllipse::QRealEllipse(const QRealEllipse &other)
	 : Item(), mRectangleImpl()
{
	mNeedScalingRect = other.mNeedScalingRect ;
	setPen(other.pen());
	setBrush(other.brush());
	mDomElementType = pictureType;
	setX1(other.x1());
	setX2(other.x2());
	setY1(other.y1());
	setY2(other.y2());
	mListScalePoint = other.mListScalePoint;
	setPos(other.x(), other.y());
}

Item* QRealEllipse::clone()
{
	QRealEllipse* item = new QRealEllipse(*this);
	return item;
}

QRectF QRealEllipse::boundingRect() const
{
	return mRectangleImpl.boundingRect(x1(), y1(), x2(), y2(), scalingDrift);
}

void QRealEllipse::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	mRectangleImpl.drawEllipseItem(painter, x1(), y1(), x2(), y2());
}

QPair<QDomElement, Item::DomElementTypes> QRealEllipse::generateItem(QDomDocument &document
		, const QPoint &topLeftPicture)
{
	QDomElement ellipse = setPenBrushToDoc(document, "ellipse");
	setXandY(ellipse, sceneBoundingRectCoord(topLeftPicture));

	return QPair<QDomElement, Item::DomElementTypes>(ellipse, mDomElementType);
}
