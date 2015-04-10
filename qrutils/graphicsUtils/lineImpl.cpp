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

#include "lineImpl.h"

using namespace graphicsUtils;

LineImpl::LineImpl()
{
}

QRectF LineImpl::boundingRect(qreal x1, qreal y1, qreal x2, qreal y2, qreal penWidth, const int drift) const
{
	return (QRectF(qMin(x1, x2) - penWidth, qMin(y1, y2) - penWidth, abs(x2 - x1) + penWidth, abs(y2 - y1) + penWidth)
			.adjusted(-drift, -drift, drift, drift));
}

QRectF LineImpl::realBoundingRectWithoutScene(qreal x1, qreal y1, qreal x2, qreal y2, qreal penWidth
		, const int drift) const
{
	return boundingRect(x1, y1, x2, y2, penWidth, drift).adjusted(drift + penWidth, drift + penWidth, -drift, -drift);
}

void LineImpl::drawItem(QPainter* painter, qreal x1, qreal y1, qreal x2, qreal y2)
{
	painter->drawLine(x1, y1, x2, y2);
}

void LineImpl::drawPointExtractionForItem(QPainter* painter, qreal x1, qreal y1, qreal x2, qreal y2)
{
	painter->drawPoint(x1, y1);
	painter->drawPoint(x2, y2);
}

void LineImpl::drawExtractionForItem(QPainter* painter, qreal x1, qreal y1, qreal x2, qreal y2, const int drift)
{
	painter->drawPath(shape(drift, x1, y1, x2, y2));
}

void LineImpl::drawFieldForResizeItem(QPainter* painter, const int resizeDrift, qreal x1, qreal y1, qreal x2, qreal y2)
{
	painter->drawEllipse(QPointF(x1, y1), resizeDrift, resizeDrift);
	painter->drawEllipse(QPointF(x2, y2), resizeDrift, resizeDrift);
}

QLineF LineImpl::line(qreal x1, qreal y1, qreal x2, qreal y2) const
{
	return QLineF(x1, y1, x2, y2);
}

QPainterPath LineImpl::shape(const int drift, qreal x1, qreal y1, qreal x2, qreal y2) const
{
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);

	QPainterPathStroker ps;
	ps.setWidth(drift);

	path.moveTo(x1, y1);
	path.lineTo(x2, y2);
	path = ps.createStroke(path);

	return path;
}

QPair<qreal, qreal> LineImpl::reshapeRectWithShiftForLine(qreal x1, qreal y1, qreal x2, qreal y2, qreal differenceX
		, qreal differenceY, qreal size)
{
	if (differenceX > differenceY) {
		if(x2 > x1)
			return QPair<qreal, qreal>(x1 + size, y1);
		else
			return QPair<qreal, qreal>(x1 - size, y1);
	} else {
		if(y2 > y1)
			return QPair<qreal, qreal>(x1, y1 + size);
		else
			return QPair<qreal, qreal>(x1, y1 - size);
	}
}
