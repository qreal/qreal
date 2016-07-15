/* Copyright 2007-2016 QReal Research Group
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

#include "statPoint.h"

#include <QtGui/QPainter>

using namespace qReal::gui::editor;

StatPoint::StatPoint(const PointPortInfo &info)
	: mPoint(info.point)
	, mPropX(info.scalesX)
	, mPropY(info.scalesY)
	, mInitWidth(info.initWidth)
	, mInitHeight(info.initHeight)
	, mType(info.type)
{}

StatPoint::operator QPointF () const
{
	return mPoint;
}

void StatPoint::operator= (const QPointF &p)
{
	mPoint = p;
	mPropX = false;
	mPropY = false;
	mInitHeight = 1;
	mInitWidth = 1;
}

void StatPoint::paint(QPainter *painter, const QRectF &contents) const
{
	const qreal x = mPoint.x() * (mPropX ? mInitWidth : contents.width());
	const qreal y = mPoint.y() * (mPropY ? mInitHeight : contents.height());

	QLineF pointToDraw(x - 0.1, y - 0.1, x + 0.1, y + 0.1);

	const QColor portColor("#465945");
	const QColor highlightColor("#c3dcc4");

	painter->save();
	QPen pen = painter->pen();
	pen.setColor(highlightColor);
	pen.setWidth(12);
	painter->setPen(pen);
	painter->drawLine(pointToDraw);
	pen.setColor(portColor);
	pen.setWidth(3);
	painter->setPen(pen);
	painter->drawLine(pointToDraw);
	painter->restore();
}

QPointF StatPoint::transformForContents(const QRectF &contents) const
{
	const qreal x = mPoint.x() * (mPropX ? mInitWidth : contents.width());
	const qreal y = mPoint.y() * (mPropY ? mInitHeight : contents.height());

	return QPointF(x, y);
}

QString StatPoint::type() const
{
	return mType;
}
