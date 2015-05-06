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

#include "arch.h"

#include <math.h>
#include <QtWidgets/QGraphicsSceneMouseEvent>

Arch::Arch(QRectF rect, int startAngle, int spanAngle, Item* parent = 0)
	:Item(parent)
{
	mNeedScalingRect = false;
	setPen(QPen(Qt::gray));
	setBrush(QBrush(QColor(), Qt::NoBrush));
	mDomElementType = pictureType;
	setX1(rect.left());
	setX2(rect.right());
	setY1(rect.top());
	setY2(rect.bottom());
	mSpanAngle = spanAngle;
	mStartAngle = startAngle;
	mRect = rect;
}

Arch::Arch(const Arch &other)
	:Item()
{
	mNeedScalingRect = other.mNeedScalingRect ;
	setPen(other.pen());
	setBrush(other.brush());
	mDomElementType = pictureType;
	setX1(other.x1());
	setX2(other.x2());
	setY1(other.y1());
	setY2(other.y2());
	mSpanAngle = other.mSpanAngle;
	mStartAngle = other.mStartAngle;
	mRect = other.mRect;
	mListScalePoint = other.mListScalePoint;
	setPos(other.x(), other.y());
}

Item* Arch::clone()
{
	Arch* item = new Arch(*this);
	return item;
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
	QRectF itemBoundingRect = boundingRect();
	painter->drawPoint(itemBoundingRect.topLeft());
	painter->drawPoint(itemBoundingRect.topRight());
	painter->drawPoint(itemBoundingRect.bottomLeft());
	painter->drawPoint(itemBoundingRect.bottomRight());
}

void Arch::drawScalingRects(QPainter* painter)
{
	Q_UNUSED(painter);
}

void Arch::resizeItem(QGraphicsSceneMouseEvent *event)
{
	Q_UNUSED(event);
}

QRectF Arch::sceneBoundingRectCoord(const QPoint &topLeftPicture)
{
	const qreal x1 = scenePos().x() + mRect.x() - topLeftPicture.x();
	const qreal y1 = scenePos().y() + mRect.y() - topLeftPicture.y();
	return QRectF(x1, y1, mRect.width(), mRect.height());
}

QPair<QDomElement, Item::DomElementTypes> Arch::generateItem(QDomDocument &document, const QPoint &topLeftPicture)
{
	QDomElement arch = document.createElement("arc");
	arch.setAttribute("startAngle", mStartAngle);
	arch.setAttribute("spanAngle", mSpanAngle);
	setXandY(arch, sceneBoundingRectCoord(topLeftPicture));

	return QPair<QDomElement, Item::DomElementTypes>(arch, mDomElementType);
}
