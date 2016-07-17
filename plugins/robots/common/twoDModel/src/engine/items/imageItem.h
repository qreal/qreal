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
	Q_PROPERTY(bool memorize READ memorizes WRITE setMemorize)
	Q_PROPERTY(QString path READ path WRITE setPath)

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

	/// Returns image of this item.
	const model::Image &image() const;

	/// Returns true if this item is embedded into save.
	bool memorizes() const;

	/// Returns true if this item is embedded into save.
	QString path() const;

	/// Tells item to embed image into a save or keep it externally.
	void setMemorize(bool memorize);

	/// Sets a path to displayed image.
	void setPath(const QString &path);

signals:
	/// Emitted when user selects or unselects
	void selectedChanged(bool selected);

private:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

	QRect deserializeRect(const QString &string) const;

	graphicsUtils::RectangleImpl mImpl;
	model::Image mImage;
};

}
}
