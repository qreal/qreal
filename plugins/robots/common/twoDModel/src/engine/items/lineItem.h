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

#include "colorFieldItem.h"

namespace twoDModel {
namespace items {

class LineItem : public ColorFieldItem
{
	Q_OBJECT

public:
	LineItem(const QPointF &begin, const QPointF &end, int cornerRadius = 0);

	AbstractItem *clone() const override;

	/// Creates and returns colored line item for 2D model palette.
	/// Transfers ownership.
	static QAction *lineTool();

	QPainterPath shape() const override;

	QRectF boundingRect() const override;
	void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) override;
	void drawExtractionForItem(QPainter* painter) override;

	void resizeItem(QGraphicsSceneMouseEvent *event) override;
	void reshapeRectWithShift() override;

	QDomElement serialize(QDomDocument &document, const QPointF &topLeftPicture) const override;
	void deserialize(const QDomElement &element) override;

	void deserializePenBrush(const QDomElement &element);
	void setSerializeName(const QString &name);

protected:
	void setPrivateData();

	graphicsUtils::LineImpl mLineImpl;
	QString mSerializeName;
	int mCornerRadius;
};

}
}
