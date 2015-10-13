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

#include "startPosition.h"

#include "twoDModel/engine/model/constants.h"

using namespace twoDModel::items;

const QSizeF size = QSizeF(13, 13);
const int lineWidth = 3;

StartPosition::StartPosition(QGraphicsItem *parent)
	: RotateItem(parent)
{
	setX(robotWidth / 2);
	setY(robotHeight / 2);
	RotateItem::init();
}

QRectF StartPosition::boundingRect() const
{
	return QRectF(-size.width() / 2, -size.height() / 2, size.width(), size.height());
}

void StartPosition::drawItem(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)
	painter->save();
	QPen pen(Qt::red);
	pen.setWidth(lineWidth);
	painter->setPen(pen);
	painter->drawLine(-size.width() / 2, -size.height() / 2, size.width() / 2, size.height() / 2);
	painter->drawLine(-size.width() / 2, size.height() / 2, size.width() / 2, -size.height() / 2);
	painter->restore();
}

void StartPosition::serialize(QDomElement &robotElement, QDomDocument &target) const
{
	QDomElement startPositionElement = target.createElement("startPosition");
	startPositionElement.setAttribute("x", QString::number(scenePos().x()));
	startPositionElement.setAttribute("y", QString::number(scenePos().y()));
	startPositionElement.setAttribute("direction", QString::number(rotation()));
	robotElement.appendChild(startPositionElement);
}

void StartPosition::deserialize(const QDomElement &robotElement)
{
	const QDomElement startPositionElement = robotElement.firstChildElement("startPosition");
	if (startPositionElement.isNull()) {
		const QStringList robotPositionParts = robotElement.attribute("position").split(":");
		const QString robotX = robotPositionParts.count() != 2 ? "0" : robotPositionParts[0];
		const QString robotY = robotPositionParts.count() != 2 ? "0" : robotPositionParts[1];
		setX(robotX.toDouble() + robotWidth / 2);
		setY(robotY.toDouble() + robotHeight / 2);
		setRotation(robotElement.attribute("direction").toDouble());
	} else {
		setX(startPositionElement.attribute("x").toDouble());
		setY(startPositionElement.attribute("y").toDouble());
		setRotation(startPositionElement.attribute("direction").toDouble());
	}
}

void StartPosition::drawFieldForResizeItem(QPainter *painter)
{
	Q_UNUSED(painter)
}

void StartPosition::changeDragState(qreal x, qreal y)
{
	Q_UNUSED(x)
	Q_UNUSED(y)
	setDragState(None);
}

void StartPosition::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	Q_UNUSED(event)
}
