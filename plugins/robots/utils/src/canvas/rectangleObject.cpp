/* Copyright 2015 CyberTech Labs Ltd.
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

#include "utils/canvas/rectangleObject.h"

#include <QtCore/QJsonObject>
#include <QtGui/QPainter>

using namespace utils;

RectangleObject::RectangleObject(QObject *parent)
	: CanvasObject(parent)
	, mFilled(false)
{
}

RectangleObject::RectangleObject(const QRect &shape, const QColor &color, int thickness, bool filled, QObject *parent)
	: CanvasObject(color, thickness, parent)
	, mShape(shape)
	, mFilled(filled)
{
}

QRect RectangleObject::shape() const
{
	return mShape;
}

QRect RectangleObject::boundingRect() const
{
	return mShape;
}

bool RectangleObject::filled() const
{
	return mFilled;
}

void RectangleObject::setFilled(bool filled)
{
	mFilled = filled;
}

void RectangleObject::paint(QPainter *painter)
{
	CanvasObject::paint(painter);
	painter->setBrush(mFilled ? QBrush(color(), Qt::SolidPattern) : QBrush());
	painter->drawRect(mShape);
}

QJsonObject RectangleObject::toJson() const
{
	return QJsonObject({
		{ "type", "rectangle" }
		, { "x", mShape.x() }
		, { "y", mShape.y() }
		, { "width", mShape.width() }
		, { "height", mShape.height() }
		, { "color", color().name() }
		, { "thickness", thickness() }
	});
}
