/* Copyright 2016 Polina Tarasova
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

#include "statCircular.h"

#include <QtGui/QPainter>

using namespace qReal::gui::editor;

StatCircular::StatCircular(const CircularPortInfo &info)
	: mCenter(info.center)
	, mPropX(info.scalesX)
	, mPropY(info.scalesY)
	, mR(info.radius)
	, mInitWidth(info.initWidth)
	, mInitHeight(info.initHeight)
	, mType(info.type)
{
}

void StatCircular::paint(QPainter *painter, const QRectF &contents) const
{
	const qreal x = mCenter.x() * (mPropX ? mInitWidth : contents.width());
	const qreal y = mCenter.y() * (mPropY ? mInitHeight : contents.height());

	QPointF center(x, y);

	const qreal rx = mR * (mPropX ? mInitWidth : contents.width());
	const qreal ry = mR * (mPropX ? mInitWidth : contents.height());

	const QColor portColor("#465945");
	const QColor highlightColor("#c3dcc4");

	painter->save();
	QPen pen = painter->pen();
	pen.setColor(highlightColor);
	pen.setWidth(7);
	painter->setPen(pen);
	painter->drawEllipse(center, rx, ry);
	pen.setColor(portColor);
	pen.setWidth(1);
	painter->setPen(pen);
	painter->drawEllipse(center, rx, ry);
	painter->restore();
}

StatCircular::CircularPort StatCircular::transformForContents(const QRectF &contents) const
{
	const qreal x = mCenter.x() * (mPropX ? mInitWidth : contents.width());
	const qreal y = mCenter.y() * (mPropY ? mInitHeight : contents.height());

	const qreal rx = mR * (mPropX ? mInitWidth : contents.width());
	const qreal ry = mR * (mPropX ? mInitWidth : contents.height());

	CircularPort port;
	port.x = x;
	port.y = y;
	port.rx = rx;
	port.ry = ry;
	return port;
}

QString StatCircular::type() const
{
	return mType;
}
