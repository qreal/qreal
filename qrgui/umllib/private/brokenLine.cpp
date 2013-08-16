#include "brokenLine.h"

namespace qReal {

BrokenLine::BrokenLine(EdgeElement *edge)
	: LineHandler(edge)
{
}

void BrokenLine::moveEdge(QPointF const &pos, bool needAlign)
{
	QPolygonF line = mEdge->line();
	int const indexGrid = SettingsManager::value("IndexGrid").toInt();

	if (mDragType == EdgeElement::noPort) {
		mDragType = addPoint(mDragStartPoint);
	}

	line = mEdge->line();
	line[mDragType] = needAlign ? mEdge->alignedPoint(pos, indexGrid) : pos;
	mEdge->setLine(line);

	mEdge->update();
}

void BrokenLine::improveAppearance()
{
	mEdge->delCloseLinePoints();
	mEdge->deleteLoops();
}

}
