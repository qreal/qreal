/* Copyright 2017 CyberTech Labs Ltd.
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

#include "skittleItem.h"

#include <QtGui/QIcon>
#include <QtWidgets/QAction>

#include <twoDModel/engine/model/constants.h>

using namespace twoDModel::items;

SkittleItem::SkittleItem(QPointF position)
	: SolidItem()
{
	setX(position.x());
	setY(position.y());
	setZValue(1);
	setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsScenePositionChanges);
	setAcceptDrops(true);
}

QAction *SkittleItem::skittleTool()
{
	QAction * const result = new QAction(QIcon(":/icons/skittle_2d.png"), tr("Skittle (S)"), nullptr);
	result->setShortcut(QKeySequence(Qt::Key_S));
	result->setCheckable(true);
	return result;
}

QRectF SkittleItem::boundingRect() const
{
	return QRectF({0, 0}, skittleSize);
}

void SkittleItem::drawItem(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)
	painter->drawImage(boundingRect(), QImage(":/icons/skittle_2d.png"));
}
