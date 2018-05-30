/* Copyright 2018 CyberTech Labs Ltd.
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

#include "ballItem.h"

#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtSvg/QSvgRenderer>

#include <twoDModel/engine/model/constants.h>

using namespace twoDModel::items;

BallItem::BallItem(const QPointF &position)
	: SolidItem()
	, mStartPosition(QPointF())
	, mStartRotation(0.0f)
	, mSvgRenderer(new QSvgRenderer)
{
	mSvgRenderer->load(QString(":/icons/2d_ball.svg"));
	setPos(position);
	setZValue(1);
	setTransformOriginPoint(boundingRect().center());
}

BallItem::~BallItem()
{
	delete mSvgRenderer;
}

QAction *BallItem::ballTool()
{
	QAction * const result = new QAction(QIcon(":/icons/2d_ball.svg"), tr("Ball (B)"), nullptr);
	result->setShortcut(QKeySequence(Qt::Key_C));
	result->setCheckable(true);
	return result;
}

QRectF BallItem::boundingRect() const
{
	return QRectF({0, 0}, ballSize);
}

void BallItem::drawItem(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)
	mSvgRenderer->render(painter, boundingRect());
}

QDomElement BallItem::serialize(QDomElement &element) const
{
	QDomElement ballNode = AbstractItem::serialize(element);
	ballNode.setTagName("ball");
	ballNode.setAttribute("x", QString::number(x1() + scenePos().x()));
	ballNode.setAttribute("y", QString::number(y1() + scenePos().y()));
	ballNode.setAttribute("markerX", QString::number(x1() + mStartPosition.x()));
	ballNode.setAttribute("markerY", QString::number(y1() + mStartPosition.y()));
	ballNode.setAttribute("rotation", QString::number(rotation()));
	ballNode.setAttribute("startRotation", QString::number(mStartRotation));
	return ballNode;
}

void BallItem::deserialize(const QDomElement &element)
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

void BallItem::saveStartPosition()
{
	mStartPosition = pos();
	mStartRotation = rotation();
	emit x1Changed(x1());
}

void BallItem::returnToStartPosition()
{
	setPos(mStartPosition);
	setRotation(mStartRotation);
	emit x1Changed(x1());
}

QPolygonF BallItem::collidingPolygon() const
{
	QRectF rect = {{0, 0}, ballSize - QSize(1, 1)};
	rect.moveTo(scenePos());
	return QPolygonF(rect);
}

qreal BallItem::angularDamping() const
{
	return 0.08f;
}

qreal BallItem::linearDamping() const
{
	return 0.08f;
}

QPainterPath BallItem::path() const
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

bool BallItem::isCircle() const
{
	return true;
}

qreal BallItem::mass() const
{
	return 0.35f;
}

qreal BallItem::friction() const
{
	return 0.6f;
}

SolidItem::BodyType BallItem::bodyType() const
{
	return SolidItem::DYNAMIC;
}
