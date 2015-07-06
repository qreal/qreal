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

#include "rotateItem.h"

using namespace graphicsUtils;

RotateItem::RotateItem(QGraphicsItem *parent)
	: AbstractItem(parent)
{
}

QRectF RotateItem::rect() const
{
	return boundingRect();
}

Rotater &RotateItem::rotater()
{
	return mRotater;
}

void RotateItem::init()
{
	mRotater.setMasterItem(this);
	mRotater.setVisible(false);
}

QVariant RotateItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
	if (change == QGraphicsItem::ItemSelectedChange) {
		mRotater.setVisible(theOnlySelectedRotateItem(value.toBool()));
		mRotater.setSelected(mRotater.isVisible());
	}

	return AbstractItem::itemChange(change, value);
}

bool RotateItem::theOnlySelectedRotateItem(bool thisSelected) const
{
	if (!thisSelected) {
		return false;
	}

	for (QGraphicsItem * const selectedItem : scene()->selectedItems()) {
		if (dynamic_cast<RotateItem *>(selectedItem) && selectedItem != this) {
			return false;
		}
	}

	return true;
}
