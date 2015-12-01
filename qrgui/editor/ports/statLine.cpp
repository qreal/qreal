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

#include "statLine.h"

using namespace qReal::gui::editor;

StatLine::StatLine(const QLineF &line, bool propX1, bool propY1, bool propX2, bool propY2
		, int initWidth, int initHeight, PortImpl *impl)
	: PortInterface(impl), mLine(line), mPropX1(propX1), mPropY1(propY1), mPropX2(propX2), mPropY2(propY2)
	, mInitWidth(initWidth), mInitHeight(initHeight)
{}

StatLine::operator QLineF () const
{
	return mLine;
}

void StatLine::operator= (const QLineF &l)
{
	mLine = l;
	mPropX1 = false;
	mPropX2 = false;
	mPropY1 = false;
	mPropY2 = false;
	mInitHeight = 1;
	mInitWidth = 1;
}

void StatLine::paint(QPainter *painter, const QRectF &contents) const
{
	const qreal x1 = mLine.x1() * (mPropX1 ? mInitWidth : contents.width());
	const qreal y1 = mLine.y1() * (mPropY1 ? mInitHeight : contents.height());

	const qreal x2 = mLine.x2() * (mPropX2 ? mInitWidth : contents.width());
	const qreal y2 = mLine.y2() * (mPropY2 ? mInitHeight : contents.height());

	QLineF lineToDraw(x1, y1, x2, y2);

	const QColor portColor("#465945");
	const QColor highlightColor("#c3dcc4");

	painter->save();
	QPen pen = painter->pen();
	pen.setColor(highlightColor);
	pen.setWidth(7);
	painter->setPen(pen);
	painter->drawLine(lineToDraw);
	pen.setColor(portColor);
	pen.setWidth(1);
	painter->setPen(pen);
	painter->drawLine(lineToDraw);
	painter->restore();
}

QLineF StatLine::transformForContents(const QRectF &contents) const
{
	const qreal x1 = mLine.x1() * (mPropX1 ? mInitWidth : contents.width());
	const qreal y1 = mLine.y1() * (mPropY1 ? mInitHeight : contents.height());

	const qreal x2 = mLine.x2() * (mPropX2 ? mInitWidth : contents.width());
	const qreal y2 = mLine.y2() * (mPropY2 ? mInitHeight : contents.height());

	return QLineF(x1, y1, x2, y2);
}
