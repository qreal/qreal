#include "squareLine.h"

qreal const epsilon = 0.0001;
qreal const offset = 2 * kvadratik;

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
	} else if (mDragType == 0 || mDragType == mSavedLine.size() - 1) {
		line[mDragType] = pos;
		mEdge->setLine(line);
		adjustEndSegments();
	}

	mEdge->update();
}

void SquareLine::adjust()
{
	LineHandler::adjust();
	if (!mEdge->isLoop()) {
		adjustEndSegments();
	}
}

void SquareLine::adjustEndSegments()
{
	if (mEdge->line().count() == 2) {
		squarize();
	} else {
		adjustStart();
		adjustEnd();
	}
}

void SquareLine::adjustStart()
{
	QPolygonF line = mEdge->line();

	if ((qAbs(line[0].x() - line[1].x()) < epsilon) || (qAbs(line[0].y() - line[1].y()) < epsilon)) {
		return;
	}

	if (line[1] == line[2] && line.count() > 3) {
		if (qAbs(line[2].x() - line[3].x()) < qAbs(line[2].y() - line[3].y())) {
			line[1].setX(line[0].x());
		} else {
			line[1].setY(line[0].y());
		}
	} else {
		if (qAbs(line[1].y() - line[2].y()) < qAbs(line[1].x() - line[2].x())) {
			line[1].setX(line[0].x());
		} else {
			line[1].setY(line[0].y());
		}
	}

	mEdge->setLine(line);
}

void SquareLine::adjustEnd()
{
	QPolygonF line = mEdge->line();

	if ((qAbs(line[line.count() - 1].x() - line[line.count() - 2].x()) < epsilon)
			|| qAbs(line[line.count() - 1].y() - line[line.count() - 2].y()) < epsilon) {
		return;
	}

	if (line[line.count() - 2] == line[line.count() - 3] && line.count() > 3) {
		if (qAbs(line[line.count() - 3].x() - line[line.count() - 4].x())
				< qAbs(line[line.count() - 3].y() - line[line.count() - 4].y())) {
			line[line.count() - 2].setX(line[line.count() - 1].x());
		} else {
			line[line.count() - 2].setY(line[line.count() - 1].y());
		}
	} else {
		if (qAbs(line[line.count() - 2].y() - line[line.count() - 3].y())
				< qAbs(line[line.count() - 2].x() - line[line.count() - 3].x())) {
			line[line.count() - 2].setX(line[line.count() - 1].x());
		} else {
			line[line.count() - 2].setY(line[line.count() - 1].y());
		}
	}

	mEdge->setLine(line);
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

void SquareLine::improveAppearance()
{
//	mEdge->delCloseLinePoints();
//	mEdge->deleteLoops();
	if (needCorrect()) {
		squarize();
	}
}

bool SquareLine::needCorrect() const
{
	return needCorrectSrc() || needCorrectDst();
}

bool SquareLine::needCorrectSrc() const
{
	if (!mEdge->src()) {
		return false;
	}

	QPolygonF line = mEdge->line();
	int const srcPoint = firstOutsidePoint(true);

	return ((srcPoint != 1) || intersects(QLineF(line[0], line[1]), mEdge->src()));
}

bool SquareLine::needCorrectDst() const
{
	if (!mEdge->dst()) {
		return false;
	}

	QPolygonF const line = mEdge->line();
	int const dstPoint = firstOutsidePoint(false);

	return ((dstPoint != line.count() - 2)
			|| intersects(QLineF(line[line.count() - 2], line[line.count() - 1]), mEdge->dst()));
}

bool SquareLine::intersects(QLineF const &line, NodeElement *node) const
{
	bool const isLineVertical = qAbs(line.x1() - line.x2()) < epsilon;
	int const side = mEdge->defineNodePortSide(node == mEdge->src());

	if ((isLineVertical && ((side == EdgeElement::left) || (side == EdgeElement::right)))
			|| (!isLineVertical && ((side == EdgeElement::top) || (side == EdgeElement::bottom)))) {
		return true;
	}

	QRectF rect(mEdge->mapFromItem(node, node->contentsRect()).boundingRect());

	QLineF firstLine(rect.topLeft(), isLineVertical ? rect.topRight() : rect.bottomLeft());
	QLineF secondLine(isLineVertical ? rect.bottomLeft() : rect.topRight(), rect.bottomRight());

	if ((side == EdgeElement::top) || (side == EdgeElement::left)) {
		return line.intersect(secondLine, 0) == QLineF::BoundedIntersection;
	} else {
		return line.intersect(firstLine, 0) == QLineF::BoundedIntersection;
	}
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
	{
		int const srcSide = mEdge->defineNodePortSide(true);
		int const dstSide = mEdge->defineNodePortSide(false);
		if (srcSide == dstSide) {
			verticalSameSide(line, srcSide == EdgeElement::top);
		} else {
			verticalSquareLine(line, (line.first().y() + line.last().y()) / 2);
		}
		break;
	}
	case horizontal:
	{
		int const srcSide = mEdge->defineNodePortSide(true);
		int const dstSide = mEdge->defineNodePortSide(false);
		if (srcSide == dstSide) {
			horizontalSameSide(line, srcSide == EdgeElement::left);
		} else {
			horizontalSquareLine(line, (line.first().x() + line.last().x()) / 2);
		}
		break;
	}
	case verticalTurn:
		verticalTurningSquareLine(line);
		mEdge->setLine(line);
		if (needCorrectSrc()) {
			QPolygonF innerLine;
			innerLine.append(outgoingPoint(true));
			if (needCorrectDst()) {
				innerLine.append(outgoingPoint(false));
				horizontalTurningSquareLine(innerLine);
				innerLine.insert(0, line.first());
				innerLine.append(line.last());
				line = innerLine;
			} else if (mEdge->dst()){
				QPair<qreal, qreal> space = horizontalIntermediateSpace();
				innerLine.append(line.last());
				if (space.first < space.second) {
					horizontalSquareLine(innerLine, (space.first + space.second) / 2);
				} else {
					horizontalSameSide(innerLine, mEdge->defineNodePortSide(false) == EdgeElement::left);
				}
				innerLine.insert(0, line.first());
				line = innerLine;
			}
		} else if (needCorrectDst() && mEdge->src()) {
			QPolygonF innerLine;
			innerLine.append(line.first());
			innerLine.append(outgoingPoint(false));
			QPair<qreal, qreal> space = verticalIntermediateSpace();
			if (space.first < space.second) {
				verticalSquareLine(innerLine, (space.first + space.second) / 2);
			} else {
				verticalSameSide(innerLine, mEdge->defineNodePortSide(true) == EdgeElement::top);
			}
			innerLine.append(line.last());
			line = innerLine;
		}
		break;
	case horizontalTurn:
		horizontalTurningSquareLine(line);
		mEdge->setLine(line);
		if (needCorrectSrc()) {
			QPolygonF innerLine;
			innerLine.append(outgoingPoint(true));
			if (needCorrectDst()) {
				innerLine.append(outgoingPoint(false));
				verticalTurningSquareLine(innerLine);
				innerLine.insert(0, line.first());
				innerLine.append(line.last());
				line = innerLine;
			} else if (mEdge->dst()){
				innerLine.append(line.last());
				QPair<qreal, qreal> space = verticalIntermediateSpace();
				if (space.first < space.second) {
					verticalSquareLine(innerLine, (space.first + space.second) / 2);
				} else {
					verticalSameSide(innerLine, mEdge->defineNodePortSide(false) == EdgeElement::top);
				}
				innerLine.insert(0, line.first());
				line = innerLine;
			}
		} else if (needCorrectDst() && mEdge->src()) {
			QPolygonF innerLine;
			innerLine.append(line.first());
			innerLine.append(outgoingPoint(false));
			QPair<qreal, qreal> space = horizontalIntermediateSpace();
			if (space.first < space.second) {
				horizontalSquareLine(innerLine, (space.first + space.second) / 2);
			} else {
				horizontalSameSide(innerLine, mEdge->defineNodePortSide(true) == EdgeElement::left);
			}
			innerLine.append(line.last());
			line = innerLine;
		}
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

void SquareLine::horizontalSquareLine(QPolygonF &line, qreal middle)
{
	QPointF insertPoint1 = line.first();
	QPointF insertPoint2 = line.last();

	insertPoint1.setX(middle);
	insertPoint2.setX(insertPoint1.x());

	line.insert(1, insertPoint1);
	line.insert(2, insertPoint2);
}

void SquareLine::verticalSquareLine(QPolygonF &line, qreal middle)
{
	QPointF insertPoint1 = line.first();
	QPointF insertPoint2 = line.last();

	insertPoint1.setY(middle);
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

void SquareLine::horizontalSameSide(QPolygonF &line, bool leftLoop)
{
	QRectF srcRect = mEdge->mapFromItem(mEdge->src(), mEdge->src()->contentsRect()).boundingRect();
	QRectF dstRect = mEdge->mapFromItem(mEdge->dst(), mEdge->dst()->contentsRect()).boundingRect();

	qreal const loopLine = leftLoop ? qMin(srcRect.left(), dstRect.left()) - offset
			: qMax(srcRect.right(), dstRect.right()) + offset;
	horizontalSquareLine(line, loopLine);
}

void SquareLine::verticalSameSide(QPolygonF &line, bool topLoop)
{
	QRectF srcRect = mEdge->mapFromItem(mEdge->src(), mEdge->src()->contentsRect()).boundingRect();
	QRectF dstRect = mEdge->mapFromItem(mEdge->dst(), mEdge->dst()->contentsRect()).boundingRect();

	qreal const loopLine = topLoop ? qMin(srcRect.top(), dstRect.top()) - offset
			: qMax(srcRect.bottom(), dstRect.bottom()) + offset;
	verticalSquareLine(line, loopLine);
}

QPair<qreal, qreal> SquareLine::horizontalIntermediateSpace() const
{
	QRectF srcRect = mEdge->mapFromItem(mEdge->src(), mEdge->src()->contentsRect()).boundingRect();
	QRectF dstRect = mEdge->mapFromItem(mEdge->dst(), mEdge->dst()->contentsRect()).boundingRect();

	if (srcRect.right() < dstRect.left()) {
		return qMakePair(srcRect.right(), dstRect.left());
	} else {
		return qMakePair(dstRect.right(), srcRect.left());
	}
}

QPair<qreal, qreal> SquareLine::verticalIntermediateSpace() const
{
	QRectF srcRect = mEdge->mapFromItem(mEdge->src(), mEdge->src()->contentsRect()).boundingRect();
	QRectF dstRect = mEdge->mapFromItem(mEdge->dst(), mEdge->dst()->contentsRect()).boundingRect();

	if (srcRect.bottom() < dstRect.top()) {
		return qMakePair(srcRect.bottom(), dstRect.top());
	} else {
		return qMakePair(dstRect.bottom(), srcRect.top());
	}
}

QPointF SquareLine::outgoingPoint(bool isStart) const
{
	QPolygonF const line = mEdge->line();
	int const side = mEdge->defineNodePortSide(isStart);

	QPointF outGoing = isStart ? line.first() : line.last();

	switch (side) {
	case EdgeElement::top:
		outGoing.setY(outGoing.y() - offset);
		break;
	case EdgeElement::bottom:
		outGoing.setY(outGoing.y() + offset);
		break;
	case EdgeElement::left:
		outGoing.setX(outGoing.x() - offset);
		break;
	case EdgeElement::right:
		outGoing.setX(outGoing.x() + offset);
		break;
	default:
		;
	}

	return outGoing;
}

QPointF SquareLine::portArrangePoint(bool startFromSrc) const
{
	QPolygonF line = mEdge->line();
	return startFromSrc ? mEdge->mapToItem(mEdge->src(), line[line.count() - 1])
			: mEdge->mapToItem(mEdge->dst(), line[0]);
}

}
