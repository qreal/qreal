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

#include <qrutils/graphicsUtils/lineImpl.h>
#include <qrutils/graphicsUtils/abstractItem.h>

#include "src/engine/items/solidItem.h"

namespace twoDModel {
namespace items {

class WallItem : public graphicsUtils::AbstractItem, public SolidItem
{
	Q_OBJECT

public:
	WallItem(const QPointF &begin, const QPointF &end);

	/// Creates a copy of this wall item. Transfers ownership to the caller.
	WallItem *clone() const;

	/// Creates and returns wall item for 2D model palette.
	/// Transfers ownership.
	static QAction *wallTool();

	QPainterPath shape() const override;
	QRectF boundingRect() const override;

	QPointF begin() const;
	QPointF end() const;
	qreal width() const;

	/// Draws selection rect around sensorBoundingBox
	void drawExtractionForItem(QPainter *painter) override;
	void drawItem(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	void resizeItem(QGraphicsSceneMouseEvent *event) override;
	void reshapeRectWithShift() override;

	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

	QDomElement serialize(QDomElement &element) const override;
	void deserialize(const QDomElement &element) override;

	void onOverlappedWithRobot(bool overlapped = true);

	QPainterPath path() const;

	void resizeWithGrid(QGraphicsSceneMouseEvent *event, int indexGrid);

	void resizeBeginWithGrid(int indexGrid);
	void reshapeEndWithGrid(int indexGrid);
	void reshapeBeginWithGrid(int indexGrid);
	void setDraggedEnd(qreal x, qreal y);
	void alignTheWall(int indexGrid);


	QPolygonF collidingPolygon() const override;
	qreal mass() const override;
	qreal friction() const override;
	BodyType bodyType() const override;

signals:
	void wallDragged(WallItem *item, const QPainterPath &shape, const QRectF &oldPos);

protected:
	void setPrivateData();

private slots:
	void handleReposition(const QPointF &pos);

private:
	void recalculateBorders();

	qreal alignedCoordinate(qreal coord, int coef, const int indexGrid) const;
	void setBeginCoordinatesWithGrid(int indexGrid);
	void setEndCoordinatesWithGrid(int indexGrid);
	void countCellNumbCoordinates(int indexGrid);

	graphicsUtils::LineImpl mLineImpl;

	bool mOverlappedWithRobot = false;
	const QImage mImage;

	qreal mOldX1 = 0;
	qreal mOldY1 = 0;
	qreal mOldX2 = 0;
	qreal mOldY2 = 0;
	QPointF mOldPosition = QPointF(0, 0);
	int mCellNumbX1 = 0;
	int mCellNumbY1 = 0;
	int mCellNumbX2 = 0;
	int mCellNumbY2 = 0;

	QPainterPath mPath;
};

}
}
