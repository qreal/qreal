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

#include "editor/private/brokenLine.h"

using namespace qReal;
using namespace qReal::gui::editor;

BrokenLine::BrokenLine(EdgeElement *edge)
		: LineHandler(edge)
		, mDeletePointAction(tr("Delete point"), this)
		, mDeleteSegmentAction(tr("Delete segment"), this)
		, mMinimizeAction(tr("Remove all points"), this)
{
	connectAction(&mDeletePointAction, this, SLOT(deletePoint(const QPointF &)));
	connectAction(&mDeleteSegmentAction, this, SLOT(deleteSegment(const QPointF &)));
	connectAction(&mMinimizeAction, this, SLOT(minimize()));
}

void BrokenLine::handleEdgeMove(const QPointF &pos)
{
	if (mDragType == EdgeElement::noPort) {
		mDragType = addPoint(mDragStartPoint);
	}

	QPolygonF line = mEdge->line();
	if (mDragType >= 0) {
		line[mDragType] = SettingsManager::value("ActivateGrid").toBool() ? alignedPoint(pos) : pos;
	}
	mEdge->setLine(line);
}

int BrokenLine::addPoint(const QPointF &pos)
{
	const int segmentNumber = defineSegment(pos);
	if (segmentNumber >= 0) {
		QPolygonF line = mEdge->line();
		line.insert(segmentNumber + 1, pos);
		mEdge->setLine(line);
		mDragType = segmentNumber + 1;
	}

	return mDragType;
}

void BrokenLine::improveAppearance()
{
	deleteUnneededPoints();
	deleteLoops();
}

void BrokenLine::deleteUnneededPoints()
{
	if (mEdge->isLoop()) {
		return;
	}

	QPolygonF line = mEdge->line();

	deleteClosePoints(line);
	removeSmallTriangles(line);

	mEdge->setLine(line);
}

void BrokenLine::deleteClosePoints(QPolygonF &line) const
{
	const int rad = kvadratik * 2;

	for (int i = 0; i < line.size() - 1; i++) {
		if (QLineF(line[i], line[i + 1]).length() < rad) {
			if (i != line.size() - 2) {
				line.remove(i + 1);
				i--;
			} else if (i != 0) {
				line.remove(i);
				i = i - 2;
			}
		}
	}
}

void BrokenLine::removeSmallTriangles(QPolygonF &line) const
{
	for (int i = 0; i < line.size() - 2; i++) {
		if (tooSmallTriangle(line, i)) {
			line.remove(i + 1);
			i--;
		}
	}
}

bool BrokenLine::tooSmallTriangle(const QPolygonF &line, int i) const
{
	QPainterPath line1(line[i]);
	line1.lineTo(line[i + 1]);

	QPainterPath line2(line[i + 1]);
	line2.lineTo(line[i + 2]);

	QPainterPath line3(line[i]);
	line3.lineTo(line[i + 2]);

	QPainterPathStroker neighbourhood;
	neighbourhood.setWidth(kvadratik * 4);

	return neighbourhood.createStroke(line1).contains(line[i + 2])
			|| neighbourhood.createStroke(line2).contains(line[i])
			|| neighbourhood.createStroke(line3).contains(line[i + 1]);
}

void BrokenLine::alignToGrid()
{
	QPolygonF line = mEdge->line();

	for (int i = 1; i < line.size() - 1; ++i) {
		line[i] = alignedPoint(line[i]);
	}

	mEdge->setLine(line);
}

QPointF BrokenLine::alignedPoint(const QPointF &point) const
{
	QPointF result = mEdge->mapToScene(point);
	const int indexGrid = SettingsManager::value("IndexGrid").toInt();

	const int coefX = static_cast<int>(result.x()) / indexGrid;
	const int coefY = static_cast<int>(result.y()) / indexGrid;

	result = QPointF(SceneGridHandler::alignedCoordinate(result.x(), coefX, indexGrid)
			, SceneGridHandler::alignedCoordinate(result.y(), coefY, indexGrid));

	return mEdge->mapFromScene(result);
}

QList<ContextMenuAction *> BrokenLine::extraActions(const QPointF &pos)
{
	QList<ContextMenuAction *> result;

	if (delPointActionIsPossible(pos)) {
		result << &mDeletePointAction;
	}

	if (delSegmentActionIsPossible(pos)) {
		result << &mDeleteSegmentAction;
	}

	if (minimizeActionIsPossible()) {
		result << &mMinimizeAction;
	}

	return result;
}

void BrokenLine::deletePoint(const QPointF &pos)
{
	if (delPointActionIsPossible(pos)) {
		const int pointNumber = definePoint(pos);
		QPolygonF line = mEdge->line();
		line.remove(pointNumber);
		mEdge->setLine(line);
		mEdge->layOut();
	}
}

void BrokenLine::deleteSegment(const QPointF &pos)
{
	if (delSegmentActionIsPossible(pos)) {
		const int segmentNumber = defineSegment(pos);
		if (segmentNumber >= 0) {
			deletePoint(mEdge->line()[segmentNumber]);
			deletePoint(mEdge->line()[segmentNumber]);
		}
	}
}

bool BrokenLine::delPointActionIsPossible(const QPointF &pos) const
{
	if (mEdge->isLoop()) {
		return false;
	}

	const int pointIndex = definePoint(pos);
	return (pointIndex > 0) && (pointIndex < mEdge->line().count() - 1);
}

bool BrokenLine::delSegmentActionIsPossible(const QPointF &pos)
{
	if (mEdge->isLoop()) {
		return false;
	}

	mSavedLine = mEdge->line();
	const int segment = defineSegment(pos);
	return (segment > 0) && (segment < mEdge->line().count() - 2);
}

bool BrokenLine::minimizeActionIsPossible() const
{
	return !mEdge->isLoop() && (mEdge->line().count() > 2);
}
