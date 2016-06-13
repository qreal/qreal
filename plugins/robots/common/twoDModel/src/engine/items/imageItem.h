/* Copyright 2016 CyberTech Labs Ltd.
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

#include "twoDModel/engine/model/image.h"

#include <qrutils/graphicsUtils/abstractItem.h>
#include <qrutils/graphicsUtils/rectangleImpl.h>

namespace twoDModel {
namespace items {

class ImageItem : public graphicsUtils::AbstractItem
{
	Q_OBJECT

public:
	ImageItem(const model::Image &image, const QRect &geometry);

	AbstractItem *clone() const;

	/// Creates and returns image item for 2D model palette.
	/// Transfers ownership.
	static QAction *imageTool();

	QPainterPath shape() const override;

	QRectF boundingRect() const override;
	QRectF calcNecessaryBoundingRect() const override;
	void drawItem(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

	void serialize(QDomElement &element) const override;
	void deserialize(const QDomElement &element) override;

protected:
	QRect deserializeRect(const QString &string) const;

	graphicsUtils::RectangleImpl mImpl;
	model::Image mImage;
};

}
}
