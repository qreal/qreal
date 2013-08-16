#include "squareLine.h"

namespace qReal {

SquareLine::SquareLine(EdgeElement *edge)
	: LineHandler(edge)
{
}

void SquareLine::moveEdge(QPointF const &pos, bool needAlign)
{
	Q_UNUSED(needAlign)
	QPolygonF line = mEdge->line();

	if (mDragType == EdgeElement::noPort) {
		moveSegment(pos);
		return;
	} else if (mDragType == 0 || mDragType == mEdge->line().size() - 1) {
		line[mDragType] = pos;
		mEdge->setLine(line);
		mEdge->squarize();
	}

	mEdge->update();
}

void SquareLine::adjust()
{
	LineHandler::adjust();
	if (!mEdge->isLoop()) {
		mEdge->squarize();
	}
}

void SquareLine::improveAppearance()
{
	mEdge->delCloseLinePoints();
	mEdge->deleteLoops();
	mEdge->squarize();
}

void SquareLine::moveSegment(QPointF const &pos)
{
	int segmentNumber = defineSegment(pos);
	if (segmentNumber <= 0 || segmentNumber >= mEdge->line().count() - 2) {
		return;
	}

	QPolygonF line = mSavedLine;
	QLineF segment(line[segmentNumber], line[segmentNumber + 1]);
	QPointF offset(pos - line[segmentNumber]);

	if (segment.x1() == segment.x2()) {
		offset.setY(0);
	}

	if (segment.y1() == segment.y2()) {
		offset.setX(0);
	}

	line[segmentNumber] += offset;
	line[segmentNumber + 1] += offset;
	mEdge->setLine(line);
	mEdge->update();
}
}
