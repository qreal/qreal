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

#include "fakeScene.h"

#include "src/engine/model/worldModel.h"
#include "src/engine/items/wallItem.h"
#include "src/engine/items/colorFieldItem.h"

using namespace twoDModel;
using namespace view;
using namespace model;

FakeScene::FakeScene(const WorldModel &world)
{
	connect(&world, &WorldModel::wallAdded, [=](items::WallItem *wall) { addClone(wall, wall->clone()); });
	connect(&world, &WorldModel::colorItemAdded, [=](items::ColorFieldItem *item) { addClone(item, item->clone()); });
	connect(&world, &WorldModel::otherItemAdded, [=](QGraphicsItem *item) {
		if (QGraphicsLineItem * const trace = dynamic_cast<QGraphicsLineItem *>(item)) {
			addClone(item, new QGraphicsLineItem(trace->line()));
		}
	});
	connect(&world, &WorldModel::itemRemoved, this, &FakeScene::deleteItem);
}

void FakeScene::addClone(QGraphicsItem * const original, QGraphicsItem * const cloned)
{
	mClonedItems[original] = cloned;
	addItem(cloned);
}

void FakeScene::deleteItem(QGraphicsItem * const original)
{
	if (mClonedItems.contains(original)) {
		delete mClonedItems[original];
		mClonedItems.remove(original);
	}
}

QImage view::FakeScene::render(const QRectF &piece)
{
	QImage result(piece.size().toSize(), QImage::Format_RGB32);
	QPainter painter(&result);

	QBrush brush(Qt::SolidPattern);
	brush.setColor(Qt::white);
	painter.setBrush(brush);
	painter.setPen(QPen(Qt::white));
	painter.drawRect(piece.translated(-piece.topLeft()));

	QGraphicsScene::render(&painter, QRectF(), piece);

	return result;
}
