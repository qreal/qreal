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

#include "utils/canvas/textObject.h"

#include <QtCore/QJsonObject>
#include <QtGui/QPainter>

using namespace utils;

TextObject::TextObject(QObject *parent)
	: CanvasObject(parent)
	, mX(0)
	, mY(0)
{
}

TextObject::TextObject(int x, int y, const QString &text, const QColor &color, int thickness, QObject *parent)
	: CanvasObject(color, thickness, parent)
	, mX(x)
	, mY(y)
	, mText(text)
{
}

int TextObject::x() const
{
	return mX;
}

void TextObject::setX(int x)
{
	mX = x;
}

int TextObject::y() const
{
	return mY;
}

void TextObject::setY(int y)
{
	mY = y;
}

QPoint TextObject::pos() const
{
	return QPoint(mX, mY);
}

QString TextObject::text() const
{
	return mText;
}

void TextObject::setText(const QString &text)
{
	mText = text;
}

void TextObject::paint(QPainter *painter)
{
	CanvasObject::paint(painter);
	painter->drawText(mX, mY, mText);
}

QJsonObject TextObject::toJson() const
{
	return QJsonObject({
		{ "type", "text" }
		, { "x", x() }
		, { "y", y() }
		, { "text", text() }
		, { "color", color().name() }
		, { "thickness", thickness() }
	});
}
