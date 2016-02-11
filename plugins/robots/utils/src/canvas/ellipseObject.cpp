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

#include "utils/canvas/ellipseObject.h"

#include <QtCore/QJsonObject>
#include <QtGui/QPainter>

using namespace utils;

EllipseObject::EllipseObject(QObject *parent)
	: CanvasObject(parent)
	, mSemiDiameterX(0)
	, mSemiDiameterY(0)
	, mFilled(false)
{
}

EllipseObject::EllipseObject(const QPoint &center, int semiDiameterX, int semiDiameterY
		, const QColor &color, int thickness, bool filled, QObject *parent)
	: CanvasObject(color, thickness, parent)
	, mCenter(center)
	, mSemiDiameterX(semiDiameterX)
	, mSemiDiameterY(semiDiameterY)
	, mFilled(filled)
{
}

QPoint EllipseObject::center() const
{
	return mCenter;
}

void EllipseObject::setCenter(const QPoint &center)
{
	mCenter = center;
}

int EllipseObject::semiDiameterX() const
{
	return mSemiDiameterX;
}

void EllipseObject::setSemiDiameterX(int a)
{
	mSemiDiameterX = a;
}

int EllipseObject::semiDiameterY() const
{
	return mSemiDiameterY;
}

void EllipseObject::setSemiDiameterY(int b)
{
	mSemiDiameterY = b;
}

QRect EllipseObject::boundingRect() const
{
	return QRect(center() - QPoint(semiDiameterX(), semiDiameterY()), QSize(2 * semiDiameterX(), 2 * semiDiameterY()));
}

bool EllipseObject::filled() const
{
	return mFilled;
}

void EllipseObject::setFilled(bool filled)
{
	mFilled = filled;
}

void EllipseObject::paint(QPainter *painter)
{
	CanvasObject::paint(painter);
	painter->setBrush(mFilled ? QBrush(color(), Qt::SolidPattern) : QBrush());
	painter->drawEllipse(mCenter, mSemiDiameterX, mSemiDiameterY);
}

QJsonObject EllipseObject::toJson() const
{
	return QJsonObject({
		{ "type", "ellipse" }
		, { "x", mCenter.x() }
		, { "y", mCenter.y() }
		, { "a", mSemiDiameterX }
		, { "b", mSemiDiameterY }
		, { "color", color().name() }
		, { "thickness", thickness() }
	});
}
