#include "brokenLine.h"

namespace qReal {

BrokenLine::BrokenLine(EdgeElement *edge)
		: LineHandler(edge)
		, mDeletePointAction(tr("Delete point"), this)
		, mDeleteSegmentAction(tr("Delete segment"), this)
		, mMinimizeAction(tr("Remove all points"), this)
{
	connect(&mDeletePointAction, SIGNAL(triggered(QPointF const &)), this, SLOT(deletePoint(QPointF const &)));
	connect(&mDeleteSegmentAction, SIGNAL(triggered(QPointF const &)), this, SLOT(deleteSegment(QPointF const &)));
	connect(&mMinimizeAction, SIGNAL(triggered(QPointF const &)), this, SLOT(minimize()));
}

void BrokenLine::handleEdgeMove(QPointF const &pos, bool needAlign)
{
	QPolygonF line = mEdge->line();

	if (mDragType == EdgeElement::noPort) {
		mDragType = addPoint(mDragStartPoint);
	}

	line = mEdge->line();
	line[mDragType] = needAlign ? alignedPoint(pos) : pos;
	mEdge->setLine(line);

	mEdge->update();
}

void BrokenLine::improveAppearance()
{
	mEdge->delCloseLinePoints();
	mEdge->deleteLoops();
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
		int pointNumber = definePoint(pos);
		QPolygonF line = mEdge->line();
		line.remove(pointNumber);
		mEdge->setLine(line);
		mEdge->layOut();
	}
}

void BrokenLine::deleteSegment(QPointF const &pos)
{
	if (delSegmentActionIsPossible(pos)) {
		int segmentNumber = defineSegment(pos);
		deletePoint(mEdge->line()[segmentNumber]);
		deletePoint(mEdge->line()[segmentNumber]);
	}
}

bool BrokenLine::delPointActionIsPossible(QPointF const &pos) const
{
	if (mEdge->isLoop()) {
		return false;
	}

	int pointIndex = definePoint(pos);
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

}
