#include "lineHandler.h"

#include "nodeElement.h"
#include "../controller/controller.h"

namespace qReal {

LineHandler::LineHandler(EdgeElement *edge, LineType type)
		: mEdge(edge), mType(type), mReshapeCommand(NULL)
{}

void LineHandler::startMovingEdge(int dragType, QPointF const &pos)
{
	mReshapeCommand = new commands::ReshapeEdgeCommand(static_cast<EditorViewScene *>(mEdge->scene()), mEdge->id());
	mReshapeCommand->startTracking();

	mSavedLine = mEdge->line();
	mDragType = dragType;
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
	delete mReshapeCommand;
	mEdge->setLine(mSavedLine);
}

void LineHandler::endMovingEdge()
{
	if ((mDragType == 0) || (mDragType == mEdge->line().count() - 1)) {
		bool isStart = mDragType == 0;
		if (nodeChanged(isStart)) {
			layOut();
		} else if (!checkPort(isStart ? mEdge->line().first() : mEdge->line().last(), isStart)) {
			rejectMovingEdge();
			return;
		} else {
			mEdge->connectToPort();
			mEdge->reconnectToNearestPorts(!isStart, isStart);

			if (mEdge->src()) {
				mEdge->src()->arrangeLinearPorts();
			}
			if (mEdge->dst()) {
				mEdge->dst()->arrangeLinearPorts();
			}

			adjust();
			improveAppearance();
		}
	} else {
		layOut();
	}

	if (mReshapeCommand) {
		mReshapeCommand->stopTracking();
		mEdge->controller()->execute(mReshapeCommand);
		mReshapeCommand = NULL;
	}

	mDragType = EdgeElement::noPort;
}

void LineHandler::adjust()
{
	QPolygonF line = mEdge->line();
	NodeElement *src = mEdge->src();
	NodeElement *dst = mEdge->dst();

	if (src) {
		line.first() = mEdge->mapFromItem(src, src->portPos(mEdge->fromPort()));
	}

	if (dst) {
		line.last() = mEdge->mapFromItem(dst, dst->portPos(mEdge->toPort()));
	}

	mEdge->setLine(line);

	if (mEdge->isLoop()) {
		mEdge->createLoopEdge();
		return;
	}

	if (mType == static_cast<int>(squareLine) && !mEdge->isLoop()) {
		mEdge->squarize();
	}
}

void LineHandler::layOut()
{
	if (mDragType == 0 || mDragType == mEdge->line().count() - 1) {
		mEdge->connectToPort();
	}
	if (mEdge->src()) {
		mEdge->src()->arrangeLinks();
		mEdge->src()->adjustLinks();
	}
	if (mEdge->dst()) {
		mEdge->dst()->arrangeLinks();
		mEdge->dst()->adjustLinks();
	}

	improveAppearance();
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
	ps.setWidth(kvadratik);
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

void LineHandler::improveAppearance()
{
	switch (mType) {
	case brokenLine:
		mEdge->delCloseLinePoints();
		mEdge->deleteLoops();
		break;
	case squareLine:
		mEdge->delCloseLinePoints();
		mEdge->deleteLoops();
		mEdge->squarize();
		break;
	case curveLine:
		break;
	}
}

bool LineHandler::checkPort(QPointF const &pos, bool isStart) const
{
	NodeElement *node = dynamic_cast<NodeElement *>(mEdge->getNodeAt(pos, isStart));
	if (!node) {
		return true;
	}

	qreal port = node->portId(mEdge->mapToItem(node, pos), isStart ? mEdge->fromPortTypes() : mEdge->toPortTypes());
	if (port < 0) {
		return true;
	}

	QPointF point = mEdge->mapFromItem(node, node->portPos(port));
	QRectF rect(point - QPointF(kvadratik, kvadratik), point + QPointF(kvadratik, kvadratik));
	return rect.contains(pos);
}

bool LineHandler::nodeChanged(bool isStart) const
{
	NodeElement *node = dynamic_cast<NodeElement *>(mEdge->getNodeAt(isStart
			? mEdge->line().first() : mEdge->line().last(), isStart));
	return isStart ? (node != mEdge->src()) : (node != mEdge->dst());
}

}
