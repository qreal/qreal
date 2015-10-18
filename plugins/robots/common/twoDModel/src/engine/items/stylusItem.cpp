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

#include "stylusItem.h"

#include <QtWidgets/QAction>

using namespace twoDModel::items;
using namespace graphicsUtils;

StylusItem::StylusItem(qreal x1, qreal y1)
	: mStylusImpl()
{
	QPen pen(this->pen());
	pen.setColor(Qt::black);
	pen.setCapStyle(Qt::RoundCap);
	setPen(pen);
	setX1(x1);
	setY1(y1);
	mTmpX1 = x1;
	mTmpY1 = y1;
}

AbstractItem *StylusItem::clone() const
{
	const auto cloned = new StylusItem(x1(), y1());
	AbstractItem::copyTo(cloned);
	connect(this, &StylusItem::segmentAdded, [=](LineItem * const segment) {
		cloned->mAbstractListLine << segment->clone();
		cloned->recalculateProperties();
	});
	cloned->mTmpX1 = mTmpX1;
	cloned->mTmpY1 = mTmpY1;
	cloned->mBoundingRect = mBoundingRect;
	for (const AbstractItem *item : mAbstractListLine) {
		cloned->mAbstractListLine.push_back(static_cast<const LineItem *>(item)->clone());
	}

	return cloned;
}

QAction *StylusItem::stylusTool()
{
	QAction * const result = new QAction(QIcon(":/icons/2d_pencil.png"), tr("Stylus (S)"), nullptr);
	result->setShortcut(QKeySequence(Qt::Key_S));
	return result;
}

void StylusItem::addLine(qreal x2, qreal y2)
{
	setX2(x2);
	setY2(y2);
	LineItem * const line = new LineItem(QPointF(mTmpX1, mTmpY1), QPointF(this->x2(), this->y2()));
	line->setPen(pen());
	line->setBrush(brush());
	line->setSerializeName(QString("stylusLine"));
	mAbstractListLine.push_back(line);
	recalculateProperties();
	mTmpX1 = this->x2();
	mTmpY1 = this->y2();
	emit segmentAdded(line);
}

QPainterPath StylusItem::shape() const
{
	return mStylusImpl.shape(mAbstractListLine);
}

QRectF StylusItem::boundingRect() const
{
	return mBoundingRect;
}

void StylusItem::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	mStylusImpl.drawItem(mAbstractListLine, painter, option, widget);
}

void StylusItem::drawExtractionForItem(QPainter* painter)
{
	mStylusImpl.drawExtractionForItem(mAbstractListLine, painter);
}

void StylusItem::drawFieldForResizeItem(QPainter* painter)
{
	mStylusImpl.drawFieldForResizeItem(painter);
}

void StylusItem::drawScalingRects(QPainter* painter)
{
	mStylusImpl.drawScalingRects(painter);
}

void StylusItem::setPenStyle(const QString& text)
{
	AbstractItem::setPenStyle(text);
	mStylusImpl.setPenStyle(mAbstractListLine, text);
}

void StylusItem::setPenWidth(int width)
{
	AbstractItem::setPenWidth(width);
	mStylusImpl.setPenWidth(mAbstractListLine, width);
}

void StylusItem::setPenColor(const QString& text)
{
	AbstractItem::setPenColor(text);
	mStylusImpl.setPenColor(mAbstractListLine, text);
}

void StylusItem::setBrushStyle(const QString& text)
{
	AbstractItem::setBrushStyle(text);
	mStylusImpl.setBrushStyle(mAbstractListLine, text);
}

void StylusItem::setBrushColor(const QString& text)
{
	AbstractItem::setBrushColor(text);
	mStylusImpl.setBrushColor(mAbstractListLine, text);
}

QDomElement StylusItem::serialize(QDomDocument &document, const QPointF &topLeftPicture) const
{
	QDomElement stylusNode = setPenBrushToDoc(document, "stylus");
	AbstractItem::serialize(stylusNode);
	for (AbstractItem * const abstractItem : mAbstractListLine) {
			LineItem * const line = static_cast<LineItem *>(abstractItem);
			line->setSerializeName("stylusLine");
			QDomElement item = line->serialize(document, topLeftPicture - QPoint(static_cast<int>(scenePos().x())
					, static_cast<int>(scenePos().y())));
			stylusNode.appendChild(item);
	}

	return stylusNode;
}

void StylusItem::deserialize(const QDomElement &element)
{
	AbstractItem::deserialize(element);
	mAbstractListLine.clear();
	recalculateProperties();

	readPenBrush(element);
	QPen pen(this->pen());
	pen.setCapStyle(Qt::RoundCap);
	setPen(pen);
	QDomNodeList stylusAttributes = element.childNodes();
	for (int i = 0; i < stylusAttributes.length(); ++i) {
			QDomElement type = stylusAttributes.at(i).toElement();
			if (type.tagName() == "stylusLine") {
				LineItem * const line = new LineItem(QPointF(0, 0), QPointF(0, 0));
				line->deserialize(type);
				line->setPen(this->pen());
				mAbstractListLine.append(line);
				emit segmentAdded(line);
				recalculateProperties();
			}
	}
}

void StylusItem::resizeItem(QGraphicsSceneMouseEvent *event)
{
	mStylusImpl.resizeItem(event);
}

void StylusItem::recalculateProperties()
{
	mBoundingRect = mStylusImpl.boundingRect(mAbstractListLine);
}
