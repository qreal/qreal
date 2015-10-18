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

#include "editor/private/curveLine.h"

using namespace qReal;
using namespace qReal::gui::editor;

CurveLine::CurveLine(EdgeElement *edge)
		: LineHandler(edge)
{
	setBezierPoints();
}

void CurveLine::handleEdgeMove(const QPointF &pos)
{
	QPolygonF line = mEdge->line();

	if (mDragType >= 0) {
		line[mDragType] = pos;
		mEdge->setLine(line);
	}
}

void CurveLine::adjust()
{
	LineHandler::adjust();
	setBezierPoints();
}

void CurveLine::drawLine(QPainter *painter, bool drawSavedLine)
{
	QPolygonF line = drawSavedLine ? mSavedLine : mEdge->line();
	painter->drawPath(bezierCurve(line));
}

void CurveLine::drawPorts(QPainter *painter)
{
	drawControlLines(painter);
	LineHandler::drawPorts(painter);
}

void CurveLine::drawControlLines(QPainter *painter)
{
	QPolygonF line = mEdge->line();

	QPen pen = painter->pen();
	pen.setStyle(Qt::DashLine);
	painter->save();
	painter->setPen(pen);
	painter->drawLine(line[0], line[1]);
	painter->drawLine(line[2], line[3]);
	painter->restore();
}

void CurveLine::drawPort(QPainter *painter, int portNumber)
{
	if ((portNumber == 0) || (portNumber == mEdge->line().count() - 1)) {
		LineHandler::drawPort(painter, portNumber);
		return;
	}

	QPen pen;
	pen.setCapStyle(Qt::RoundCap);
	QColor color;
	QPointF p1(-0.25, 0);
	QPointF p2(0.25, 0);

	color.setNamedColor("#ffcc66");
	pen.setWidth(12);
	pen.setColor(color);
	painter->setPen(pen);
	painter->drawLine(p1, p2);

	color.setNamedColor("#ff6666");
	pen.setWidth(3);
	pen.setColor(color);
	painter->setPen(pen);
	painter->drawLine(p1, p2);
}

QPainterPath CurveLine::shape() const
{
	return bezierCurve(mEdge->line());
}

QPainterPath CurveLine::bezierCurve(const QPolygonF &line) const
{
	QPainterPath curve(line[0]);
	curve.cubicTo(line[1], line[line.count() - 2], line[line.count() - 1]);
	return curve;
}

void CurveLine::setBezierPoints()
{
	QPolygonF line = mEdge->line();

	if (line.size() == 4) {
		return;
	}

	if (mEdge->isLoop()) {
		QPolygonF newLine;
		newLine << line[0] << line[2] << line[3] << line[5];
		line = newLine;
	}
	if (line.size() == 2) {
		QPolygonF newLine;
		newLine << line[0] << (line[1] - line[0]) / 3 << 2 * (line[1] - line[0]) / 3 << line[1];
		line = newLine;
	}
	if (line.size() == 3) {
		QPolygonF newLine;
		newLine << line[0] << line[1] << line[1] << line[2];
		line = newLine;
	}
	if (line.size() > 4) {
		QPolygonF newLine;
		newLine << line[0] << line[1] << line[line.size() - 2] << line.last();
		line = newLine;
	}

	mEdge->setLine(line);
}
