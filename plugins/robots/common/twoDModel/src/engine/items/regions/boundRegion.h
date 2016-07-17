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

#include "regionItem.h"

namespace twoDModel {
namespace items {

/// Zone in 2D model that is drawn above some other item, maybe with some expanded bounds.
class BoundRegion : public RegionItem
{
public:
	explicit BoundRegion(const QGraphicsObject &boundItem, const QString &boundId, QGraphicsItem *parent = 0);

	/// Returns a width of the stripe on which region item wider than item it is bound to.
	int stroke() const;

	/// Sets a width of the stripe on which region item wider than item it is bound to.
	void setStroke(int stroke);

	void serialize(QDomElement &element) override;
	void deserialize(const QDomElement &element) override;

	QRectF boundingRect() const override;

private:
	QPainterPath shape() const override;
	QString regionType() const override;

	const QGraphicsObject &mBoundItem;
	const QString mBoundId;
	int mStroke;
};

}
}
