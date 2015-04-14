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

#include "lineItem.h"

namespace twoDModel {
namespace items {

class WallItem : public LineItem
{
	Q_OBJECT

public:
	WallItem(const QPointF &begin, const QPointF &end);

	AbstractItem *clone() const override;

	QPointF begin();
	QPointF end();
	bool isDragged() const;
	qreal width() const;

	/// Draws selection rect around sensorBoundingBox
	void drawExtractionForItem(QPainter *painter) override;
	void drawItem(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

	QDomElement serialize(QDomDocument &document, const QPoint &topLeftPicture) override;
	void deserializePenBrush(const QDomElement &element);

	void onOverlappedWithRobot(bool overlapped = true);

	QPainterPath path() const;

signals:
	void wallDragged(WallItem *item, const QPainterPath &shape, const QRectF &oldPos);

protected:
	void setPrivateData();

private:
	void recalculateBorders();

	bool mDragged;
	bool mOverlappedWithRobot;
	const QImage mImage;

	int mOldX1;
	int mOldY1;

	QPainterPath mPath;
};

}
}
