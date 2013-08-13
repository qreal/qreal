#include "lineHandler.h"

namespace qReal {

LineHandler::LineHandler(EdgeElement *edge, LineType type)
		: mEdge(edge), mType(type)
{
}

void LineHandler::startMovingEdge(int dragType, QPointF const &pos)
{
	mSavedLine = mEdge->line();
	mDragType = mEdge->getPoint(pos);
	if (dragType != EdgeElement::noPort) {
		mDragType = dragType;
	} else {
		mDragType = mEdge->addPointHandler(pos);
	}
}

void LineHandler::moveEdge(QPointF const &pos, bool needAlign)
{
	if (mDragType < 0) {
			return;
	}

	QPolygonF line = mEdge->line();
	int const indexGrid = SettingsManager::value("IndexGrid").toInt();
	line[mDragType] = needAlign ? mEdge->alignedPoint(pos, indexGrid) : pos;
	mEdge->setLine(line);

	if (mType == squareLine) {
		mEdge->squarize();
	}
}

void LineHandler::rejectMovingEdge()
{
	mEdge->setLine(mSavedLine);
}

void LineHandler::endMovingEdge()
{
	mEdge->connectToPort();

	if (mType != curveLine) {
		mEdge->delCloseLinePoints();
	}

	mEdge->arrangeSrcAndDst();
	mEdge->adjustNeighborLinks();
}

}
