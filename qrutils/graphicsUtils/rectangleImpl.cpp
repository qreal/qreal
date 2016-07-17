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

#include "rectangleImpl.h"

using namespace graphicsUtils;

RectangleImpl::RectangleImpl()
{
}

QRectF RectangleImpl::boundingRect(qreal x1, qreal y1, qreal x2, qreal y2, const int scalingDrift) const
{
	return QRectF(qMin(x1, x2), qMin(y1, y2), qAbs(x2 - x1), qAbs(y2 - y1)).adjusted(-scalingDrift, -scalingDrift
			, scalingDrift, scalingDrift);
}

QPainterPath RectangleImpl::shape(qreal x1, qreal y1, qreal x2, qreal y2, const int drift) const
{
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);

	QPainterPathStroker ps;
	ps.setWidth(drift);

	path.addRect(boundingRect(x1, y1, x2, y2, drift));
	path = ps.createStroke(path);

	return path;
}

QRectF RectangleImpl::calcRect(qreal x1, qreal y1, qreal x2, qreal y2) const
{
	if(x2 > x1) {
		if (y2 > y1)
			return QRectF(x1, y1, x2 - x1, y2 - y1);
		else
			return QRectF(x1, y2, x2 - x1, y1 - y2);
	} else {
		if (y2 > y1)
			return QRectF(x2, y1, x1 - x2, y2 - y1);
		else
			return QRectF(x2, y2, x1 - x2, y1 - y2);
	}
}

void RectangleImpl::drawRectItem(QPainter *painter, qreal x1, qreal y1, qreal x2, qreal y2)
{
	painter->drawRect(calcRect(x1, y1, x2, y2));
}

void RectangleImpl::drawEllipseItem(QPainter *painter, qreal x1, qreal y1, qreal x2, qreal y2)
{
	painter->drawEllipse(calcRect(x1, y1, x2, y2));
}

void RectangleImpl::drawImageItemWithMirrored(QPainter *painter, qreal x1, qreal y1, qreal x2, qreal y2
		, const QImage &myImage)
{
	QImage image(myImage);
	if(x2 > x1) {
		if (y2 < y1)
			image = myImage.mirrored(false, true);
	} else {
		if (y2 > y1)
			image = myImage.mirrored(true, false);
		else
			image = myImage.mirrored(true, true);
	}
	painter->drawImage(QRectF(qMin(x1, x2), qMin(y1, y2), qAbs(x2 - x1), qAbs(y2 - y1)), image);
}

void RectangleImpl::drawImageItem(QPainter *painter, qreal x1, qreal y1, qreal x2, qreal y2, const QImage &myImage)
{
	painter->drawImage(QRectF(qMin(x1, x2), qMin(y1, y2), qAbs(x2 - x1), qAbs(y2 - y1)), myImage);
}
