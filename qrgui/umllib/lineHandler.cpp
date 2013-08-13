#include "lineHandler.h"

namespace qReal {

LineHandler::LineHandler(EdgeElement *edge, LineType type)
		: mEdge(edge), mType(type)
{}

void LineHandler::startMovingEdge(QPointF const &pos)
{
	mSavedLine = mEdge->line();
	mDragType = mEdge->getPoint(pos);
	mDragStartPoint = pos;
}

void LineHandler::moveEdge(QPointF const &pos, bool needAlign)
{
	QPolygonF line = mEdge->line();
	int const indexGrid = SettingsManager::value("IndexGrid").toInt();

	switch (mType) {
	case brokenLine:
		if (mDragType == EdgeElement::noPort) {
			mDragType = addPoint(mDragStartPoint);
		}

		line = mEdge->line();
		line[mDragType] = needAlign ? mEdge->alignedPoint(pos, indexGrid) : pos;
		mEdge->setLine(line);
		break;
	case squareLine:
		if (mDragType == EdgeElement::noPort) {
			moveSegment(pos);
			return;
		} else if (mDragType == 0 || mDragType == mEdge->line().size() - 1) {
				line[mDragType] = pos;
				mEdge->setLine(line);
				mEdge->squarize();
		}
		break;
	case curveLine:
		if (mDragType >= 0) {
			line[mDragType] = pos;
			mEdge->setLine(line);
		}
	}
	mEdge->update();
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

int LineHandler::addPoint(QPointF const &pos)
{
	int segmentNumber = defineSegment(pos);
	if (segmentNumber >= 0) {
		QPolygonF line = mEdge->line();
		line.insert(segmentNumber + 1, pos);
		mEdge->setLine(line);
		mDragType = segmentNumber + 1;
		mEdge->update();
	}
	return mDragType;
}

void LineHandler::setType(LineType type)
{
	mType = type;
}

int LineHandler::defineSegment(QPointF const &pos)
{
	QPainterPath path;
	QPainterPathStroker ps;
	QPolygonF line = mEdge->line();
	ps.setWidth(kvadratik); // expressly this width > width of shape() path (see #569 and #165)
	for (int i = 0; i < line.size() - 1; ++i) {
		path.moveTo(line[i]);
		path.lineTo(line[i + 1]);
		if (ps.createStroke(path).contains(pos)) {
			return i;
		}
	}
	return -1;
}

void LineHandler::moveSegment(QPointF const &pos)
{
	int segmentNumber = defineSegment(pos);
	if (segmentNumber >= 0) {
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

}
