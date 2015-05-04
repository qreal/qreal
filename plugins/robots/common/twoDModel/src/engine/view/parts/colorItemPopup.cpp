/* Copyright 2015 QReal Research Group, Dmitry Mordvinov
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

#include "colorItemPopup.h"

#include "src/engine/items/wallItem.h"

using namespace twoDModel::view;

ColorItemPopup::ColorItemPopup(graphicsUtils::AbstractScene &scene, QWidget *parent)
	: ItemPopup(scene, parent)
{
}

ColorItemPopup::~ColorItemPopup()
{
}

bool ColorItemPopup::suits(QGraphicsItem *item)
{
	/// @todo: Make wall not inheriting from ColorFieldItem.
	return dynamic_cast<items::ColorFieldItem *>(item) != nullptr
			&& dynamic_cast<items::WallItem *>(item) == nullptr;
}

void ColorItemPopup::attachTo(const QList<QGraphicsItem *> &items)
{
	mItems.clear();
	for (QGraphicsItem * const item : items) {
		mItems << dynamic_cast<items::ColorFieldItem *>(item);
	}
}

void ColorItemPopup::detach()
{
	mItems.clear();
}
