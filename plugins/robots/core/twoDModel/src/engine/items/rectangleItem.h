/* Copyright 2015 CyberTech Labs Ltd.
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

#include "colorFieldItem.h"

namespace twoDModel {
namespace items {

class RectangleItem : public ColorFieldItem
{
	Q_OBJECT
	Q_PROPERTY(bool filled READ filled WRITE setFilled)

public:
	RectangleItem(const QPointF &begin, const QPointF &end);

	AbstractItem *clone() const override;

	/// Creates and returns colored rectangular item for 2D model palette.
	/// Transfers ownership.
	static QAction *rectangleTool();

	QRectF boundingRect() const override;
	QRectF calcNecessaryBoundingRect() const override;
	void drawItem(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

	QDomElement serialize(QDomDocument &document, const QPointF &topLeftPicture) const override;
	void deserialize(const QDomElement &element) override;

	QPainterPath shape() const override;

	/// Returns true if ellipse is filled with its pen color or false if it is "empty".
	bool filled() const;

	/// Fills ellipse with its pen color if true is passed or makes it "empty".
	void setFilled(bool filled);

private:
	void setPrivateData();

	graphicsUtils::RectangleImpl mRectangleImpl;
};

}
}
