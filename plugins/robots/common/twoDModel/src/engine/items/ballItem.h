/* Copyright 2018 CyberTech Labs Ltd.
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

#include "src/engine/items/solidItem.h"
#include <qrutils/graphicsUtils/abstractItem.h>

class QSvgRenderer;

namespace twoDModel {
namespace items {

class BallItem : public graphicsUtils::AbstractItem, public SolidItem
{
	Q_OBJECT

public:
	explicit BallItem(const QPointF &position);
	~BallItem();

	/// Creates and returns ball item for 2D model palette.
	/// Transfers ownership.
	static QAction *ballTool();

	QRectF boundingRect() const override;
	void drawItem(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	void drawFieldForResizeItem(QPainter* painter) override { Q_UNUSED(painter) }
	void resizeItem(QGraphicsSceneMouseEvent *event) override { Q_UNUSED(event) }

	QDomElement serialize(QDomElement &element) const override;
	void deserialize(const QDomElement &element) override;

	void saveStartPosition();
	void returnToStartPosition();

	bool isCircle() const override;
	qreal mass() const override;
	BodyType bodyType() const override;
	qreal friction() const override;
	QPolygonF collidingPolygon() const override;
	qreal angularDamping() const override;
	qreal linearDamping() const override;

	QPainterPath path() const;

private:
	QPointF mStartPosition;
	qreal mStartRotation;

	QSvgRenderer *mSvgRenderer;
};

}
}
