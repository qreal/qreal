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

#include "linePort.h"

LinePort::LinePort(qreal x1, qreal y1, qreal x2, qreal y2, Line* parent)
	:Line(x1, y1, x2, y2, parent), mType("NonTyped")
{
	mNeedScalingRect = true;
	setPen(QPen(Qt::blue));
	mDomElementType = portType;
	setX1(x1);
	setY1(y1);
	setX2(x2);
	setY2(y2);
}

LinePort::LinePort(const LinePort &other)
	:Line(other)
{
	mNeedScalingRect = other.mNeedScalingRect ;
	setPen(other.pen());
	setBrush(other.brush());
	mDomElementType = portType;
	setX1(other.x1());
	setX2(other.x2());
	setY1(other.y1());
	setY2(other.y2());
	mListScalePoint = other.mListScalePoint;
	mType = other.mType;
	setPos(other.x(), other.y());
}

Item* LinePort::clone()
{
	LinePort* item = new LinePort(*this);
	return item;
}

QPair<QDomElement, Item::DomElementTypes> LinePort::generateItem(QDomDocument &document, const QPoint &topLeftPicture)
{
	QDomElement linePort = document.createElement("linePort");

	const qreal x1 = scenePos().x() + line().x1() - topLeftPicture.x();
	const qreal y1 = scenePos().y() + line().y1() - topLeftPicture.y();
	const qreal x2 = scenePos().x() + line().x2() - topLeftPicture.x();
	const qreal y2 = scenePos().y() + line().y2() - topLeftPicture.y();

	QPair<QPair<QString, QString>, QPair<QString, QString> > res = setXandYBefore(QRectF(x1, y1
			, x2 - x1, y2 - y1).normalized().toRect());
	QDomElement start  = document.createElement("start");
	linePort.appendChild(start);
	start.setAttribute("starty", res.first.second);
	start.setAttribute("startx", res.first.first);

	QDomElement end  = document.createElement("end");
	linePort.appendChild(end);
	end.setAttribute("endy", res.second.second);
	end.setAttribute("endx", res.second.first);

	linePort.setAttribute("type", mType);

	return QPair<QDomElement, Item::DomElementTypes>(linePort, mDomElementType);
}

void LinePort::setType(const QString &type)
{
	mType = type;
}

QString LinePort::getType() const
{
	return mType;
}
