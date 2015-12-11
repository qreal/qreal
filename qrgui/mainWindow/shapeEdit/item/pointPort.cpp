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

#include "pointPort.h"

#include "mainWindow/shapeEdit/scene.h"
#include "mainWindow/shapeEdit/item/createItemPushButton.h"

using namespace qReal::shapeEdit;
using namespace qReal::commands;

const int step = 3;

PointPort::PointPort(qreal x, qreal y, Item *parent)
    : Item(parent)
    , mPointImpl()
{
	mNeedScalingRect = true;
    init(x, y);
    Item::setPen(QPen(Qt::blue));
    Item::setBrush(QBrush(Qt::blue, Qt::SolidPattern));
	mDomElementType = portType;
}

PointPort::PointPort(const PointPort &other)
    : Item()
    , mPointImpl()
{
	mNeedScalingRect = other.mNeedScalingRect ;
    Item::setPen(other.pen());
    Item::setBrush(other.brush());
	mDomElementType = portType;
	setX1(other.x1());
	setX2(other.x2());
	setY1(other.y1());
	setY2(other.y2());
	mRadius = other.mRadius;
	mListScalePoint = other.mListScalePoint;
	mType = other.mType;
	setPos(other.x(), other.y());
}

void PointPort::init(qreal x, qreal y)
{
    mRadius = 2;
    mUnrealRadius = mRadius * 1.6;
    setX1(x - mUnrealRadius);
    setY1(y - mUnrealRadius);
    setX2(x + mUnrealRadius);
    setY2(y + mUnrealRadius);
}

Item* PointPort::clone()
{
	PointPort* item = new PointPort(*this);
	return item;
}

void PointPort::mousePressEvent(QGraphicsSceneMouseEvent *event, Scene *scene)
{
    init(event->scenePos().x(), event->scenePos().y());
    setType(scene->getPortType());
    scene->setZValue(this);
    scene->addItem(this);
}

void PointPort::mouseMoveEvent(QGraphicsSceneMouseEvent *event, Scene *scene)
{
    Q_UNUSED(event);
    Q_UNUSED(scene);
}

QString PointPort::getItemName() const
{
    return QString("pointPort");
}

void PointPort::customizeButton(CreateItemPushButton *button) const
{
    QIcon icon;
    icon.addFile(QString(":/mainWindow/images/ellipse.png"), QSize(), QIcon::Normal, QIcon::Off);
    button->setIcon(icon);
    button->setIconSize(QSize(4, 6));
}

QRectF PointPort::boundingRect() const
{
	return mPointImpl.boundingRect(x1() + mUnrealRadius, y1() + mUnrealRadius, mRadius, scalingDrift);
}

void PointPort::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->setPen(pen());
	painter->setBrush(brush());
	mPointImpl.drawItem(painter, x1() + mUnrealRadius, y1() + mUnrealRadius, mRadius);
}

void PointPort::drawExtractionForItem(QPainter* painter)
{
	QPen pen(Qt::red);
	pen.setWidth(2 * mRadius / 2.3);
	painter->setPen(pen);
	Item::drawExtractionForItem(painter);
	drawFieldForResizeItem(painter);
}

void PointPort::drawFieldForResizeItem(QPainter* painter)
{
	mPointImpl.drawFieldForResizeItem(painter);
}

void PointPort::drawScalingRects(QPainter* painter)
{
	QBrush brush(Qt::SolidPattern);
	QRectF itemBoundingRect = boundingRect().adjusted(scalingDrift, scalingDrift, -scalingDrift, -scalingDrift);
	qreal x1= itemBoundingRect.left();
	qreal y1 = itemBoundingRect.top();
	int scalingPoint = scalingRect;

	brush.setColor(mListScalePoint.at(4).second);
	painter->setBrush(brush);
	painter->drawRect(x1 - scalingPoint - step, y1 - step, scalingPoint, scalingPoint);

	brush.setColor(mListScalePoint.at(0).second);
	painter->setBrush(brush);
	painter->drawRect(x1 - step, y1 - scalingPoint - step, scalingPoint, scalingPoint);
}

void PointPort::changeDragState(qreal x, qreal y)
{
	Q_UNUSED(x);
	Q_UNUSED(y);
}

void PointPort::changeScalingPointState(qreal x, qreal y)
{
	QRectF itemBoundingRect = boundingRect().adjusted(scalingDrift, scalingDrift, -scalingDrift, -scalingDrift);
	qreal x1= itemBoundingRect.left();
	qreal x2 = itemBoundingRect.right();
	qreal y1 = itemBoundingRect.top();
	qreal y2 = itemBoundingRect.bottom();
	int correction = step;
	calcForChangeScalingState(QPointF(x, y), QPointF(x1, y1), QPointF(x2, y2), correction);
}

void PointPort::resizeItem(QGraphicsSceneMouseEvent *event)
{
	Q_UNUSED(event);
}

void PointPort::setPenStyle(const QString &text)
{
    Q_UNUSED(text)
}
void PointPort::setPenWidth(int width)
{
    Q_UNUSED(width)
}
void PointPort::setPenColor(const QString &text)
{
    Q_UNUSED(text)
}
void PointPort::setBrushStyle(const QString &text)
{
    Q_UNUSED(text)
}
void PointPort::setBrushColor(const QString &text)
{
    Q_UNUSED(text)
}
void PointPort::setPen(const QString &penStyle, int width, const QString &penColor)
{
    Q_UNUSED(penStyle)
    Q_UNUSED(width)
    Q_UNUSED(penColor)
}
void PointPort::setBrush(const QString &brushStyle, const QString &brushColor)
{
    Q_UNUSED(brushStyle)
    Q_UNUSED(brushColor)
}
void PointPort::setPenBrush(const QString &penStyle, int width, const QString &penColor
        , const QString &brushStyle, const QString &brushColor)
{
    Q_UNUSED(penStyle)
    Q_UNUSED(width)
    Q_UNUSED(penColor)
    Q_UNUSED(brushStyle)
    Q_UNUSED(brushColor)
}

QPair<QDomElement, Item::DomElementTypes> PointPort::generateItem(QDomDocument &document, const QPoint &topLeftPicture)
{
	QRectF itemBoundingRect = boundingRect().adjusted(scalingDrift, scalingDrift, -scalingDrift, -scalingDrift);
	QDomElement pointPort = document.createElement("pointPort");
	const int x = static_cast<int>(scenePos().x() + itemBoundingRect.x() + mRadius - topLeftPicture.x());
	const int y = static_cast<int>(scenePos().y() + itemBoundingRect.y() + mRadius - topLeftPicture.y());
	pointPort.setAttribute("y", setSingleScaleForDoc(4, x, y));
	pointPort.setAttribute("x", setSingleScaleForDoc(0, x, y));
	pointPort.setAttribute("type", mType);

	return QPair<QDomElement, Item::DomElementTypes>(pointPort, mDomElementType);
}
