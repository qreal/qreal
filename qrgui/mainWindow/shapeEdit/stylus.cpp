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

#include "stylus.h"

Stylus::Stylus(qreal x1, qreal y1, Item* parent) : Item(parent), mStylusImpl()
{
	mNeedScalingRect = false;
	setPen(QPen(Qt::black));
	setX1(x1);
	setY1(y1);
	mTmpX1 = x1;
	mTmpY1 = y1;
	mDomElementType = pictureType;
}

Stylus::Stylus(const Stylus &other)
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
	mTmpX1 = other.mTmpX1;
	mTmpY1 = other.mTmpY1;
	mListScalePoint = other.mListScalePoint;
	foreach (AbstractItem *line, other.mAbstractListLine) {
		Line *newLine = new Line(*dynamic_cast<Line *>(line));
		mAbstractListLine.append(newLine);
	}
	setPos(other.x(), other.y());
}

Item* Stylus::clone()
{
	Stylus* item = new Stylus(*this);
	return item;
}

void Stylus::addLine(qreal x2, qreal y2)
{
	setX2(x2);
	setY2(y2);
	Line *line = new Line(mTmpX1, mTmpY1, this->x2(), this->y2(), nullptr);
	line->setPen(pen());
	line->setBrush(brush());
	mAbstractListLine.push_back(dynamic_cast<AbstractItem *>(line));
	mTmpX1 = this->x2();
	mTmpY1 = this->y2();
}

void Stylus::addLineInList(Line *line)
{
	mListLine.push_back(line);
	mAbstractListLine.push_back(dynamic_cast<AbstractItem *>(line));
}

QPainterPath Stylus::shape() const
{
	return mStylusImpl.shape(mAbstractListLine);
}

QRectF Stylus::boundingRect() const
{
	return mStylusImpl.boundingRect(mAbstractListLine);
}

void Stylus::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	mStylusImpl.drawItem(mAbstractListLine, painter, option, widget);
}

void Stylus::drawExtractionForItem(QPainter* painter)
{
	mStylusImpl.drawExtractionForItem(mAbstractListLine, painter);

	/*setPenBrushDriftRect(painter);
	painter->drawPath(shape());
	drawFieldForResizeItem(painter);*/
}

void Stylus::drawFieldForResizeItem(QPainter* painter)
{
	mStylusImpl.drawFieldForResizeItem(painter);
}

void Stylus::drawScalingRects(QPainter* painter)
{
	mStylusImpl.drawScalingRects(painter);
}

void Stylus::setPenStyle(const QString &text)
{
	Item::setPenStyle(text);
	mStylusImpl.setPenStyle(mAbstractListLine, text);
}

void Stylus::setPenWidth(int width)
{
	Item::setPenWidth(width);
	mStylusImpl.setPenWidth(mAbstractListLine, width);
}

void Stylus::setPenColor(const QString &text)
{
	Item::setPenColor(text);
	mStylusImpl.setPenColor(mAbstractListLine, text);
}

void Stylus::setBrushStyle(const QString &text)
{
	Item::setBrushStyle(text);
	mStylusImpl.setBrushStyle(mAbstractListLine, text);
}

void Stylus::setBrushColor(const QString &text)
{
	Item::setBrushColor(text);
	mStylusImpl.setBrushColor(mAbstractListLine, text);
}

QPair<QDomElement, Item::DomElementTypes> Stylus::generateItem(QDomDocument &document, const QPoint &topLeftPicture)
{
	QDomElement stylus = document.createElement("stylus");
	foreach (AbstractItem *aItem, mAbstractListLine) {
		Line *line = dynamic_cast<Line *>(aItem);
		QDomElement item = (line->generateItem(document
				, topLeftPicture - QPoint(static_cast<int>(scenePos().x()), static_cast<int>(scenePos().y())))).first;
		stylus.appendChild(item);
	}
	return QPair<QDomElement, Item::DomElementTypes>(stylus, mDomElementType);
}
