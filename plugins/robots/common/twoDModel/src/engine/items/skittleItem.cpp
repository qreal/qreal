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
#include <QtSvg/QSvgRenderer>

#include <twoDModel/engine/model/constants.h>

using namespace twoDModel::items;

SkittleItem::SkittleItem(const QPointF &position)
	: SolidItem()
	, mStartPosition(QPointF())
	, mStartRotation(0.0f)
	, mSvgRenderer(new QSvgRenderer)
{
	mSvgRenderer->load(QString(":/icons/2d_can.svg"));
	setPos(position);
	setZValue(1);
	setTransformOriginPoint(boundingRect().center());
}

SkittleItem::~SkittleItem()
{
	delete mSvgRenderer;
}

QAction *SkittleItem::skittleTool()
{
	QAction * const result = new QAction(QIcon(":/icons/2d_can.svg"), tr("Can (C)"), nullptr);
	result->setShortcut(QKeySequence(Qt::Key_C));
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
	mSvgRenderer->render(painter, boundingRect());
}

QDomElement SkittleItem::serialize(QDomElement &element) const
{
	QDomElement skittleNode = AbstractItem::serialize(element);
	skittleNode.setTagName("skittle");
	skittleNode.setAttribute("x", QString::number(x1() + scenePos().x()));
	skittleNode.setAttribute("y", QString::number(y1() + scenePos().y()));
	skittleNode.setAttribute("markerX", QString::number(x1() + mStartPosition.x()));
	skittleNode.setAttribute("markerY", QString::number(y1() + mStartPosition.y()));
	skittleNode.setAttribute("rotation", QString::number(rotation()));
	skittleNode.setAttribute("startRotation", QString::number(mStartRotation));
	return skittleNode;
}

void SkittleItem::deserialize(const QDomElement &element)
{
	AbstractItem::deserialize(element);

	qreal x = element.attribute("x", "0").toDouble();
	qreal y = element.attribute("y", "0").toDouble();
	qreal markerX = element.attribute("markerX", "0").toDouble();
	qreal markerY = element.attribute("markerY", "0").toDouble();
	qreal rotation = element.attribute("rotation", "0").toDouble();
	mStartRotation = element.attribute("startRotation", "0").toDouble();

	setPos(QPointF(x, y) + boundingRect().center());
	setTransformOriginPoint(boundingRect().center());
	mStartPosition = {markerX, markerY};
	setRotation(rotation);
}

void SkittleItem::saveStartPosition()
{
	mStartPosition = pos();
	mStartRotation = rotation();
	emit x1Changed(x1());
}

void SkittleItem::returnToStartPosition()
{
	setPos(mStartPosition);
	setRotation(mStartRotation);
	emit x1Changed(x1());
}

QPolygonF SkittleItem::collidingPolygon() const
{
	QRectF rect = {{0, 0}, skittleSize - QSize(1, 1)};
	rect.moveTo(scenePos());
	return QPolygonF(rect);
}

qreal SkittleItem::angularDamping() const
{
	return 6.0f;
}

qreal SkittleItem::linearDamping() const
{
	return 6.0f;
}

QPainterPath SkittleItem::path() const
{
	QPainterPath path;
	QPolygonF collidingPlgn = collidingPolygon();
	QMatrix m;
	m.rotate(rotation());

	QPointF firstP = collidingPlgn.at(0);
	collidingPlgn.translate(-firstP.x(), -firstP.y());

	path.addEllipse(collidingPlgn.boundingRect());
	path = m.map(path);
	path.translate(firstP.x(), firstP.y());

	return path;
}

bool SkittleItem::isCircle() const
{
	return true;
}

qreal SkittleItem::mass() const
{
	return 0.05;
}

qreal SkittleItem::friction() const
{
	return 0.2;
}

SolidItem::BodyType SkittleItem::bodyType() const
{
	return SolidItem::DYNAMIC;
}
