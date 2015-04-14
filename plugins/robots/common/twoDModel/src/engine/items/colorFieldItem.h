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
public:
	explicit ColorFieldItem(QGraphicsItem* parent = 0);
	~ColorFieldItem() override;

	/// Creates a copy of this graphical item. Transfers ownership to the caller.
	virtual AbstractItem *clone() const = 0;

	virtual QRectF boundingRect() const = 0;
	void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) override = 0;

	virtual QDomElement serialize(QDomDocument &document, const QPoint &topLeftPicture) = 0;
};

}
}
