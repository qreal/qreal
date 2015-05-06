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

#include <qrutils/graphicsUtils/rectangleImpl.h>

#include "mainWindow/shapeEdit/item.h"

class Image : public Item
{
public:
	Image(QString fileName, qreal x, qreal y, Item* parent = 0);
	Image(const Image &other);
	virtual Item* clone();
	virtual QRectF boundingRect() const;
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual void setItemZValue(int zValue);

	virtual QPair<QDomElement, Item::DomElementTypes> generateItem(QDomDocument &document
			, const QPoint &topLeftPicture);
private:
	QGraphicsPixmapItem* mPixmapItem;
	QImage mImage;
	QString mFileName;
	graphicsUtils::RectangleImpl mRectangleImpl;
};
