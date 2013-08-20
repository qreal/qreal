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
		moveSegment(mDragStartPoint, pos);
		return;
	} else if (mDragType == 0 || mDragType == mEdge->line().size() - 1) {
		line[mDragType] = pos;
		mEdge->setLine(line);
		squarize();
	}

	mEdge->update();
}

void SquareLine::adjust()
{
	LineHandler::adjust();
	if (!mEdge->isLoop()) {
		squarize();
	}
}

void SquareLine::handleIntersections()
{

}

void SquareLine::improveAppearance()
{
	mEdge->delCloseLinePoints();
	mEdge->deleteLoops();
	squarize();
}

void SquareLine::moveSegment(QPointF const &oldPos, QPointF const &newPos)
{
	int segmentNumber = defineSegment(oldPos);
	if (segmentNumber <= 0 || segmentNumber >= mEdge->line().count() - 2) {
		return;
	}

	QPolygonF line = mSavedLine;
	QLineF segment(line[segmentNumber], line[segmentNumber + 1]);
	QPointF offset(newPos - line[segmentNumber]);

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

void SquareLine::squarize()
{
	QPolygonF line;
	line << mEdge->line().first() << mEdge->line().last();

	// we don't need to correct the already straight line
	if (line.first().x() == line.last().x() || line.first().y() == line.last().y()) {
		mEdge->setLine(line);
		return;
	}

	int type = defineType();

	switch (type) {
	case vertical:
		verticalSquareLine(line);
		break;
	case horizontal:
		horizontalSquareLine(line);
		break;
	case verticalTurn:
		verticalTurningSquareLine(line);
		break;
	case horizontalTurn:
		horizontalTurningSquareLine(line);
		break;
	default:
		qDebug() << "incorrect link type";
	}

	mEdge->setLine(line);
}

int SquareLine::defineType()
{
	if (!(mEdge->src() && mEdge->dst())) {
		return horizontalTurn;
	}

	// defining type of square link by looking at sides of nodes which the link is connected to
	int startSide = mEdge->defineNodePortSide(true);
	int endSide = mEdge->defineNodePortSide(false);

	if (startSide == EdgeElement::top || startSide == EdgeElement::bottom) {
		if (endSide == EdgeElement::top || endSide == EdgeElement::bottom) {
			return vertical;
		} else {
			return verticalTurn;
		}
	} else if (endSide == EdgeElement::left || endSide == EdgeElement::right) {
		return horizontal;
	} else {
		return horizontalTurn;
	}
}

void SquareLine::horizontalSquareLine(QPolygonF &line)
{
	QPointF insertPoint1 = line.first();
	QPointF insertPoint2 = line.last();

	insertPoint1.setX((insertPoint1.x() + insertPoint2.x()) / 2);
	insertPoint2.setX(insertPoint1.x());

	line.insert(1, insertPoint1);
	line.insert(2, insertPoint2);
}

void SquareLine::verticalSquareLine(QPolygonF &line)
{
	QPointF insertPoint1 = line.first();
	QPointF insertPoint2 = line.last();

	insertPoint1.setY((insertPoint1.y() + insertPoint2.y()) / 2);
	insertPoint2.setY(insertPoint1.y());

	line.insert(1, insertPoint1);
	line.insert(2, insertPoint2);
}

void SquareLine::horizontalTurningSquareLine(QPolygonF &line)
{
	QPointF insertPoint = line.first();
	insertPoint.setX(line.last().x());
	line.insert(1, insertPoint);
}

void SquareLine::verticalTurningSquareLine(QPolygonF &line)
{
	QPointF insertPoint = line.first();
	insertPoint.setY(line.last().y());
	line.insert(1, insertPoint);
}

}
