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

#include "utils/canvas/arcObject.h"

#include <QtCore/QJsonObject>
#include <QtGui/QPainter>

using namespace utils;

ArcObject::ArcObject(QObject *parent)
	: CanvasObject(parent)
{
}

ArcObject::ArcObject(const QRect &rect, int startAngle, int spanAngle
		, const QColor &color, int thickness, QObject *parent)
	: CanvasObject(color, thickness, parent)
	, mRect(rect)
	, mStartAngle(startAngle)
	, mSpanAngle(spanAngle)
{
}

QPoint ArcObject::ellipseCenter() const
{
	return mRect.center();
}

void ArcObject::setEllipseCenter(const QPoint &center)
{
	mRect.moveTo(center - QPoint(mRect.width() / 2, mRect.height() / 2));
}

QRect ArcObject::ellipseRect() const
{
	return mRect;
}

void ArcObject::setEllipseRect(const QRect &rect)
{
	mRect = rect;
}

int ArcObject::startAngle() const
{
	return mStartAngle;
}

void ArcObject::setStartAngle(int angle)
{
	mStartAngle = angle;
}

int ArcObject::spanAngle() const
{
	return mSpanAngle;
}

void ArcObject::setSpanAngle(int angle)
{
	mSpanAngle = angle;
}

QRect ArcObject::boundingRect() const
{
	return mRect;
}

void ArcObject::paint(QPainter *painter)
{
	CanvasObject::paint(painter);
	// Multiplying on 16 needed because Qt require argument in 1/16 degree format.
	painter->drawArc(ellipseRect(), mStartAngle * 16, mSpanAngle * 16);
}

QJsonObject ArcObject::toJson() const
{
	return QJsonObject({
		{ "type", "arc" }
		, { "x", mRect.center().x() }
		, { "y", mRect.center().y() }
		, { "a", mRect.width() / 2 }
		, { "b", mRect.height() / 2 }
		, { "startAngle", mStartAngle }
		, { "spanAngle", mSpanAngle }
		, { "color", color().name() }
		, { "thickness", thickness() }
	});
}
