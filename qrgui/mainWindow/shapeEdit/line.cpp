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

#include "line.h"

#include <QtGui/QPainter>
#include <QtWidgets/QStyle>
#include <QtWidgets/QStyleOptionGraphicsItem>

const int step = 3;

using namespace graphicsUtils;

Line::Line(qreal x1, qreal y1, qreal x2, qreal y2, Item* parent):Item(parent), mLineImpl()
{
	mNeedScalingRect = false;
	setPen(QPen(Qt::green));
	mDomElementType = pictureType;
	setX1(x1);
	setY1(y1);
	setX2(x2);
	setY2(y2);
}

Line::Line(const Line &other):Item(), mLineImpl()
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

Item* Line::clone()
{
	Line* item = new Line(*this);
	return item;
}

QRectF Line::boundingRect() const
{
	return mLineImpl.boundingRect(x1(), y1(), x2(), y2(), pen().width(), drift);
}

QRectF Line::realBoundingRect() const
{
	return mapToScene(mLineImpl.realBoundingRectWithoutScene(x1(), y1(), x2(), y2()
			, pen().width(), drift)).boundingRect();
}

void Line::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	mLineImpl.drawItem(painter, x1(), y1(), x2(), y2());
}

void Line::drawExtractionForItem(QPainter* painter)
{
	mLineImpl.drawPointExtractionForItem(painter, x1(), y1(), x2(), y2());
	setPenBrushDriftRect(painter);
	mLineImpl.drawExtractionForItem(painter, x1(), y1(), x2(), y2(), drift);
	mLineImpl.drawFieldForResizeItem(painter, resizeDrift, x1(), y1(), x2(), y2());

	painter->setPen(QPen(Qt::red));
	painter->setBrush(QBrush(Qt::SolidPattern));
	drawScalingRects(painter);
}

void Line::drawScalingRects(QPainter* painter)
{
	QBrush brush(Qt::SolidPattern);
	if (x2() > x1()) {
		if (y2() > y1()) {
			brush.setColor(mListScalePoint.at(4).second);
			painter->setBrush(brush);
			painter->drawRect(x1() - scalingRect - step, y1() - step, scalingRect, scalingRect);

			brush.setColor(mListScalePoint.at(0).second);
			painter->setBrush(brush);
			painter->drawRect(x1() - step, y1() - scalingRect - step, scalingRect, scalingRect);

			brush.setColor(mListScalePoint.at(3).second);
			painter->setBrush(brush);
			painter->drawRect(x2() - scalingRect + step, y2() + step, scalingRect, scalingRect);

			brush.setColor(mListScalePoint.at(7).second);
			painter->setBrush(brush);
			painter->drawRect(x2() + step, y2() - scalingRect + step, scalingRect, scalingRect);
		} else {
			brush.setColor(mListScalePoint.at(2).second);
			painter->setBrush(brush);
			painter->drawRect(x1() - step, y1() + step, scalingRect, scalingRect);

			brush.setColor(mListScalePoint.at(6).second);
			painter->setBrush(brush);
			painter->drawRect(x1()  - scalingRect - step, y1() - scalingRect + step, scalingRect, scalingRect);

			brush.setColor(mListScalePoint.at(1).second);
			painter->setBrush(brush);
			painter->drawRect(x2() - scalingRect + step, y2() - scalingRect - step, scalingRect, scalingRect);

			brush.setColor(mListScalePoint.at(5).second);
			painter->setBrush(brush);
			painter->drawRect(x2() + step, y2() - step, scalingRect, scalingRect);
		}
	} else {
		if (y2() > y1()) {
			brush.setColor(mListScalePoint.at(5).second);
			painter->setBrush(brush);
			painter->drawRect(x1() + step, y1() - step, scalingRect, scalingRect);

			brush.setColor(mListScalePoint.at(1).second);
			painter->setBrush(brush);
			painter->drawRect(x1() - scalingRect + step, y1() - scalingRect - step, scalingRect, scalingRect);

			brush.setColor(mListScalePoint.at(6).second);
			painter->setBrush(brush);
			painter->drawRect(x2() - scalingRect - step, y2() - scalingRect + step, scalingRect, scalingRect);

			brush.setColor(mListScalePoint.at(2).second);
			painter->setBrush(brush);
			painter->drawRect(x2() - step, y2() + step, scalingRect, scalingRect);
		} else {
			brush.setColor(mListScalePoint.at(4).second);
			painter->setBrush(brush);
			painter->drawRect(x2() - scalingRect - step, y2() - step, scalingRect, scalingRect);

			brush.setColor(mListScalePoint.at(0).second);
			painter->setBrush(brush);
			painter->drawRect(x2() - step, y2() - scalingRect - step, scalingRect, scalingRect);

			brush.setColor(mListScalePoint.at(3).second);
			painter->setBrush(brush);
			painter->drawRect(x1() - scalingRect + step, y1() + step, scalingRect, scalingRect);

			brush.setColor(mListScalePoint.at(7).second);
			painter->setBrush(brush);
			painter->drawRect(x1() + step, y1() - scalingRect + step, scalingRect, scalingRect);
		}
	}
}

QLineF Line::line() const
{
	return mLineImpl.line(x1(), y1(), x2(), y2());
}

QPainterPath Line::shape() const
{
	return mLineImpl.shape(drift, x1(), y1(), x2(), y2());
}

void Line::changeScalingPointState(qreal x, qreal y)
{
	qreal x1= (boundingRect().adjusted(drift, drift, -drift, -drift)).left();
	qreal x2 = (boundingRect().adjusted(drift, drift, -drift, -drift)).right();
	qreal y1 = (boundingRect().adjusted(drift, drift, -drift, -drift)).top();
	qreal y2 = (boundingRect().adjusted(drift, drift, -drift, -drift)).bottom();
	int correction = step;
	calcForChangeScalingState(QPointF(x, y), QPointF(x1, y1), QPointF(x2, y2), correction);
	if (mScalingState == topRightX || mScalingState == topRightY
			|| mScalingState == bottomLeftX || mScalingState == bottomLeftY)
	{
		setDragState(None);
	}
}

void Line::resizeItem(QGraphicsSceneMouseEvent *event)
{
	if (dragState() == TopLeft || dragState() == BottomRight) {
		Item::resizeItem(event);
	}
}

void Line::reshapeRectWithShift()
{
	qreal differenceX = qAbs(x2() - x1());
	qreal differenceY = qAbs(y2() - y1());
	qreal differenceXY = qAbs(differenceX - differenceY);
	qreal size = qMax(differenceX, differenceY);
	const int delta = size / 2;
	if (differenceXY > delta) {
		QPair<qreal, qreal> res = mLineImpl.reshapeRectWithShiftForLine(x1(), y1(), x2(), y2()
				, differenceX, differenceY, size);
		setX2(res.first);
		setY2(res.second);
	} else {
		Item::reshapeRectWithShift();
	}
}

QPair<QPair<QString, QString>, QPair<QString, QString> > Line::setXandYBefore(const QRect &rect)
{
	QString x1;
	QString y1;
	QString y2;
	QString x2;

	if (this->x2() > this->x1()) {
		if (this->y2() > this->y1()) {
			y1 = setScaleForDoc(4, rect);
			x1 = setScaleForDoc(0, rect);
			y2 = setScaleForDoc(7, rect);
			x2 = setScaleForDoc(3, rect);
		} else {
			y1 = setScaleForDoc(6, rect);
			x1 = setScaleForDoc(2, rect);
			y2 = setScaleForDoc(5, rect);
			x2 = setScaleForDoc(1, rect);
		}
	} else {
		if (this->y2() > this->y1()) {
			y1 = setScaleForDoc(5, rect);
			x1 = setScaleForDoc(1, rect);
			y2 = setScaleForDoc(6, rect);
			x2 = setScaleForDoc(2, rect);
		} else {
			y1 = setScaleForDoc(7, rect);
			x1 = setScaleForDoc(3, rect);
			y2 = setScaleForDoc(4, rect);
			x2 = setScaleForDoc(0, rect);
		}
	}

	// omfg!
	return QPair<QPair<QString, QString>, QPair<QString, QString> >(QPair<QString, QString>(x1, y1)
			, QPair<QString, QString>(x2, y2));
}

void Line::setDomXandY(QDomElement& dom, QPair<QPair<QString, QString>, QPair<QString, QString> > pair)
{
	dom.setAttribute("y1", pair.first.second);
	dom.setAttribute("x1", pair.first.first);
	dom.setAttribute("y2", pair.second.second);
	dom.setAttribute("x2", pair.second.first);
}

QPair<QDomElement, Item::DomElementTypes> Line::generateItem(QDomDocument &document, const QPoint &topLeftPicture)
{
	const qreal x1 = scenePos().x() + line().x1() - topLeftPicture.x();
	const qreal y1 = scenePos().y() + line().y1() - topLeftPicture.y();
	const qreal x2 = scenePos().x() + line().x2() - topLeftPicture.x();
	const qreal y2 = scenePos().y() + line().y2() - topLeftPicture.y();

	QDomElement line = setPenBrushToDoc(document, "line");
	setDomXandY(line, setXandYBefore(QRectF(x1, y1, x2 - x1 + 1, y2 - y1 + 1).normalized().toRect()));

	return QPair<QDomElement, Item::DomElementTypes>(line, mDomElementType);
}
