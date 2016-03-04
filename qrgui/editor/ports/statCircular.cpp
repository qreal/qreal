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

#include "statCircular.h"

using namespace qReal::gui::editor;

StatCircular::StatCircular(const QPointF &point, const qreal &r,bool propX, bool propY, int initWidth
		, int initHeight, PortImpl *impl)
	: PortInterface(impl), mCircular(point), mR(r), mPropX(propX), mPropY(propY)
		, mInitWidth(initWidth), mInitHeight(initHeight)
{}

void StatCircular::paint(QPainter *painter, const QRectF &contents) const
{
	const qreal x = mCircular.x() * (mPropX ? mInitWidth : contents.width());
	const qreal y = mCircular.y() * (mPropY ? mInitHeight : contents.height());

	QPointF center(x, y);

	const qreal r1 = mR * (mPropX ? mInitWidth : contents.width());
	const qreal r2 = mR * (mPropX ? mInitWidth : contents.height());

	const QColor portColor("#465945");
	const QColor highlightColor("#c3dcc4");

	painter->save();
	QPen pen = painter->pen();
	pen.setColor(highlightColor);
	pen.setWidth(7);
	painter->setPen(pen);
	painter->drawEllipse(center, r1, r2);
	pen.setColor(portColor);
	pen.setWidth(1);
	painter->setPen(pen);
	painter->drawEllipse(center, r1, r2);
	painter->restore();
}

QLineF StatCircular::transformForContents(const QRectF &contents) const
{
	const qreal x = mCircular.x() * (mPropX ? mInitWidth : contents.width());
	const qreal y = mCircular.y() * (mPropY ? mInitHeight : contents.height());

	const qreal r1 = mR * (mPropX ? mInitWidth : contents.width());
	const qreal r2 = mR * (mPropX ? mInitWidth : contents.height());

	return QLineF(x, y, r1, r2);

}
