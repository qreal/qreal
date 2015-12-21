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

#include "rectangle.h"

#include "mainWindow/shapeEdit/scene.h"
#include "mainWindow/shapeEdit/item/createItemPushButton.h"

using namespace qReal::shapeEdit;
using namespace qReal::commands;

ShapeEditRectangle::ShapeEditRectangle(qreal x1, qreal y1, qreal x2, qreal y2, Item* parent)
	:Item(parent), mRectangleImpl()
{
	mNeedScalingRect = true;
	setPen(QPen(Qt::black));
	setBrush(QBrush(QColor(), Qt::NoBrush));
	mDomElementType = pictureType;
	setX1(x1);
	setY1(y1);
	setX2(x2);
	setY2(y2);
}

ShapeEditRectangle::ShapeEditRectangle(const ShapeEditRectangle &other)
	:Item(), mRectangleImpl()
{
	mNeedScalingRect = other.mNeedScalingRect ;
	setPen(other.pen());
	setBrush(other.brush());
	mDomElementType = pictureType;
	setX1(other.x1());
	setX2(other.x2());
	setY1(other.y1());
	setY2(other.y2());
	mListScalePoint = other.mListScalePoint;
	setPos(other.x(), other.y());
}

Item* ShapeEditRectangle::clone()
{
    ShapeEditRectangle* item = new ShapeEditRectangle(*this);
	return item;
}

void ShapeEditRectangle::mousePressEvent(QGraphicsSceneMouseEvent *event, Scene *scene)
{
    qreal mX1 = event->scenePos().x();
    qreal mY1 = event->scenePos().y();
    setX1(mX1);
    setY1(mY1);
    setX2(mX1);
    setY2(mY1);

    scene->setPenBrushForItem(this);
    scene->setZValue(this);
    scene->removeMoveFlagForItem(event, this);
    scene->setWaitMove(true);
    scene->addItem(this);
}

void ShapeEditRectangle::reshape(QGraphicsSceneMouseEvent *event)
{
    setX2(event->scenePos().x());
    setY2(event->scenePos().y());
    if (event->modifiers() & Qt::ShiftModifier)
        reshapeRectWithShift();
}

void ShapeEditRectangle::customizeButton(CreateItemPushButton *button) const
{
    button->setObjectName(QString("rectanglePushButton"));
    QIcon icon;
    icon.addFile(QString(":/mainWindow/images/rect.png"), QSize(), QIcon::Normal, QIcon::Off);
    button->setIcon(icon);
    button->setIconSize(QSize(45, 16));
    button->setToolTip(tr("Add a rectangle"));
}

QRectF ShapeEditRectangle::boundingRect() const
{
	return mRectangleImpl.boundingRect(x1(), y1(), x2(), y2(), scalingDrift);
}

void ShapeEditRectangle::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	mRectangleImpl.drawRectItem(painter, x1(), y1(), x2(), y2());
}

QPair<QDomElement, Item::DomElementTypes> ShapeEditRectangle::generateItem(QDomDocument &document
		, const QPoint &topLeftPicture)
{
	QDomElement rectangle = setPenBrushToDoc(document, "rectangle");
	setXandY(rectangle, sceneBoundingRectCoord(topLeftPicture));

	return QPair<QDomElement, Item::DomElementTypes>(rectangle, mDomElementType);
}
