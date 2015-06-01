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

#pragma once

#include <QtGui/QPainter>

#include "mainWindow/shapeEdit/item.h"
#include "mainWindow/shapeEdit/ellipse.h"

class Arch : public Item
{
public:
	Arch(QRectF rect, int startAngle, int spanAngle, Item* parent);
	Arch(const Arch &other);
	virtual Item* clone();
	int startAngle() const;
	int spanAngle() const;
	void setStartAngle(int start);
	void setSpanAngle(int span);

	virtual QRectF boundingRect() const;
	virtual QRectF sceneBoundingRectCoord(const QPoint &topLeftPicture);
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual void drawExtractionForItem(QPainter* painter);
	virtual void drawScalingRects(QPainter* painter);
	virtual void resizeItem(QGraphicsSceneMouseEvent *event);

	virtual QPair<QDomElement, Item::DomElementTypes> generateItem(QDomDocument &document
			, const QPoint &topLeftPicture);

private:
	int mStartAngle;
	int mSpanAngle;
	QRectF mRect;
};
