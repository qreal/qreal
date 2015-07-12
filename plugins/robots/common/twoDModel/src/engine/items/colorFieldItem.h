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

#include <qrutils/graphicsUtils/abstractItem.h>

namespace twoDModel {
namespace items {

class ColorFieldItem : public graphicsUtils::AbstractItem
{
	Q_OBJECT

	Q_PROPERTY(QColor color READ color WRITE setColor)
	Q_PROPERTY(int thickness READ thickness WRITE setThickness)

public:
	explicit ColorFieldItem(QGraphicsItem *parent = 0);
	~ColorFieldItem() override;

	/// Returns a color of this item.
	QColor color() const;

	/// Sets a color of this item.
	void setColor(const QColor &color);

	/// Returns a thickness of this item in px.
	int thickness() const;

	/// Sets a thickness of this item.
	/// @param The thickness value in px.
	void setThickness(int thickness);

	/// Creates a copy of this graphical item. Transfers ownership to the caller.
	virtual AbstractItem *clone() const = 0;

	virtual QDomElement serialize(QDomDocument &document, const QPointF &topLeftPicture) const = 0;

	virtual QRectF boundingRect() const = 0;
};

}
}
