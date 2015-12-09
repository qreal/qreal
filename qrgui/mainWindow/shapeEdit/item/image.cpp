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

#include "image.h"

#include <QtGui/QBitmap>
#include <QtGui/QImageWriter>

#include "mainWindow/shapeEdit/scene.h"
#include "mainWindow/shapeEdit/commands/addItemCommand.h"
#include "mainWindow/shapeEdit/item/createItemPushButton.h"

using namespace qReal::shapeEdit;
using namespace qReal::commands;

Image::Image(QString fileName, qreal x, qreal y, Item* parent)
	: Item(parent)
	, mRectangleImpl()
{
    init(fileName, x, y);
	mNeedScalingRect = true;
	mDomElementType = pictureType;
}

Image::Image(const Image &other)
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
	mPixmapItem = other.mPixmapItem;
	mFileName = other.mFileName;
	mImage = other.mImage;
	setPos(other.x(), other.y());
}

void Image::init(QString fileName, qreal x, qreal y)
{
    mFileName = fileName;
    mImage = QImage(fileName);
    QPixmap* pixmap = new QPixmap(fileName);
    mPixmapItem = new QGraphicsPixmapItem(*pixmap);
    setX1(x);
    setY1(y);
    setX2(x + pixmap->width());
    setY2(y + pixmap->height());
}

Item* Image::clone()
{
	Image* item = new Image(*this);
	return item;
}

AbstractCommand *Image::mousePressEvent(QGraphicsSceneMouseEvent *event, Scene *scene)
{
    init(scene->getFileName(), event->scenePos().x(), event->scenePos().y());
    scene->setZValue(this);
    return new AddItemCommand(scene, this);
}

AbstractCommand *Image::mouseMoveEvent(QGraphicsSceneMouseEvent *event, Scene *scene)
{
    Q_UNUSED(event);
    Q_UNUSED(scene);
    return nullptr;
}

QRectF Image::boundingRect() const
{
	return mRectangleImpl.boundingRect(x1(), y1(), x2(), y2(), scalingDrift);
}

QString Image::getItemName() const
{
    return QString("image");
}

void Image::customizeButton(CreateItemPushButton *button) const
{
    //button->setText(QApplication::translate("ShapeEdit", "Image", 0));
    button->setText(tr("Image"));
}

void Image::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	mRectangleImpl.drawImageItemWithMirrored(painter, x1(), y1(), x2(), y2(), mImage);
}

void Image::setItemZValue(int zValue)
{
	mZValue = zValue;
	mPixmapItem->setZValue(zValue);
	setZValue(zValue);
}

QPair<QDomElement, Item::DomElementTypes> Image::generateItem(QDomDocument &document, const QPoint &topLeftPicture)
{
	QDomElement image = document.createElement("image");
	setXandY(image, sceneBoundingRectCoord(topLeftPicture));
	image.setAttribute("name", "images/" + mFileName.section('/', -1));

	return QPair<QDomElement, Item::DomElementTypes>(image, mDomElementType);
}
