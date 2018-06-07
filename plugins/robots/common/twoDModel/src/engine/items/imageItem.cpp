/* Copyright 2016 CyberTech Labs Ltd.
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

#include "imageItem.h"

#include <QtGui/QPainter>
#include <QtWidgets/QAction>
#include <QtWidgets/QGraphicsSceneMouseEvent>

#include <qrkernel/settingsManager.h>

using namespace twoDModel::items;
using namespace qReal;
using namespace graphicsUtils;

ImageItem::ImageItem(model::Image *image, const QRect &geometry)
	: mImpl()
	, mImage(image)
{
	setX(0);
	setY(0);
	setX1(geometry.left());
	setY1(geometry.top());
	setX2(geometry.right());
	setY2(geometry.bottom());
}

AbstractItem *ImageItem::clone() const
{
	const auto cloned = new ImageItem(mImage, QRect(x1(), y1(), x2() - x1(), y2() - y1()));
	AbstractItem::copyTo(cloned);
	return cloned;
}

QAction *ImageItem::imageTool()
{
	QAction * const result = new QAction(QIcon(":/icons/2d_image.svg"), tr("Image (I)"), nullptr);
	result->setShortcut(QKeySequence(Qt::Key_I));
	result->setCheckable(false);
	return result;
}

QRectF ImageItem::boundingRect() const
{
	return mImpl.boundingRect(x1(), y1(), x2(), y2(), drift);
}

QRectF ImageItem::calcNecessaryBoundingRect() const
{
	return QRectF(qMin(x1(), x2()), qMin(y1(), y2()), qAbs(x2() - x1()), qAbs(y2() - y1()));
}

void ImageItem::drawItem(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	const qreal zoom = scene()->views().isEmpty() ? 1.0 : scene()->views().first()->transform().m11();
	mImage->draw(*painter, calcNecessaryBoundingRect().toRect(), zoom);
}

QPainterPath ImageItem::shape() const
{
	QPainterPath result;
	result.addRect(boundingRect());
	return result;
}

QDomElement ImageItem::serialize(QDomElement &parent) const
{
	QDomElement imageNode = AbstractItem::serialize(parent);
	imageNode.setTagName("image");

//	mImage.serialize(imageNode);
	imageNode.setAttribute("rect", QString("%1:%2:%3:%4").arg(
			QString::number(x1())
			, QString::number(y1())
			, QString::number(x2() - x1())
			, QString::number(y2() - y1())));
	imageNode.setAttribute("position", QString::number(x()) + ":" + QString::number(y()));
	imageNode.setAttribute("imageId", mImage->imageId());
	return imageNode;
}

void ImageItem::deserialize(const QDomElement &element)
{
	AbstractItem::deserialize(element);
	const QRect rect = deserializeRect(element.attribute("rect"));
	setPos(mImpl.deserializePoint(element.attribute("position")));
	setX1(rect.left());
	setX2(rect.right());
	setY1(rect.top());
	setY2(rect.bottom());
}

twoDModel::model::Image *ImageItem::image() const
{
	return mImage;
}

bool ImageItem::memorizes() const
{
	return !mImage->external();
}

QString ImageItem::path() const
{
	return mImage->path();
}

void ImageItem::setMemorize(bool memorize)
{
	mImage->setExternal(!memorize);
	emit internalImageChanged();
}

void ImageItem::setPath(const QString &path)
{
	mImage->setPath(path);
	update();
	emit internalImageChanged();
}

void ImageItem::setBackgroundRole(bool background)
{
	mBackgroundRole = background;
}

bool ImageItem::isBackground() const
{
	return mBackgroundRole;
}

QVariant ImageItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
	if (change == QGraphicsItem::ItemSelectedHasChanged) {
		emit selectedChanged(value.toBool());
	}

	return AbstractItem::itemChange(change, value);
}

QRect ImageItem::deserializeRect(const QString &string) const
{
	const QStringList splittedStr = string.split(":");
	if (splittedStr.count() == 4) {
		const int x = splittedStr[0].toInt();
		const int y = splittedStr[1].toInt();
		const int w = splittedStr[2].toInt();
		const int h = splittedStr[3].toInt();
		return QRect(x, y, w, h);
	}

	return QRect();
}
