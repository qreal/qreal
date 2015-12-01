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

#include "editor/private/lineHandler.h"

#include "editor/nodeElement.h"
#include "controller/controller.h"

using namespace qReal;
using namespace qReal::gui::editor;

LineHandler::LineHandler(EdgeElement *edge)
		: mEdge(edge)
		, mSavedLine(mEdge->line())
		, mDragType(EdgeElement::noPort)
		, mNodeWithHighlightedPorts(nullptr)
		, mReshapeCommand(nullptr)
		, mReshapeStarted(false)
{
}

int LineHandler::startMovingEdge(const QPointF &pos)
{
	startReshape();

	mSavedLine = mEdge->line();
	mDragType = definePoint(pos);
	mDragStartPoint = pos;

	return mDragType;
}

void LineHandler::rejectMovingEdge()
{
	delete mReshapeCommand;
	mReshapeCommand = nullptr;
	mReshapeStarted = false;
	mDragType = EdgeElement::noPort;
	dehighlightPorts();
	mEdge->setLine(mSavedLine);
}

void LineHandler::moveEdge(const QPointF &pos)
{
	if (!mEdge->isLoop() || (mDragType == 0) || (mDragType == mEdge->line().count() - 1)) {
		handleEdgeMove(pos);
	}

	if ((mDragType == 0) || (mDragType == mEdge->line().count() - 1)) {
		highlightPorts(mDragType == 0);
	}
}

void LineHandler::handleEdgeMove(const QPointF &)
{
}

void LineHandler::endMovingEdge()
{
	if ((mDragType == 0) || (mDragType == mEdge->line().count() - 1)) {
		const bool isStart = (mDragType == 0);
		if (nodeChanged(isStart)) {
			mEdge->connectToPort();
			layOut();
		} else if (!checkPort(isStart ? mEdge->line().first() : mEdge->line().last(), isStart)) {
			rejectMovingEdge();
			return;
		} else {
			mEdge->connectToPort();
			layOut(false);
		}
	} else {
		mEdge->connectToPort();
		layOut();
	}

	endReshape();
	mDragType = EdgeElement::noPort;
	dehighlightPorts();
}

void LineHandler::startReshape()
{
	mReshapeCommand = new commands::ReshapeEdgeCommand(static_cast<EditorViewScene *>(mEdge->scene()), mEdge->id());
	mReshapeCommand->startTracking();
	mReshapeStarted = true;
}

void LineHandler::endReshape()
{
	if (mReshapeCommand) {
		mReshapeCommand->stopTracking();
		if (mReshapeCommand->somethingChanged()) {
			mEdge->controller()->execute(mReshapeCommand);
		} else {
			delete mReshapeCommand;
		}
		mReshapeCommand = nullptr;
	}

	mReshapeStarted = false;
}

void LineHandler::highlightPorts(bool isStart)
{
	dehighlightPorts();

	const QPolygonF line = mEdge->line();
	mNodeWithHighlightedPorts = mEdge->getNodeAt(isStart ? line[0] : line[line.count() - 1], isStart);
	if (mNodeWithHighlightedPorts) {
		mNodeWithHighlightedPorts->setPortsVisible(isStart ? mEdge->fromPortTypes() : mEdge->toPortTypes());
	}
}

void LineHandler::dehighlightPorts()
{
	if (mNodeWithHighlightedPorts) {
		mNodeWithHighlightedPorts->setPortsVisible(QStringList());
		mNodeWithHighlightedPorts = nullptr;
	}
}

void LineHandler::adjust()
{
	QPolygonF line = mEdge->line();
	NodeElement *src = mEdge->src();
	NodeElement *dst = mEdge->dst();

	if (src && src->isSelected() && dst && dst->isSelected() && !mEdge->isLoop()) {
		QPointF offset = mEdge->mapFromItem(src, src->portPos(mEdge->fromPort())) - line.first();
		mEdge->setPos(mEdge->pos() + offset);
		return;
	}

	if (src) {
		line.first() = mEdge->mapFromItem(src, src->portPos(mEdge->fromPort()));
	}

	if (dst) {
		line.last() = mEdge->mapFromItem(dst, dst->portPos(mEdge->toPort()));
	}

	mEdge->setLine(line);

	if (mEdge->isLoop()) {
		mEdge->createLoopEdge();
	}
}

void LineHandler::layOut(bool needReconnect)
{
	connectAndArrange(needReconnect, needReconnect);
	if (!mEdge->isLoop()) {
		improveAppearance();
	}
}

void LineHandler::connectAndArrange(bool reconnectSrc, bool reconnectDst)
{
	reconnect(reconnectSrc, reconnectDst);
	mEdge->arrangeLinearPorts();
}

void LineHandler::reconnect(bool reconnectSrc, bool reconnectDst)
{
	const NodeElement * const src = mEdge->src();
	const NodeElement * const dst = mEdge->dst();

	if (src && reconnectSrc) {
		const int targetLinePoint = firstOutsidePoint(true);
		const qreal newFrom = src->portId(mEdge->mapToItem(src, mEdge->line()[targetLinePoint])
				, mEdge->fromPortTypes());
		mEdge->setFromPort(newFrom);
	}

	if (dst && reconnectDst) {
		const int targetLinePoint = firstOutsidePoint(false);
		const qreal newTo = dst->portId(mEdge->mapToItem(dst, mEdge->line()[targetLinePoint]), mEdge->toPortTypes());
		mEdge->setToPort(newTo);
	}
}

int LineHandler::firstOutsidePoint(bool startFromSrc) const
{
	const NodeElement * const node = startFromSrc ? mEdge->src() : mEdge->dst();
	if (!node) {
		return 0;
	}

	int point = startFromSrc ? 0 : mEdge->line().count() - 1;
	while (point >= 0 && point < mEdge->line().count()
			&& node->contentsRect().contains(mEdge->mapToItem(node, mEdge->line().at(point)))) {
		startFromSrc ? point++ : point--;
	}

	if (point < 0) {
		return 0;
	}

	if (point >= mEdge->line().count()) {
		return mEdge->line().count() - 1;
	}

	return point;
}

EdgeArrangeCriteria LineHandler::arrangeCriteria(const NodeElement *node, const QLineF &portLine) const
{
	const QPointF portCenter = (portLine.p1() + portLine.p2()) / 2;
	const QPointF arrangePoint = portArrangePoint(node);
	QLineF arrangeLine(portCenter, arrangePoint);
	arrangeLine.setAngle(arrangeLine.angle() - portLine.angle());

	const bool turningLeft = arrangeLine.dx() < 0;
	return EdgeArrangeCriteria(turningLeft ? -1 : 1, 0.0, arrangeLine.dx());
}


QPointF LineHandler::portArrangePoint(const NodeElement *node) const
{
	if (!mEdge->isLoop()) {
		return (node == mEdge->src()) ? mEdge->mapToItem(mEdge->src(), mEdge->line()[1])
				: mEdge->mapToItem(mEdge->dst(), mEdge->line()[mEdge->line().count() - 2]);
	} else {
		mEdge->createLoopEdge();
		return mEdge->mapToItem(mEdge->src(), mEdge->line()[3]);
	}
}

int LineHandler::definePoint(const QPointF &pos) const
{
	const QPolygonF line = mEdge->line();
	for (int i = 0; i < line.size(); ++i)
		if (QRectF(line[i] - QPointF(EdgeElement::stripeWidth / 2, EdgeElement::stripeWidth / 2)
				, QSizeF(EdgeElement::stripeWidth, EdgeElement::stripeWidth)).contains(pos))
			return i;

	return -1;
}

int LineHandler::defineSegment(const QPointF &pos) const
{
	QPainterPathStroker ps;
	ps.setWidth(EdgeElement::stripeWidth);
	for (int i = 0; i < mSavedLine.size() - 1; ++i) {
		QPainterPath path(mSavedLine[i]);
		path.lineTo(mSavedLine[i + 1]);
		if (ps.createStroke(path).contains(pos)) {
			return i;
		}
	}

	return -1;
}

void LineHandler::improveAppearance()
{
}

void LineHandler::deleteLoops()
{
	if (mEdge->isLoop()) {
		return;
	}

	QPolygonF line = mEdge->line();
	deleteLoop(line, 0);
	mEdge->setLine(line);
}

void LineHandler::deleteLoop(QPolygonF &line, int startPos)
{
	for (int i = startPos; i < line.size() - 3; ++i) {
		bool isCut = false;
		for (int j = i + 2; j < line.size() - 1; ++j) {
			QPointF cut;
			if (QLineF(line[i], line[i + 1]).intersect(QLineF(line[j], line[j + 1]), &cut)
					== QLineF::BoundedIntersection)
			{
				if ((i != 0) || !((j == line.size() - 2)
						&& (QLineF(line.first(), line.last()).length() < (kvadratik * 2))))
				{
					QPainterPath path;
					QPainterPathStroker ps;
					ps.setWidth(kvadratik);
					for (int k = 0; k < line.size() - 1; ++k) {
						path.moveTo(line[k]);
						path.lineTo(line[k + 1]);
						if (ps.createStroke(path).contains(cut)) {
							line.insert(k + 1, cut);
							break;
						}
					}

					line.remove(i + 2, j - i);
					deleteLoop(line, i);
					isCut = true;
					break;
				}
			}
		}

		if (isCut) {
			break;
		}
	}
}

void LineHandler::alignToGrid()
{
}

bool LineHandler::checkPort(const QPointF &pos, bool isStart) const
{
	const NodeElement * const node = mEdge->getNodeAt(pos, isStart);
	if (!node) {
		return true;
	}

	const qreal port = node->portId(mEdge->mapToItem(node, pos), isStart
			? mEdge->fromPortTypes()
			: mEdge->toPortTypes());

	if (port < 0) {
		return true;
	}

	const QPointF point = mEdge->mapFromItem(node, node->portPos(port));
	QRectF rect(point - QPointF(kvadratik, kvadratik), point + QPointF(kvadratik, kvadratik));
	return rect.contains(pos);
}

bool LineHandler::nodeChanged(bool isStart) const
{
	const NodeElement * const node = mEdge->getNodeAt(isStart ? mEdge->line().first() : mEdge->line().last(), isStart);
	return isStart ? (node != mEdge->src()) : (node != mEdge->dst());
}

void LineHandler::drawLine(QPainter *painter, bool drawSavedLine)
{
	const QPolygonF line = drawSavedLine ? mSavedLine : mEdge->line();
	painter->drawPolyline(line);
}

void LineHandler::drawPorts(QPainter *painter)
{
	for (int i = 0; i < mEdge->line().count(); i++) {
		painter->save();
		painter->translate(mEdge->line().at(i));
		drawPort(painter, i);
		painter->restore();
	}
}

void LineHandler::drawPort(QPainter *painter, int portNumber)
{
	Q_UNUSED(portNumber)

	QPen pen;
	const QPointF p1(-0.25, 0);
	const QPointF p2(0.25, 0);

	const QColor portColor("#465945");
	const QColor highlightColor("#c3dcc4");

	pen.setWidth(12);
	pen.setColor(highlightColor);
	painter->setPen(pen);
	painter->drawLine(p1, p2);

	pen.setWidth(3);
	pen.setColor(portColor);
	painter->setPen(pen);
	painter->drawLine(p1, p2);
}

QPainterPath LineHandler::shape() const
{
	QPainterPath path;
	path.addPolygon(mEdge->line());
	return path;
}

QPolygonF LineHandler::savedLine() const
{
	return mSavedLine;
}

bool LineHandler::isReshapeStarted() const
{
	return mReshapeStarted;
}

QList<ContextMenuAction *> LineHandler::extraActions(const QPointF &pos)
{
	Q_UNUSED(pos)
	return QList<ContextMenuAction *>();
}

void LineHandler::connectAction(ContextMenuAction *action, QObject *receiver, char const *slot) const
{
	connect(action, SIGNAL(triggered(const QPointF &)), this, SLOT(startReshape()));
	connect(action, SIGNAL(triggered(const QPointF &)), receiver, slot);
	connect(action, SIGNAL(triggered(const QPointF &)), this, SLOT(endReshape()));
}

void LineHandler::minimize()
{
	QPolygonF line;
	line << mEdge->line().first() << mEdge->line().last();
	mEdge->setLine(line);
	layOut();
}
