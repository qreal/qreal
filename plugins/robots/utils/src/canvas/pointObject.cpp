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

#include "utils/canvas/pointObject.h"

#include <QtCore/QJsonObject>
#include <QtGui/QPainter>

using namespace utils;

PointObject::PointObject(QObject *parent)
	: CanvasObject(parent)
	, mX(0)
	, mY(0)
{
}

PointObject::PointObject(int x, int y, const QColor &color, int thickness, QObject *parent)
	: CanvasObject(color, thickness, parent)
	, mX(x)
	, mY(y)
{
}

int PointObject::x() const
{
	return mX;
}

void PointObject::setX(int x)
{
	mX = x;
}

int PointObject::y() const
{
	return mY;
}

void PointObject::setY(int y)
{
	mY = y;
}

QPoint PointObject::pos() const
{
	return QPoint(mX, mY);
}

void PointObject::paint(QPainter *painter)
{
	CanvasObject::paint(painter);
	painter->drawPoint(mX, mY);
}

QJsonObject PointObject::toJson() const
{
	return QJsonObject({
		{ "type", "point" }
		, { "x", mX }
		, { "y", mY }
		, { "color", color().name() }
		, { "thickness", thickness() }
	});
}
