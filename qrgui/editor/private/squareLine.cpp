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

#include "editor/private/squareLine.h"

using namespace qReal;
using namespace qReal::gui::editor;

const qreal epsilon = 0.0001;
const qreal offset = 2 * kvadratik;

SquareLine::SquareLine(EdgeElement *edge)
		: LineHandler(edge)
		, mLayOutAction(tr("Lay out"), this)
{
	connectAction(&mLayOutAction, this, SLOT(minimize()));
}

void SquareLine::handleEdgeMove(const QPointF &pos)
{
	QPolygonF line = mEdge->line();

	if (mDragType == EdgeElement::noPort) {
		moveSegment(mDragStartPoint, pos);
		return;
	} else if ((mDragType == 0) || (mDragType == mSavedLine.size() - 1)) {
		line[mDragType] = pos;
		mEdge->setLine(line);
		adjustEndSegments();
	}
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

	if ((line[1] == line[2]) && (line.count() > 3)) {
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
			|| qAbs(line[line.count() - 1].y() - line[line.count() - 2].y()) < epsilon)
	{
		return;
	}

	if ((line[line.count() - 2] == line[line.count() - 3]) && (line.count() > 3)) {
		if (qAbs(line[line.count() - 3].x() - line[line.count() - 4].x())
				< qAbs(line[line.count() - 3].y() - line[line.count() - 4].y()))
		{
			line[line.count() - 2].setX(line[line.count() - 1].x());
		} else {
			line[line.count() - 2].setY(line[line.count() - 1].y());
		}
	} else {
		if (qAbs(line[line.count() - 2].y() - line[line.count() - 3].y())
				< qAbs(line[line.count() - 2].x() - line[line.count() - 3].x()))
		{
			line[line.count() - 2].setX(line[line.count() - 1].x());
		} else {
			line[line.count() - 2].setY(line[line.count() - 1].y());
		}
	}

	mEdge->setLine(line);
}

void SquareLine::moveSegment(const QPointF &oldPos, const QPointF &newPos)
{
	const int segmentNumber = defineSegment(oldPos);
	if ((segmentNumber <= 0) || (segmentNumber >= mEdge->line().count() - 2)) {
		return;
	}

	QPolygonF line = mSavedLine;
	QLineF segment(line[segmentNumber], line[segmentNumber + 1]);
	QPointF offset(newPos - line[segmentNumber]);

	if (qAbs(segment.x1() - segment.x2()) < epsilon) {
		offset.setY(0);
	}

	if (qAbs(segment.y1() - segment.y2()) < epsilon) {
		offset.setX(0);
	}

	line[segmentNumber] += offset;
	line[segmentNumber + 1] += offset;
	mEdge->setLine(line);
}

void SquareLine::improveAppearance()
{
	if (needCorrect()) {
		squarize();
	}

	deleteLoops();
	deleteShortSegments();
}

void SquareLine::deleteShortSegments()
{
	QPolygonF line = mEdge->line();

	for (int i = 1; i < line.size() - 2; i++) {
		QLineF segment(line[i], line[i + 1]);
		if (segment.length() < epsilon) {
			line.remove(i, 2);
			i--;
		} else if (segment.length() < kvadratik * 2) {
			if (i != 1) {
				QLineF previousLine(line[i - 1], line[i]);
				if (qAbs(previousLine.x1() - previousLine.x2()) < epsilon) {
					line[i - 1] = QPointF(line[i + 1].x(), line[i - 1].y());
				} else {
					line[i - 1] = QPointF(line[i - 1].x(), line[i + 1].y());
				}

				line.remove(i, 2);
				i--;
			} else if (i != line.size() - 3) {
				QLineF nextLine(line[i + 1], line[i + 2]);
				if (qAbs(nextLine.x1() - nextLine.x2()) < epsilon) {
					line[i + 2] = QPointF(line[i].x(), line[i + 2].y());
				} else {
					line[i + 2] = QPointF(line[i + 2].x(), line[i].y());
				}

				line.remove(i, 2);
				i--;
			}
		}
	}

	mEdge->setLine(line);
}

bool SquareLine::needCorrect() const
{
	return needCorrectSrc() || needCorrectDst() || !isSquareLine();
}

bool SquareLine::needCorrectSrc() const
{
	if (!mEdge->src()) {
		return false;
	}

	QPolygonF line = mEdge->line();
	const int srcPoint = firstOutsidePoint(true);

	return ((srcPoint != 1) || intersects(QLineF(line[0], line[1]), mEdge->src()));
}

bool SquareLine::needCorrectDst() const
{
	if (!mEdge->dst()) {
		return false;
	}

	const QPolygonF line = mEdge->line();
	const int dstPoint = firstOutsidePoint(false);

	return ((dstPoint != line.count() - 2)
			|| intersects(QLineF(line[line.count() - 2], line[line.count() - 1]), mEdge->dst()));
}

bool SquareLine::intersects(const QLineF &line, NodeElement *node) const
{
	const bool isLineVertical = qAbs(line.x1() - line.x2()) < epsilon;
	const int side = mEdge->defineNodePortSide(node == mEdge->src());

	if ((isLineVertical && (side == EdgeElement::left || side == EdgeElement::right))
			|| (!isLineVertical && (side == EdgeElement::top || side == EdgeElement::bottom)))
	{
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

bool SquareLine::isSquareLine() const
{
	const QPolygonF line = mEdge->line();
	for (int i = 0; i < line.count() - 1; i++) {
		if ((qAbs(line[i].x() - line[i + 1].x()) > epsilon) && (qAbs(line[i].y() - line[i + 1].y()) > epsilon)) {
			return false;
		}
	}

	return true;
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

	LineType type = defineType();

	switch (type) {
	case vertical:
		createVerticalLine(line);
		break;
	case horizontal:
		createHorizontalLine(line);
		break;
	case verticalTurn:
		verticalTurningSquareLine(line);
		mEdge->setLine(line);
		correctLine(line, needCorrectSrc(), needCorrectDst(), true);
		break;
	case horizontalTurn:
		horizontalTurningSquareLine(line);
		mEdge->setLine(line);
		correctLine(line, needCorrectSrc(), needCorrectDst(), false);
		break;
	}

	mEdge->setLine(line);
}

SquareLine::LineType SquareLine::defineType() const
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

void SquareLine::createHorizontalLine(QPolygonF &line) const
{
	if (mEdge->defineNodePortSide(true) == mEdge->defineNodePortSide(false)) {
		horizontalSameSide(line, mEdge->defineNodePortSide(true) == EdgeElement::left);
	} else {
		horizontalSquareLine(line, (line.first().x() + line.last().x()) / 2);
	}
}

void SquareLine::createVerticalLine(QPolygonF &line) const
{
	if (mEdge->defineNodePortSide(true) == mEdge->defineNodePortSide(false)) {
		verticalSameSide(line, mEdge->defineNodePortSide(true) == EdgeElement::top);
	} else {
		verticalSquareLine(line, (line.first().y() + line.last().y()) / 2);
	}
}

void SquareLine::correctLine(QPolygonF &line, bool correctSrc, bool correctDst, bool isVertical) const
{
	if (!correctSrc && !correctDst) {
		return;
	}

	QPolygonF innerLine;
	innerLine.append(correctSrc ? outgoingPoint(true) : line.first());
	innerLine.append(correctDst ? outgoingPoint(false) : line.last());

	if (isVertical) {
		createVerticalInnerLine(innerLine, correctSrc, correctDst);
	} else {
		createHorizontalInnerLine(innerLine, correctSrc, correctDst);
	}

	if (correctSrc) {
		innerLine.insert(0, line.first());
	}
	if (correctDst) {
		innerLine.append(line.last());
	}

	line = innerLine;
}

void SquareLine::createHorizontalInnerLine(QPolygonF &line, bool correctSrc, bool correctDst) const
{
	if (correctSrc) {
		if (correctDst) {
			verticalTurningSquareLine(line);
		} else if (mEdge->dst()) {
			verticalInnerLine(line);
		}
	} else if (correctDst && mEdge->src()) {
		horizontalInnerLine(line);
	}

}

void SquareLine::createVerticalInnerLine(QPolygonF &line, bool correctSrc, bool correctDst) const
{
	if (correctSrc) {
		if (correctDst) {
			horizontalTurningSquareLine(line);
		} else if (mEdge->dst()) {
			horizontalInnerLine(line);
		}
	} else if (correctDst && mEdge->src()) {
		verticalInnerLine(line);
	}

}

void SquareLine::verticalInnerLine(QPolygonF &line) const
{
	QPair<qreal, qreal> const space = verticalIntermediateSpace();
	if (space.first < space.second) {
		verticalSquareLine(line, (space.first + space.second) / 2);
	} else {
		verticalSameSide(line, mEdge->defineNodePortSide(false) == EdgeElement::top);
	}
}

void SquareLine::horizontalInnerLine(QPolygonF &line) const
{
	QPair<qreal, qreal> const space = horizontalIntermediateSpace();
	if (space.first < space.second) {
		horizontalSquareLine(line, (space.first + space.second) / 2);
	} else {
		horizontalSameSide(line, mEdge->defineNodePortSide(false) == EdgeElement::left);
	}
}

void SquareLine::horizontalSquareLine(QPolygonF &line, qreal middleLine) const
{
	QPointF insertPoint1 = line.first();
	QPointF insertPoint2 = line.last();

	insertPoint1.setX(middleLine);
	insertPoint2.setX(insertPoint1.x());

	line.insert(1, insertPoint1);
	line.insert(2, insertPoint2);
}

void SquareLine::verticalSquareLine(QPolygonF &line, qreal middleLine) const
{
	QPointF insertPoint1 = line.first();
	QPointF insertPoint2 = line.last();

	insertPoint1.setY(middleLine);
	insertPoint2.setY(insertPoint1.y());

	line.insert(1, insertPoint1);
	line.insert(2, insertPoint2);
}

void SquareLine::horizontalTurningSquareLine(QPolygonF &line) const
{
	QPointF insertPoint = line.first();
	insertPoint.setX(line.last().x());
	line.insert(1, insertPoint);
}

void SquareLine::verticalTurningSquareLine(QPolygonF &line) const
{
	QPointF insertPoint = line.first();
	insertPoint.setY(line.last().y());
	line.insert(1, insertPoint);
}

void SquareLine::horizontalSameSide(QPolygonF &line, bool leftLoop) const
{
	const qreal loopLine = leftLoop ? qMin(srcRect().left(), dstRect().left()) - offset
			: qMax(srcRect().right(), dstRect().right()) + offset;
	horizontalSquareLine(line, loopLine);
}

void SquareLine::verticalSameSide(QPolygonF &line, bool topLoop) const
{
	const qreal loopLine = topLoop ? qMin(srcRect().top(), dstRect().top()) - offset
			: qMax(srcRect().bottom(), dstRect().bottom()) + offset;
	verticalSquareLine(line, loopLine);
}

QPair<qreal, qreal> SquareLine::horizontalIntermediateSpace() const
{
	if (srcRect().right() < dstRect().left()) {
		return qMakePair(srcRect().right(), dstRect().left());
	} else {
		return qMakePair(dstRect().right(), srcRect().left());
	}
}

QPair<qreal, qreal> SquareLine::verticalIntermediateSpace() const
{
	if (srcRect().bottom() < dstRect().top()) {
		return qMakePair(srcRect().bottom(), dstRect().top());
	} else {
		return qMakePair(dstRect().bottom(), srcRect().top());
	}
}

QPointF SquareLine::outgoingPoint(bool isStart) const
{
	const QPolygonF line = mEdge->line();
	const int side = mEdge->defineNodePortSide(isStart);

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

QRectF SquareLine::srcRect() const
{
	return mEdge->mapFromItem(mEdge->src(), mEdge->src()->contentsRect()).boundingRect();
}

QRectF SquareLine::dstRect() const
{
	return mEdge->mapFromItem(mEdge->dst(), mEdge->dst()->contentsRect()).boundingRect();
}

EdgeArrangeCriteria SquareLine::arrangeCriteria(const NodeElement *node, const QLineF &portLine) const
{
	const QPointF portCenter = (portLine.p1() + portLine.p2()) / 2;
	const QPointF arrangePoint = portArrangePoint(node);
	QLineF arrangeLine(portCenter, arrangePoint);
	arrangeLine.setAngle(arrangeLine.angle() - portLine.angle());

	const bool turningLeft = arrangeLine.dx() < 0;
	const bool above = arrangeLine.dy() < 0;
	qreal yOffset = arrangeLine.dy();
	if ((turningLeft && above) || (!turningLeft && !above)) {
		yOffset = -yOffset;
	}
	return EdgeArrangeCriteria(turningLeft ? -1 : 1, yOffset, arrangeLine.dx());
}

QPointF SquareLine::portArrangePoint(const NodeElement *node) const
{
	QPolygonF line = mEdge->line();
	NodeElement *src = mEdge->src();
	NodeElement *dst = mEdge->dst();

	if (node == src) {
		return (line.count() == 2) ? mEdge->mapToItem(src, line[1]) : mEdge->mapToItem(src, line[2]);
	}
	if (node == dst) {
		return (line.count() == 2) ? mEdge->mapToItem(src, line[0]) : mEdge->mapToItem(dst, line[line.count() - 3]);
	}
	return QPointF();
}

QList<ContextMenuAction *> SquareLine::extraActions(const QPointF &pos)
{
	Q_UNUSED(pos)

	QList<ContextMenuAction *> result;
	if (!mEdge->isLoop()) {
		result << &mLayOutAction;
	}

	return result;
}

void SquareLine::drawPort(QPainter *painter, int portNumber)
{
	if ((portNumber == 0) || (portNumber == mEdge->line().count() - 1)) {
		LineHandler::drawPort(painter, portNumber);
	}
}
