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

#include "boundRegion.h"

#include <QtXml/QDomElement>

using namespace twoDModel::items;

const int defaultStroke = 0;

BoundRegion::BoundRegion(const QGraphicsObject &boundItem, const QString &boundId, QGraphicsItem *parent)
	: RegionItem(parent)
	, mBoundItem(boundItem)
	, mBoundId(boundId)
	, mStroke(defaultStroke)
{
	// We should dispose this item if bound item is deleted.
	connect(&mBoundItem, &QObject::destroyed, this, &QObject::deleteLater);
}

int BoundRegion::stroke() const
{
	return mStroke;
}

void BoundRegion::setStroke(int stroke)
{
	mStroke = stroke;
}

void BoundRegion::serialize(QDomElement &element)
{
	RegionItem::serialize(element);
	element.setAttribute("boundItem", mBoundId);
	element.setAttribute("stroke", mStroke);
}

void BoundRegion::deserialize(const QDomElement &element)
{
	RegionItem::deserialize(element);
	if (element.hasAttribute("stroke")) {
		bool ok = false;
		const int stroke = element.attribute("stroke").toInt(&ok);
		if (ok) {
			mStroke = stroke;
		}
	}
}

QRectF BoundRegion::boundingRect() const
{
	return mBoundItem.boundingRect().adjusted(-mStroke, -mStroke, mStroke, mStroke);
}

QPainterPath BoundRegion::shape() const
{
	if (!mStroke) {
		return mBoundItem.shape();
	}

	QPainterPathStroker stroker;
	stroker.setWidth(mStroke);
	return stroker.createStroke(mBoundItem.shape());
}

QString BoundRegion::regionType() const
{
	return "bound";
}
