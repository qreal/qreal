#include "umllib/private/lineHandler.h"

#include "umllib/nodeElement.h"
#include "controller/controller.h"

using namespace qReal;

LineHandler::LineHandler(EdgeElement *edge)
		: mEdge(edge)
		, mSavedLine(mEdge->line())
		, mDragType(EdgeElement::noPort)
		, mNodeWithHighlightedPorts(NULL)
		, mReshapeCommand(NULL)
		, mReshapeStarted(false)
{
}

int LineHandler::startMovingEdge(QPointF const &pos)
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
	mReshapeCommand = NULL;
	mReshapeStarted = false;
	mDragType = EdgeElement::noPort;
	dehighlightPorts();
	mEdge->setLine(mSavedLine);
}

void LineHandler::moveEdge(QPointF const &pos)
{
	if (!mEdge->isLoop() || (mDragType == 0) || (mDragType == mEdge->line().count() - 1)) {
		handleEdgeMove(pos);
	}

	if ((mDragType == 0) || (mDragType == mEdge->line().count() - 1)) {
		highlightPorts(mDragType == 0);
	}
}

void LineHandler::handleEdgeMove(QPointF const &)
{
}

void LineHandler::endMovingEdge()
{
	if ((mDragType == 0) || (mDragType == mEdge->line().count() - 1)) {
		bool const isStart = (mDragType == 0);
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
		mReshapeCommand = NULL;
	}

	mReshapeStarted = false;
}

void LineHandler::highlightPorts(bool isStart)
{
	dehighlightPorts();

	QPolygonF const line = mEdge->line();
	mNodeWithHighlightedPorts = mEdge->getNodeAt(isStart ? line[0] : line[line.count() - 1], isStart);
	if (mNodeWithHighlightedPorts) {
		mNodeWithHighlightedPorts->setPortsVisible(isStart ? mEdge->fromPortTypes() : mEdge->toPortTypes());
	}
}

void LineHandler::dehighlightPorts()
{
	if (mNodeWithHighlightedPorts) {
		mNodeWithHighlightedPorts->setPortsVisible(QStringList());
		mNodeWithHighlightedPorts = NULL;
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
	NodeElement const * const src = mEdge->src();
	NodeElement const * const dst = mEdge->dst();

	if (src && reconnectSrc) {
		int const targetLinePoint = firstOutsidePoint(true);
		qreal const newFrom = src->portId(mEdge->mapToItem(src, mEdge->line()[targetLinePoint]), mEdge->fromPortTypes());
		mEdge->setFromPort(newFrom);
	}

	if (dst && reconnectDst) {
		int const targetLinePoint = firstOutsidePoint(false);
		qreal const newTo = dst->portId(mEdge->mapToItem(dst, mEdge->line()[targetLinePoint]), mEdge->toPortTypes());
		mEdge->setToPort(newTo);
	}
}

int LineHandler::firstOutsidePoint(bool startFromSrc) const
{
	NodeElement const * const node = startFromSrc ? mEdge->src() : mEdge->dst();
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

EdgeArrangeCriteria LineHandler::arrangeCriteria(NodeElement const *node, QLineF const &portLine) const
{
	QPointF const portCenter = (portLine.p1() + portLine.p2()) / 2;
	QPointF const arrangePoint = portArrangePoint(node);
	QLineF arrangeLine(portCenter, arrangePoint);
	arrangeLine.setAngle(arrangeLine.angle() - portLine.angle());

	bool const turningLeft = arrangeLine.dx() < 0;
	return EdgeArrangeCriteria(turningLeft ? -1 : 1, 0.0, arrangeLine.dx());
}


QPointF LineHandler::portArrangePoint(NodeElement const *node) const
{
	if (!mEdge->isLoop()) {
		return (node == mEdge->src()) ? mEdge->mapToItem(mEdge->src(), mEdge->line()[1])
				: mEdge->mapToItem(mEdge->dst(), mEdge->line()[mEdge->line().count() - 2]);
	} else {
		mEdge->createLoopEdge();
		return mEdge->mapToItem(mEdge->src(), mEdge->line()[3]);
	}
}

int LineHandler::definePoint(QPointF const &pos) const
{
	QPolygonF const line = mEdge->line();
	for (int i = 0; i < line.size(); ++i)
		if (QRectF(line[i] - QPointF(kvadratik / 2, kvadratik / 2), QSizeF(kvadratik, kvadratik)).contains(pos))
			return i;

	return -1;
}

int LineHandler::defineSegment(QPointF const &pos) const
{
	QPainterPath path;
	QPainterPathStroker ps;
	ps.setWidth(kvadratik);
	for (int i = 0; i < mSavedLine.size() - 1; ++i) {
		path.moveTo(mSavedLine[i]);
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

bool LineHandler::checkPort(QPointF const &pos, bool isStart) const
{
	NodeElement const * const node = mEdge->getNodeAt(pos, isStart);
	if (!node) {
		return true;
	}

	qreal const port = node->portId(mEdge->mapToItem(node, pos), isStart ? mEdge->fromPortTypes() : mEdge->toPortTypes());
	if (port < 0) {
		return true;
	}

	QPointF const point = mEdge->mapFromItem(node, node->portPos(port));
	QRectF rect(point - QPointF(kvadratik, kvadratik), point + QPointF(kvadratik, kvadratik));
	return rect.contains(pos);
}

bool LineHandler::nodeChanged(bool isStart) const
{
	NodeElement const * const node = mEdge->getNodeAt(isStart ? mEdge->line().first() : mEdge->line().last(), isStart);
	return isStart ? (node != mEdge->src()) : (node != mEdge->dst());
}

void LineHandler::drawLine(QPainter *painter, bool drawSavedLine)
{
	QPolygonF const line = drawSavedLine ? mSavedLine : mEdge->line();
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
	QPointF const p1(-0.25, 0);
	QPointF const p2(0.25, 0);

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

QList<ContextMenuAction *> LineHandler::extraActions(QPointF const &pos)
{
	Q_UNUSED(pos)
	return QList<ContextMenuAction *>();
}

void LineHandler::connectAction(ContextMenuAction *action, QObject *receiver, char const *slot) const
{
	connect(action, SIGNAL(triggered(QPointF const &)), this, SLOT(startReshape()));
	connect(action, SIGNAL(triggered(QPointF const &)), receiver, slot);
	connect(action, SIGNAL(triggered(QPointF const &)), this, SLOT(endReshape()));
}

void LineHandler::minimize()
{
	QPolygonF line;
	line << mEdge->line().first() << mEdge->line().last();
	mEdge->setLine(line);
	layOut();
}
