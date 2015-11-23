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

#include "utils/canvas/lineObject.h"

#include <QtCore/QJsonObject>
#include <QtGui/QPainter>

using namespace utils;

LineObject::LineObject(QObject *parent)
	: CanvasObject(parent)
{
}

LineObject::LineObject(const QPoint &begin, const QPoint &end, const QColor &color, int thickness, QObject *parent)
	: CanvasObject(color, thickness, parent)
	, mBegin(begin)
	, mEnd(end)
{
}

QPoint LineObject::begin() const
{
	return mBegin;
}

void LineObject::setBegin(const QPoint &begin)
{
	mBegin = begin;
}

QPoint LineObject::end() const
{
	return mEnd;
}

void LineObject::setEnd(const QPoint &end)
{
	mEnd = end;
}

QRect LineObject::boundingRect() const
{
	return QRect(QPoint(qMin(mBegin.x(), mEnd.x()), qMin(mBegin.y(), mEnd.y()))
			, QPoint(qMax(mBegin.x(), mEnd.x()), qMax(mBegin.y(), mEnd.y())));
}

void LineObject::paint(QPainter *painter)
{
	CanvasObject::paint(painter);
	painter->drawLine(mBegin, mEnd);
}

QJsonObject LineObject::toJson() const
{
	return QJsonObject({
		{ "type", "line" }
		, { "x1", mBegin.x() }
		, { "y1", mBegin.y() }
		, { "x2", mEnd.x() }
		, { "y2", mEnd.y() }
		, { "color", color().name() }
		, { "thickness", thickness() }
	});
}
