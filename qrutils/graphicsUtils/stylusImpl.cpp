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

#include "stylusImpl.h"

using namespace graphicsUtils;

StylusImpl::StylusImpl()
{
}

QRectF StylusImpl::searchMaxMinCoord(QList<AbstractItem *> listLine) const
{
	if (!listLine.isEmpty()) {
		QRectF lineBounds = (listLine.at(0))->realBoundingRect();
		qreal maxX = lineBounds.right();
		qreal minX = lineBounds.left();
		qreal maxY = lineBounds.bottom();
		qreal minY = lineBounds.top();
		foreach (AbstractItem *line, listLine) {
			lineBounds = line->realBoundingRect();
			minX = qMin(lineBounds.left(), minX);
			minY = qMin(lineBounds.top(), minY);
			maxX = qMax(lineBounds.right(), maxX);
			maxY = qMax(lineBounds.bottom(), maxY);
		}
		return QRectF(minX, minY, maxX - minX, maxY - minY);
	} else {
		return QRectF(0, 0, 0, 0);
	}
}

QPainterPath StylusImpl::shape(QList<AbstractItem *> listLine) const
{
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	foreach (AbstractItem *line, listLine) {
		path.addPath(line->shape());
	}
	return path;
}

QRectF StylusImpl::boundingRect(QList<AbstractItem *> listLine) const
{
	return searchMaxMinCoord(listLine);
}

void StylusImpl::drawItem(QList<AbstractItem *> listLine, QPainter* painter, const QStyleOptionGraphicsItem* option
		, QWidget* widget)
{
	foreach (AbstractItem *line, listLine) {
		painter->setPen(line->pen());
		painter->setBrush(line->brush());
		line->drawItem(painter, option, widget);
	}
}

void StylusImpl::drawExtractionForItem(QList<AbstractItem *> listLine, QPainter* painter)
{
	QRectF rect = boundingRect(listLine);
	painter->drawPoint(rect.left(), rect.top());
	painter->drawPoint(rect.left(), rect.bottom());
	painter->drawPoint(rect.right(), rect.top());
	painter->drawPoint(rect.right(), rect.bottom());
}

void StylusImpl::drawFieldForResizeItem(QPainter* painter)
{
	Q_UNUSED(painter);
}

void StylusImpl::drawScalingRects(QPainter* painter)
{
	Q_UNUSED(painter);
}

void StylusImpl::setPenStyle(QList<AbstractItem *> listLine, const QString &text)
{
	foreach (AbstractItem *line, listLine)
		line->setPenStyle(text);
}

void StylusImpl::setPenWidth(QList<AbstractItem *> listLine, int width)
{
	foreach (AbstractItem *line, listLine)
		line->setPenWidth(width);
}

void StylusImpl::setPenColor(QList<AbstractItem *> listLine, const QString &text)
{
	foreach (AbstractItem *line, listLine)
		line->setPenColor(text);
}

void StylusImpl::setBrushStyle(QList<AbstractItem *> listLine, const QString &text)
{
	foreach (AbstractItem *line, listLine)
		line->setBrushStyle(text);
}

void StylusImpl::setBrushColor(QList<AbstractItem *> listLine, const QString &text)
{
	foreach (AbstractItem *line, listLine)
		line->setBrushColor(text);
}

void StylusImpl::resizeItem(QGraphicsSceneMouseEvent *event)
{
	Q_UNUSED(event);
}
