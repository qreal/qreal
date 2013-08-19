#include "lineHandler.h"

#include "../nodeElement.h"
#include "../../controller/controller.h"

namespace qReal {

LineHandler::LineHandler(EdgeElement *edge)
	: mEdge(edge), mReshapeCommand(NULL), mReshapeStarted(false)
{
}

void LineHandler::startMovingEdge(int dragType, QPointF const &pos)
{
	mReshapeCommand = new commands::ReshapeEdgeCommand(static_cast<EditorViewScene *>(mEdge->scene()), mEdge->id());
	mReshapeCommand->startTracking();
	mReshapeStarted = true;

	mSavedLine = mEdge->line();
	mDragType = dragType;
	mDragStartPoint = pos;
}

void LineHandler::rejectMovingEdge()
{
	delete mReshapeCommand;
	mReshapeCommand = NULL;
	mReshapeStarted = false;
	mDragType = EdgeElement::noPort;
	mEdge->setLine(mSavedLine);
}

void LineHandler::moveEdge(QPointF const &, bool)
{
}

void LineHandler::endMovingEdge()
{
	if ((mDragType == 0) || (mDragType == mSavedLine.count() - 1)) {
		bool isStart = mDragType == 0;
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
		layOut();
	}

	endReshape();
	mDragType = EdgeElement::noPort;
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
		mReshapeCommand = NULL;
	}

	mReshapeStarted = false;
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
	}
}

void LineHandler::layOut(bool needReconnect)
{
	if (needReconnect) {
		reconnect(true, true);
	}

	handleIntersections();

	if (mEdge->src()) {
		mEdge->src()->arrangeLinearPorts();
		mEdge->src()->adjustLinks();
	}
	if (mEdge->dst()) {
		mEdge->dst()->arrangeLinearPorts();
		mEdge->dst()->adjustLinks();
	}

	improveAppearance();
}

void LineHandler::reconnect(bool reconnectSrc, bool reconnectDst)
{
	NodeElement *src = mEdge->src();
	NodeElement *dst = mEdge->dst();

	if (src && reconnectSrc) {
		int targetLinePoint = firstOutsidePoint(true);
		qreal newFrom = src->portId(mEdge->mapToItem(src, mEdge->line()[targetLinePoint]), mEdge->fromPortTypes());
		mEdge->setFromPort(newFrom);
	}
	if (dst && reconnectDst) {
		int targetLinePoint = firstOutsidePoint(false);
		qreal newTo = dst->portId(mEdge->mapToItem(dst, mEdge->line()[targetLinePoint]), mEdge->toPortTypes());
		mEdge->setToPort(newTo);
	}
}

int LineHandler::firstOutsidePoint(bool startFromSrc) const
{
	NodeElement *node = startFromSrc ? mEdge->src() : mEdge->dst();
	if (!node) {
		return 0;
	}
	int point = startFromSrc ? 0 : mEdge->line().count() - 1;

	while (point >= 0 && point < mEdge->line().count()
			&& node->boundingRect().contains(mEdge->mapToItem(node, mEdge->line().at(point)))) {
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

void LineHandler::improveAppearance()
{
}

void LineHandler::handleIntersections()
{
}

void LineHandler::alignToGrid()
{
	QPolygonF line = mEdge->line();

	for (int i = 1; i < line.size() - 1; ++i) {
		line[i] = alignedPoint(line[i]);
	}

	mEdge->setLine(line);
	mEdge->update();
}

QPointF LineHandler::alignedPoint(QPointF const &point) const
{
	return point;
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

void LineHandler::drawLine(QPainter *painter, bool drawSavedLine)
{
	QPolygonF line = drawSavedLine ? mSavedLine : mEdge->line();
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
	QPointF p1(-0.25,0);
	QPointF p2(0.25,0);

	QColor const portColor("#465945");
	QColor const highlightColor("#c3dcc4");

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

}
