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

#include "itemPopup.h"

#include <QtWidgets/QGraphicsView>

#include "abstractScene.h"

using namespace graphicsUtils;

ItemPopup::ItemPopup(AbstractScene &scene, QWidget *parent)
	: QFrame(parent)
	, mScene(scene)
{
	hide();

	setStyleSheet("QFrame {border-radius: 3; background: #3F000000; border: 1px solid gray;}");

	connect(&mScene, &AbstractScene::leftButtonPressed, this, &ItemPopup::onMousePressedScene);
	connect(&mScene, &AbstractScene::leftButtonReleased, this, &ItemPopup::onMouseReleasedScene);
	setFixedSize(100, 100);
}

ItemPopup::~ItemPopup()
{
}

bool ItemPopup::suits(QGraphicsItem *item)
{
	return dynamic_cast<AbstractItem *>(item) != nullptr;
}

void ItemPopup::attachTo(QGraphicsItem *item)
{
	attachTo(QList<QGraphicsItem *>() << item);
}

void ItemPopup::attachTo(const QList<QGraphicsItem *> &items)
{
	Q_UNUSED(items)
}

void ItemPopup::detach()
{
}

void ItemPopup::onMousePressedScene()
{
	if (isVisible()) {
		detach();
		hide();
	}
}

void ItemPopup::onMouseReleasedScene()
{
	QList<QGraphicsItem *> items;
	for (QGraphicsItem * const item : mScene.selectedItems()) {
		if (suits(item)) {
			items << item;
		}
	}

	if (items.isEmpty()) {
		detach();
		hide();
		return;
	}

	if (items.count() == 1) {
		attachTo(items.first());
	} else {
		attachTo(items);
	}

	show();

	// Item`s pos will be just under this widget`s middle, 10 px below.
	QGraphicsView * const view = mScene.views().first();
	Q_ASSERT(view);
	const QPointF scenePos = leftmostTopmost(items);
	const QPoint moveAbove = view->viewport()->mapToGlobal(view->mapFromScene(scenePos));
	const QPoint globalPoint{moveAbove.x() - size().width() / 2, moveAbove.y() - size().height() - 10};
	move(parentWidget()->mapFromGlobal(globalPoint));
}

QPointF ItemPopup::leftmostTopmost(const QList<QGraphicsItem *> &items)
{
	QPointF result{INT_MAX, INT_MAX};
	for (const QGraphicsItem *item : items) {
		if (const AbstractItem *abstractItem = dynamic_cast<const AbstractItem *>(item)) {
			result.setX(qMin(result.x(), abstractItem->pos().x() + abstractItem->x1()));
			result.setX(qMin(result.x(), abstractItem->pos().x() + abstractItem->x2()));
			result.setY(qMin(result.y(), abstractItem->pos().y() + abstractItem->y1()));
			result.setY(qMin(result.y(), abstractItem->pos().y() + abstractItem->y2()));
		} else {
			result.setX(qMin(result.x(), item->x()));
			result.setY(qMin(result.y(), item->y()));
		}
	}

	return result;
}
