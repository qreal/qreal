#include "umllib/private/brokenLine.h"

using namespace qReal;

BrokenLine::BrokenLine(EdgeElement *edge)
		: LineHandler(edge)
		, mDeletePointAction(tr("Delete point"), this)
		, mDeleteSegmentAction(tr("Delete segment"), this)
		, mMinimizeAction(tr("Remove all points"), this)
{
	connectAction(&mDeletePointAction, this, SLOT(deletePoint(QPointF const &)));
	connectAction(&mDeleteSegmentAction, this, SLOT(deleteSegment(QPointF const &)));
	connectAction(&mMinimizeAction, this, SLOT(minimize()));
}

void BrokenLine::handleEdgeMove(QPointF const &pos)
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

int BrokenLine::addPoint(QPointF const &pos)
{
	int const segmentNumber = defineSegment(pos);
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
	int const rad = kvadratik * 2;

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

bool BrokenLine::tooSmallTriangle(QPolygonF const &line, int i) const
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

QPointF BrokenLine::alignedPoint(QPointF const &point) const
{
	QPointF result = mEdge->mapToScene(point);
	int const indexGrid = SettingsManager::value("IndexGrid").toInt();

	int const coefX = static_cast<int>(result.x()) / indexGrid;
	int const coefY = static_cast<int>(result.y()) / indexGrid;

	result = QPointF(SceneGridHandler::alignedCoordinate(result.x(), coefX, indexGrid)
			, SceneGridHandler::alignedCoordinate(result.y(), coefY, indexGrid));

	return mEdge->mapFromScene(result);
}

QList<ContextMenuAction *> BrokenLine::extraActions(QPointF const &pos)
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

void BrokenLine::deletePoint(QPointF const &pos)
{
	if (delPointActionIsPossible(pos)) {
		int const pointNumber = definePoint(pos);
		QPolygonF line = mEdge->line();
		line.remove(pointNumber);
		mEdge->setLine(line);
		mEdge->layOut();
	}
}

void BrokenLine::deleteSegment(QPointF const &pos)
{
	if (delSegmentActionIsPossible(pos)) {
		int const segmentNumber = defineSegment(pos);
		if (segmentNumber >= 0) {
			deletePoint(mEdge->line()[segmentNumber]);
			deletePoint(mEdge->line()[segmentNumber]);
		}
	}
}

bool BrokenLine::delPointActionIsPossible(QPointF const &pos) const
{
	if (mEdge->isLoop()) {
		return false;
	}

	int const pointIndex = definePoint(pos);
	return (pointIndex > 0) && (pointIndex < mEdge->line().count() - 1);
}

bool BrokenLine::delSegmentActionIsPossible(const QPointF &pos)
{
	if (mEdge->isLoop()) {
		return false;
	}

	mSavedLine = mEdge->line();
	int const segment = defineSegment(pos);
	return (segment > 0) && (segment < mEdge->line().count() - 2);
}

bool BrokenLine::minimizeActionIsPossible() const
{
	return !mEdge->isLoop() && (mEdge->line().count() > 2);
}
