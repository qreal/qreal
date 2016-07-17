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

#include "path.h"

#include <QtCore/QTextStream>

Path::Path(const QPainterPath &path) : Item(nullptr)
{
	mNeedScalingRect = false;
	mPath = path;
	mDomElementType = Item::pictureType;
}

Path::Path(const Path &other)
	:Item()
{
	mNeedScalingRect = other.mNeedScalingRect ;
	setPen(other.pen());
	setBrush(other.brush());
	mDomElementType = Item::pictureType;
	setX1(other.x1());
	setX2(other.x2());
	setY1(other.y1());
	setY2(other.y2());
	mPath = other.mPath;
	mListScalePoint = other.mListScalePoint;
	setPos(other.x(), other.y());
}

Item* Path::clone()
{
	Path* item = new Path(*this);
	return item;
}

QRectF Path::boundingRect() const
{
	return mPath.boundingRect();
}

void Path::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->drawPath(mPath);
}

void Path::drawExtractionForItem(QPainter* painter)
{
	Q_UNUSED(painter);
	drawFieldForResizeItem(painter);
}

void Path::drawFieldForResizeItem(QPainter* painter)
{
	Q_UNUSED(painter);
}

void Path::drawScalingRects(QPainter* painter)
{
	Q_UNUSED(painter);
}

QPair<QDomElement, Item::DomElementTypes> Path::generateItem(QDomDocument &document, const QPoint &topLeftPicture)
{

	QDomElement path = setPenBrushToDoc(document, "path");

	int count = mPath.elementCount();
	QPainterPath::Element elem;
	path.setAttribute("int", count);

	QString text = "";
	for (int i = 0; i < count; ++i)
	{
		elem = mPath.elementAt(i);
		if (elem.type == QPainterPath::LineToElement)
		{
			text += " L ";
			text += QString::number(static_cast<float>(elem.x) + scenePos().x() - topLeftPicture.x());
			text += " ";
			text += QString::number(static_cast<float>(elem.y) + scenePos().y() - topLeftPicture.y());
		}
		else if (elem.type == QPainterPath::CurveToElement) {
			text += " C ";
			text += QString::number(static_cast<float>(elem.x) + scenePos().x() - topLeftPicture.x());
			text += " ";
			text += QString::number(static_cast<float>(elem.y) + scenePos().y() - topLeftPicture.y());
		}
		else if (elem.type == QPainterPath::MoveToElement)
		{
			text += " M ";
			text += QString::number(static_cast<float>(elem.x) + scenePos().x() - topLeftPicture.x());
			text += " ";
			text += QString::number(static_cast<float>(elem.y) + scenePos().y() - topLeftPicture.y());
		}
		else if (elem.type == QPainterPath::CurveToDataElement)
		{
			text += " ";
			text += QString::number(static_cast<float>(elem.x) + scenePos().x() - topLeftPicture.x());
			text += " ";
			text += QString::number(static_cast<float>(elem.y) + scenePos().y() - topLeftPicture.y());
		}
	}
	path.setAttribute("d", text);

	return QPair<QDomElement, Item::DomElementTypes>(path, mDomElementType);
}
