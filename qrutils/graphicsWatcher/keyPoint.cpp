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

#include "keyPoint.h"

using namespace utils::sensorsGraph;

KeyPoint::KeyPoint(const QColor color)
	: mVisualArea(QRectF(-10, -10, 10, 10))
	, mColor(color)
{
	setCacheMode(NoCache);
	setZValue(1000);
}

QRectF KeyPoint::boundingRect() const
{
	return mVisualArea;
}

void KeyPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)

	painter->save();
	painter->setPen(Qt::NoPen);

	QRadialGradient gradient(0, 0, 10);

	gradient.setCenter(0, 0);
	gradient.setFocalPoint(0, 0);
	gradient.setColorAt(1, mColor.light(120));
	gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
	painter->setBrush(gradient);

	painter->setPen(Qt::NoPen);
	painter->drawEllipse(mVisualArea);
	painter->restore();
}
