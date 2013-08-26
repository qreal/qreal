/** @file edgeElement.cpp
*	@brief class for an edge on a diagram
**/

#include <QtWidgets/QStyleOptionGraphicsItem>
#include <QtWidgets/QStyle>
#include <QtGui/QTextDocument>
#include <QtWidgets/QMenu>
#include <QDebug>
#include <math.h>

#include "edgeElement.h"
#include "nodeElement.h"
#include "labelFactory.h"
#include "../view/editorViewScene.h"

#include "private/brokenLine.h"
#include "private/squareLine.h"
#include "private/curveLine.h"

using namespace qReal;

const double pi = 3.14159265358979;

const qreal epsilon = 0.00000000001;

const int rightRotation = 1;// the difference between the elements of NodeSide
const int maxReductCoeff = 16;
const int standartReductCoeff = 3;

/** @brief indicator of edges' movement */

EdgeElement::EdgeElement(
		ElementImpl *impl
		, Id const &id
		, qReal::models::GraphicalModelAssistApi &graphicalAssistApi
		, qReal::models::LogicalModelAssistApi &logicalAssistApi
		)
		: Element(impl, id, graphicalAssistApi, logicalAssistApi)
		, mPenStyle(Qt::SolidLine)
		, mPenWidth(1)
		, mPenColor(Qt::black)
		, mSrc(NULL)
		, mDst(NULL)
		, mHandler(NULL)
		, mPortFrom(0)
		, mPortTo(0)
		, mDragType(noPort)
		, mLongPart(0)
		, mDelPointAction(tr("Delete point"), this)
		, mMinimizeAction(tr("Remove all points"), this)
		, mDelSegmentAction(tr("Remove segment"), this)
		, mReverseAction(tr("Reverse"), this)
		, mModelUpdateIsCalled(false)
		, mIsLoop(false)
{
	mPenStyle = mElementImpl->getPenStyle();
	mPenWidth = mElementImpl->getPenWidth();
	mPenColor = mElementImpl->getPenColor();
	setZValue(100);
	setFlag(ItemIsMovable, true);
	// if flag is true then draws strangely...
	setFlag(ItemClipsToShape, false);
	setFlag(ItemClipsChildrenToShape, false);

	mLine << QPointF(0, 0) << QPointF(200, 60);

	setAcceptHoverEvents(true);

	connect(&mDelPointAction, SIGNAL(triggered(QPointF const &)), SLOT(delPointHandler(QPointF const &)));
	connect(&mReverseAction, SIGNAL(triggered(QPointF const)), SLOT(reverseHandler(QPointF const &)));
	connect(&mMinimizeAction, SIGNAL(triggered(QPointF const &)), SLOT(minimizeHandler(QPointF const &)));
	connect(&mDelSegmentAction, SIGNAL(triggered(QPointF const &)), SLOT(deleteSegmentHandler(QPointF const &)));

	LabelFactory factory(graphicalAssistApi, mId);
	QList<LabelInterface*> titles;

	mElementImpl->init(factory, titles);
	foreach (LabelInterface *titleIface, titles) {
		Label *title = dynamic_cast<Label*>(titleIface);
		if (!title) {
			continue;
		}
		title->init(boundingRect());
		title->setParentItem(this);
		title->setShouldCenter(false);
		mLabels.append(title);
	}

	initLineHandler();
}

EdgeElement::~EdgeElement()
{
	if (mSrc)
		mSrc->delEdge(this);
	if (mDst)
		mDst->delEdge(this);

	delete mElementImpl;
	delete mHandler;
}

void EdgeElement::initTitles()
{
	Element::initTitles();
	updateLongestPart();
}

void EdgeElement::initLineHandler()
{
	delete mHandler;
	switch(SettingsManager::value("LineType").toInt()) {
	case brokenLine:
		mHandler = new BrokenLine(this);
		break;
	case curveLine:
		mHandler = new CurveLine(this);
		break;
	default:
		mHandler = new SquareLine(this);
	}
}

QRectF EdgeElement::boundingRect() const
{
	return mLine.boundingRect().adjusted(-20, -20, 20, 20);
}

QPolygonF EdgeElement::line() const
{
	return mLine;
}

void EdgeElement::setLine(QPolygonF const &line)
{
	prepareGeometryChange();
	mLine = line;
	saveConfiguration();
	updateLongestPart();
}

qreal EdgeElement::fromPort() const
{
	return mPortFrom;
}

qreal EdgeElement::toPort() const
{
	return mPortTo;
}

void EdgeElement::setFromPort(qreal const &fromPort)
{
	mPortFrom = fromPort;
	mModelUpdateIsCalled = true;
	mGraphicalAssistApi.setFromPort(id(), mPortFrom);
}

void EdgeElement::setToPort(qreal const &toPort)
{
	mPortTo = toPort;
	mModelUpdateIsCalled = true;
	mGraphicalAssistApi.setToPort(id(), mPortTo);
}

void EdgeElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget*)
{
	if (SettingsManager::value("PaintOldEdgeMode").toBool() && mHandler->isReshapeStarted()) {
		paintEdge(painter, option, true);
	}
	paintEdge(painter, option, false);
}

void EdgeElement::paintEdge(QPainter *painter, QStyleOptionGraphicsItem const *option, bool drawSavedLine) const
{
	painter->save();
	if (drawSavedLine) {
		QColor color = QColor(SettingsManager::value("oldLineColor").toString());
		setEdgePainter(painter, edgePen(painter, color, Qt::DashDotLine, mPenWidth), 0.5);
	} else {
		setEdgePainter(painter, edgePen(painter, mColor, mPenStyle, mPenWidth), painter->opacity());
	}
	mHandler->drawLine(painter, drawSavedLine);
	painter->restore();

	drawArrows(painter, drawSavedLine);

	if ((option->state & (QStyle::State_Selected | QStyle::State_MouseOver)) && !drawSavedLine) {
		painter->setBrush(Qt::SolidPattern);
		mHandler->drawPorts(painter);
	}
}

void EdgeElement::drawArrows(QPainter *painter, bool savedLine) const
{
	Qt::PenStyle style(QPen(painter->pen()).style());

	painter->save();
	if (savedLine) {
		QColor color = QColor(SettingsManager::value("oldLineColor").toString());
		setEdgePainter(painter, edgePen(painter, color, Qt::SolidLine, 3), 0.5);
	} else {
		setEdgePainter(painter, edgePen(painter, mColor, style, 3), painter->opacity());
	}
	QPolygonF line = savedLine ? mHandler->savedLine() : mLine;

	painter->save();
	painter->translate(line[0]);
	painter->rotate(90 - QLineF(line[1], line[0]).angle());
	drawStartArrow(painter);
	painter->restore();

	painter->save();
	painter->translate(line[line.size() - 1]);
	painter->rotate(90 - QLineF(line[line.size() - 2], line[line.size() - 1]).angle());
	drawEndArrow(painter);
	painter->restore();

	painter->restore();
}

QPen EdgeElement::edgePen(QPainter *painter, QColor color, Qt::PenStyle style, int width) const
{
	QPen pen = painter->pen();
	pen.setColor(color);
	pen.setBrush(color);
	pen.setStyle(style);
	pen.setWidth(width);
	return pen;
}

void EdgeElement::setEdgePainter(QPainter *painter, QPen pen, qreal opacity) const
{
	painter->setPen(pen);
	painter->setOpacity(opacity);
}

QPainterPath EdgeElement::shape() const
{
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);

	path.addPath(mHandler->shape());

	QPainterPathStroker ps;
	ps.setWidth(kvadratik - 2.5);

	path = ps.createStroke(path);

	foreach (QPointF const &point, mLine) {
		path.addRect(getPortRect(point).adjusted(1, 1, -1, -1));
	}

	return path;
}

QRectF EdgeElement::getPortRect(QPointF const &point)
{
	return QRectF(point - QPointF(kvadratik / 2, kvadratik / 2), QSizeF(kvadratik, kvadratik));
}

int EdgeElement::getPoint(const QPointF &location)
{
	for (int i = 0; i < mLine.size(); ++i)
		if (getPortRect(mLine[i]).contains(location))
			return i;

	return noPort;
}

void EdgeElement::updateLongestPart()
{
	qreal maxLen = 0.0;
	int maxIdx = 0;
	for (int i = 0; i < mLine.size() - 1; ++i) {
		qreal newLen = QLineF(mLine[i], mLine[i + 1]).length();
		if (newLen > maxLen) {
			maxLen = newLen;
			maxIdx = i;
		}
	}
	mLongPart = maxIdx;

	if (mLabels.count() == 1) {
		Label *title = mLabels[0];
		qreal x = (mLine[maxIdx].x() + mLine[maxIdx + 1].x()) / 2;
		qreal y = (mLine[maxIdx].y() + mLine[maxIdx + 1].y()) / 2;
		x -= title->boundingRect().width() / 2;
		y -= title->boundingRect().height() / 2;
		title->setPos(x, y);

		QLineF longest(mLine[maxIdx], mLine[mLongPart + 1]);

	}
}

void EdgeElement::connectToPort()
{
	mMoving = true;

	NodeElement *newSrc = getNodeAt(mLine.first(), true);
	NodeElement *newDst = getNodeAt(mLine.last(), false);

	mIsLoop = ((newSrc == newDst) && newSrc);

	setPos(pos() + mLine.first());
	mLine.translate(-mLine.first());

	if (mIsLoop) {
		connectLoopEdge(newSrc);
		createLoopEdge();
		return;
	}

	mPortFrom = newSrc ? newSrc->portId(mapToItem(newSrc, mLine.first()), fromPortTypes()) : -1.0;
	mPortTo = newDst ? newDst->portId(mapToItem(newDst, mLine.last()), toPortTypes()) : -1.0;

	if (mSrc) {
		mSrc->delEdge(this);
		mSrc = NULL;
	}

	if (mDst) {
		mDst->delEdge(this);
		mDst = NULL;
	}

	if (mPortFrom >= -epsilon) {
		mSrc = newSrc;
		mSrc->addEdge(this);
	}

	mGraphicalAssistApi.setFrom(id(), (mSrc ? mSrc->id() : Id::rootId()));
	mGraphicalAssistApi.setFromPort(id(), mPortFrom);

	if (mPortTo >= -epsilon) {
		mDst = newDst;
		mDst->addEdge(this);
	}

	mGraphicalAssistApi.setTo(id(), (mDst ? mDst->id() : Id::rootId()));
	mGraphicalAssistApi.setToPort(id(), mPortTo);

	mLogicalAssistApi.setFrom(logicalId(), (mSrc ? mSrc->logicalId() : Id::rootId()));
	mLogicalAssistApi.setTo(logicalId(), (mDst ? mDst->logicalId() : Id::rootId()));

	adjustLink();

	mGraphicalAssistApi.setPosition(id(), pos());
	saveConfiguration();

	mMoving = false;

	if (newSrc && newDst) {
		highlight(mPenColor);
	} else {
		highlight(Qt::red);
	}
}

void EdgeElement::layOut()
{
	mHandler->layOut();
}

void EdgeElement::connectLoopEdge(NodeElement *newMaster)
{
	mPortFrom = newMaster ? newMaster->portId(mapToItem(newMaster, mLine.first()), fromPortTypes()) : -1.0;
	mPortTo = newMaster ? newMaster->portId(mapToItem(newMaster, mLine.last()), toPortTypes()) : -1.0;

	if (mPortFrom >= -epsilon) {
		newMaster->delEdge(this);
		mSrc = newMaster;
		mDst = newMaster;
		mSrc->addEdge(this);
	}

	mGraphicalAssistApi.setFrom(id(), (mSrc ? mSrc->id() : Id::rootId()));
	mGraphicalAssistApi.setFromPort(id(), mPortFrom);

	mGraphicalAssistApi.setTo(id(), (mDst ? mDst->id() : Id::rootId()));
	mGraphicalAssistApi.setToPort(id(), mPortTo);

	mLogicalAssistApi.setFrom(logicalId(), (mSrc ? mSrc->logicalId() : Id::rootId()));
	mLogicalAssistApi.setTo(logicalId(), (mDst ? mDst->logicalId() : Id::rootId()));

	mGraphicalAssistApi.setPosition(id(), pos());
	saveConfiguration();

	mMoving = false;

	highlight(mPenColor);
}

void EdgeElement::createLoopEdge() // nice implementation makes sense after #602 fixed!
{
	if (!(mDst && mSrc)) {
		return;
	}

	if ((int) mPortFrom == (int) mPortTo) {
		searchNextPort();
	}

	setPos(pos() + mLine.first());
	mLine.translate(-mLine.first());

	QPolygonF newLine;

	NodeSide startSide = defineNodePortSide(true);
	NodeSide endSide = defineNodePortSide(false);

	QPointF secondPoint = boundingRectIndent(mLine.first(), startSide);
	QPointF penultPoint = boundingRectIndent(mLine.last(), endSide);

	if (isNeighbor(startSide, endSide)) {
		QPointF thirdPoint;
		if ((endSide == top) || (endSide == bottom)) {
			thirdPoint = QPointF(secondPoint.x(), penultPoint.y());
		} else {
			thirdPoint = QPointF(penultPoint.x(), secondPoint.y());
		}
		newLine << mLine.first() << secondPoint
				<< thirdPoint << thirdPoint
				<< penultPoint << mLine.last(); // Third point added twice for easy change of line mode (to curve lines)
	} else {
		QPointF thirdPoint = boundingRectIndent(secondPoint, rotateRight(startSide));
		QPointF forthPoint = boundingRectIndent(thirdPoint, rotateRight(rotateRight(startSide)));

		newLine << mLine.first() << secondPoint
				<< thirdPoint << forthPoint
				<< penultPoint << mLine.last();
	}

	setLine(newLine);
	mIsLoop = true;
}

QPointF EdgeElement::boundingRectIndent(QPointF const &point, EdgeElement::NodeSide direction)
{
	QPointF newPoint;
	QRectF bounds = mSrc->boundingRect();
	int reductFactor = indentReductCoeff();

	switch (direction) {
	case top: {
		QPointF topPoint = mapToItem(mSrc, QPointF(point.x(), 0));
		newPoint = mapFromItem(mSrc, QPointF(topPoint.x(), bounds.top() - bounds.height() / reductFactor));
		break;
	}
	case bottom: {
		QPointF bottomPoint = mapToItem(mSrc, QPointF(point.x(), 0));
		newPoint = mapFromItem(mSrc, QPointF(bottomPoint.x(), bounds.bottom() + bounds.height() / reductFactor));
		break;
	}
	case left: {
		QPointF leftPoint = mapToItem(mSrc, QPointF(0, point.y()));
		newPoint = mapFromItem(mSrc, QPointF(bounds.left() - bounds.width() / reductFactor, leftPoint.y()));
		break;
	}
	case right: {
		QPointF rightPoint = mapToItem(mSrc, QPointF(0, point.y()));
		newPoint = mapFromItem(mSrc, QPointF(bounds.right() + bounds.width() / reductFactor, rightPoint.y()));
		break;
	}
	default:
		qDebug() << "incorrect direction";
	}

	return newPoint;
}

int EdgeElement::indentReductCoeff() {
	return (maxReductCoeff - SettingsManager::value("LoopEdgeBoundsIndent").toInt());
}

void EdgeElement::searchNextPort() {
	mPortTo += 1;
	if (mPortTo > mSrc->numberOfPorts()) {
		mPortTo -= mSrc->numberOfPorts();
	}
}

bool EdgeElement::isNeighbor(const EdgeElement::NodeSide &startSide, const EdgeElement::NodeSide &endSide) const
{
	return ((startSide == rotateRight(endSide)) || (endSide == rotateRight(startSide)));
}

EdgeElement::NodeSide EdgeElement::rotateRight(EdgeElement::NodeSide side) const
{
	return (NodeSide)(((int)side + rightRotation) % 4);
}

bool EdgeElement::initPossibleEdges()
{
	if (!mPossibleEdges.isEmpty()) {
		return true;
	}

	QString editor = id().editor();
	//TODO: do a code generation for diagrams
	QString diagram = id().diagram();
	QStringList elements = mGraphicalAssistApi.editorManagerInterface().elements(editor, diagram);

	QList<StringPossibleEdge> stringPossibleEdges
			= mGraphicalAssistApi.editorManagerInterface().possibleEdges(editor, id().element());
	foreach (StringPossibleEdge pEdge, stringPossibleEdges) {
		QPair<bool, qReal::Id> edge(pEdge.second.first, Id(editor, diagram, pEdge.second.second));

		QStringList fromElements;
		QStringList toElements;
		foreach (QString const &element, elements) {
			if (mGraphicalAssistApi.editorManagerInterface().portTypes(Id(editor, diagram, element))
					.contains(pEdge.first.first)) {
				fromElements << element;
			}
			if (mGraphicalAssistApi.editorManagerInterface().portTypes(Id(editor, diagram, element))
					.contains(pEdge.first.second)) {
				toElements << element;
			}
		}

		foreach (QString const &fromElement, fromElements) {
			foreach (QString const &toElement, toElements) {
				QPair<qReal::Id, qReal::Id> nodes(Id(editor, diagram, fromElement),	Id(editor, diagram, toElement));
				PossibleEdge possibleEdge(nodes, edge);
				mPossibleEdges.push_back(possibleEdge);

			}
		}
	}

	return (!mPossibleEdges.isEmpty());
}

bool EdgeElement::isDividable()
{
	return mElementImpl->isDividable();
}

void EdgeElement::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::RightButton) {
		event->accept();
		if (event->buttons() & Qt::LeftButton) {
			mHandler->rejectMovingEdge();
		}
		return;
	}

	if ((event->modifiers() & Qt::AltModifier) && (event->button() == Qt::LeftButton)
			&& delPointActionIsPossible(event->pos())) {
		delPointHandler(event->pos());
		return;
	}

	Element::mousePressEvent(event);
	if ((mSrc && mSrc->isSelected() && isSelected()) || (mDst && mDst->isSelected() && isSelected())) {
		mDragType = wholeEdge;
	} else if (event->button() == Qt::LeftButton && !event->modifiers()) {
		mDragType = getPoint(event->pos());
		mHandler->startMovingEdge(mDragType, event->pos());
	}
}

void EdgeElement::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::RightButton) {
		event->accept();
		return;
	}

	if (mDragType == wholeEdge) {
		Element::mouseMoveEvent(event);
		adjustLink();
	} else {
		if (mDragType > mLine.size() - 1) {
			mDragType = overPointMax;
			mHandler->rejectMovingEdge();
			return;
		}

		mHandler->moveEdge(event->pos(), SettingsManager::value("ActivateGrid").toBool());
	}
}

void EdgeElement::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::RightButton) {
		event->accept();
		return;
	}

	Element::mouseReleaseEvent(event);

	if (mSrc) {
		mSrc->setPortsVisible(false);
	}

	if (mDst) {
		mDst->setPortsVisible(false);
	}

	mHandler->endMovingEdge();
}

void EdgeElement::delClosePoints()
{
	int const rad = kvadratik * 2;
	for (int i = 0; i < mLine.size() - 1; i++) {
		if (QLineF(mLine[i], mLine[i + 1]).length() < rad) {
			if (i != mLine.size() - 2) {
				mLine.remove(i + 1);
				i--;
			} else if (i != 0) {
				mLine.remove(i);
				i = i - 2;
			}
		}
	}
}

void EdgeElement::delCloseLinePoints()
{
	if (mIsLoop) { // rough prevention of transforming in the point (because #602)
		return;
	}

	prepareGeometryChange();

	int const width = kvadratik * 4;

	delClosePoints();

	for (int i = 0; i < mLine.size() - 2; i++) {
		QPainterPath path;
		QPainterPathStroker neighbourhood;
		neighbourhood.setWidth(width);
		path.moveTo(mLine[i]);
		path.lineTo(mLine[i + 2]);
		if (neighbourhood.createStroke(path).contains(mLine[i + 1])) {
			mLine.remove(i + 1);
			i--;
		}
	}

	for (int i = 0; i < mLine.size() - 2; i++) {
		QPainterPath path;
		QPainterPathStroker neighbourhood;
		neighbourhood.setWidth(width);
		path.moveTo(mLine[i + 1]);
		path.lineTo(mLine[i + 2]);
		if (neighbourhood.createStroke(path).contains(mLine[i])) {
			mLine.remove(i + 1);
			i--;
		}
	}

	for (int i = 0; i < mLine.size() - 2; i++) {
		QPainterPath path;
		QPainterPathStroker neighbourhood;
		neighbourhood.setWidth(width);
		path.moveTo(mLine[i]);
		path.lineTo(mLine[i + 1]);
		if (neighbourhood.createStroke(path).contains(mLine[i + 2])) {
			mLine.remove(i + 1);
			i--;
			// may be unneeds i-- here because exist previous deletes
		}
	}

	updateLongestPart();
}

bool EdgeElement::removeOneLinePoints(int startingPoint)
{
	if ((mLine[startingPoint].x() == mLine[startingPoint + 1].x() &&
		mLine[startingPoint].x() == mLine[startingPoint + 2].x()) ||
		(mLine[startingPoint].y() == mLine[startingPoint + 1].y() &&
		mLine[startingPoint].y() == mLine[startingPoint + 2].y()))
	{
		prepareGeometryChange();
		mLine.remove(startingPoint + 1);
		updateLongestPart();
		return true;
	} else {
		return false;
	}
}

// NOTE: using don`t forget about possible nodeElement`s overlaps (different Z-value)
// connecting to the innermost node at the point
NodeElement *EdgeElement::getNodeAt(QPointF const &position, bool isStart)
{
	QPainterPath circlePath;
	circlePath.addEllipse(mapToScene(position), 12, 12);
	QList<QGraphicsItem*> items = scene()->items(circlePath);

	if (isStart && items.contains(mSrc)) {
		return innermostChild(items, mSrc);
	} else if (!isStart && items.contains(mDst)) {
		return innermostChild(items, mDst);
	}

	foreach (QGraphicsItem *item, items) {
		NodeElement *e = dynamic_cast<NodeElement *>(item);
		if (e) {
			NodeElement *innerChild = innermostChild(items, e);
			if (innerChild) {
				return innerChild;
			}
		}
	}
	return NULL;
}

NodeElement *EdgeElement::innermostChild(QList<QGraphicsItem *> const &items, NodeElement *element) const
{
	foreach (NodeElement *child, element->childNodes()) {
		if (items.contains(child)) {
			NodeElement *innerChild = innermostChild(items, child);
			if (innerChild) {
				return innerChild;
			}
		}
	}

	if (element->numberOfPorts() > 0) {
		return element;
	}

	return NULL;
}

QList<ContextMenuAction*> EdgeElement::contextMenuActions(const QPointF &pos)
{
	QList<ContextMenuAction*> result;
	if (delPointActionIsPossible(pos)) {
		result.push_back(&mDelPointAction);
	}
	if (delSegmentActionIsPossible(pos)) {
		result.push_back(&mDelSegmentAction);
	}
	if (minimizeActionIsPossible()) {
		result.push_back(&mMinimizeAction);
	}
	if (reverseActionIsPossible()) {
		result.push_back(&mReverseAction);
	}
	return result;
}

bool EdgeElement::delPointActionIsPossible(const QPointF &pos)
{
	if (mIsLoop || (SettingsManager::value("LineType").toInt() != static_cast<int>(brokenLine))) {
		return false;
	}

	int pointIndex = getPoint(pos);
	return (pointIndex != noPort) && (pointIndex != mLine.count() - 1) && (pointIndex != 0);
}

bool EdgeElement::delSegmentActionIsPossible(const QPointF &pos)
{
	if (mIsLoop) {
		return false;
	}
	QPainterPath path;
	QPainterPathStroker ps;
	ps.setWidth(kvadratik / 2);
	for (int i = 0; i < mLine.size() - 1; ++i) {
		path.moveTo(mLine[i]);
		path.lineTo(mLine[i + 1]);
		if (ps.createStroke(path).contains(pos)) {
			return (i != 0 && (i + 1 != mLine.size() - 1));
		}
	}
	return false;
}

bool EdgeElement::minimizeActionIsPossible()
{
	return !mIsLoop && (SettingsManager::value("LineType").toInt() == static_cast<int>(brokenLine)) && (mLine.size() > 2);
}

bool EdgeElement::reverseActionIsPossible()
{
	if (mSrc) {
		if (mGraphicalAssistApi.editorManagerInterface().portTypes(mSrc->id().type()).toSet()
				.intersect(mElementImpl->toPortTypes().toSet()).empty()) {
			return false;
		}
	}

	if (mDst) {
		if (mGraphicalAssistApi.editorManagerInterface().portTypes(mDst->id().type()).toSet()
				.intersect(mElementImpl->fromPortTypes().toSet()).empty()) {
			return false;
		}
	}

	return true;
}

QList<PossibleEdge> EdgeElement::getPossibleEdges()
{
	return mPossibleEdges;
}

void EdgeElement::delPointHandler(QPointF const &pos)
{
	int pointIndex = getPoint(pos);
	if (pointIndex != noPort && pointIndex != mLine.count() - 1 && pointIndex != 0) {
		prepareGeometryChange();
		mLine.remove(pointIndex);
		arrangeAndAdjustHandler(pos);
	}
}

void EdgeElement::arrangeAndAdjustHandler(QPointF const &pos)
{
	Q_UNUSED(pos);
	adjustLink();
	arrangeSrcAndDst();
	mMoving = true;
	mGraphicalAssistApi.setPosition(id(), this->pos());
	mMoving = false;
	update();
	saveConfiguration();
}

void EdgeElement::setGraphicApiPos()
{
	mMoving = true;
	mGraphicalAssistApi.setPosition(id(), this->pos());
	mMoving = false;
}

bool EdgeElement::isBreakPointPressed()
{
	return mBreakPointPressed;
}

void EdgeElement::breakPointUnpressed()
{
	mBreakPointPressed = false;
}

void EdgeElement::breakPointHandler(QPointF const &pos)
{
	mBreakPointPressed = true;
	if (mLine.startsWith(pos.toPoint())) {
		mLine.insert(0, pos);
		mDragType = 0;
	}

	if (mLine.endsWith(pos.toPoint())) {
		mLine.insert(mLine.size() - 1, pos);
		mDragType = mLine.size() - 1;
	}
}

EdgeElement::NodeSide EdgeElement::defineNodePortSide(bool isStart)
{
	NodeElement *node = isStart ? mSrc : mDst;
	if (!node) {
		return isStart ? right : top;
	}

	QPointF pos = node->portPos(isStart ? mPortFrom : mPortTo);
	QRectF bounds = node->boundingRect();

	// divide bounding rectangle with it's diagonals, then determine in which part the port lies
	bool isTop = pos.y() < bounds.height() / bounds.width() * pos.x();
	bool isLeft = pos.y() / bounds.height() + pos.x() / bounds.width() < 1;

	if (isTop) {
		if (isLeft) {
			return top;
		} else {
			return right;
		}
	}
	if (isLeft) {
		return left;
	} else {
		return bottom;
	}
}

void EdgeElement::minimizeHandler(const QPointF &pos)
{
	Q_UNUSED(pos);
	QPolygonF newMLine;
	newMLine << mLine.first() << mLine.last();
	prepareGeometryChange();
	mLine = newMLine;
	updateLongestPart();
	adjustLink();
	arrangeSrcAndDst();
	saveConfiguration();
}

void EdgeElement::adjustLink()
{
	mHandler->adjust();
}

void EdgeElement::arrangeSrcAndDst()
{
	if (mSrc) {
		mSrc->arrangeLinks();
	} else if (mDst) {
		mDst->arrangeLinks();
	}
}

NodeElement *EdgeElement::src() const
{
	return mSrc;
}

NodeElement *EdgeElement::dst() const
{
	return mDst;
}

bool EdgeElement::isSrc(NodeElement const *node) const
{
	return (mSrc == node);
}

bool EdgeElement::isDst(NodeElement const *node) const
{
	return (mDst == node);
}

QPair<qreal, qreal> EdgeElement::portIdOn(NodeElement const *node) const
{
	if (mIsLoop && node == mSrc) {
		return qMakePair(mPortFrom, mPortTo);
	}
	if (node == mSrc) {
		return qMakePair(mPortFrom, -1.0);
	}
	if (node == mDst) {
		return qMakePair(-1.0, mPortTo);
	}
	return qMakePair(-1.0, -1.0);
}

QPointF EdgeElement::portArrangePoint(NodeElement const *node) const
{
	if (node == mSrc) {
		return mapToItem(mSrc, mHandler->portArrangePoint(true));
	}
	if (node == mDst) {
		return mapToItem(mDst, mHandler->portArrangePoint(false));
	}
	return QPointF();
}

QPointF EdgeElement::connectionPoint(NodeElement const *node) const
{
	if (node == mSrc) {
		return mapToItem(mSrc, mLine[0]);
	}
	if (node == mDst) {
		return mapToItem(mDst, mLine[mLine.count() - 1]);
	}
	return QPointF();

}

NodeElement* EdgeElement::otherSide(NodeElement const *node) const
{
	if (node == mSrc) {
		return mDst;
	}
	if (node == mDst) {
		return mSrc;
	}
	return 0;
}

void EdgeElement::reconnectToNearestPorts(bool reconnectSrc, bool reconnectDst)
{
	mHandler->reconnect(reconnectSrc, reconnectDst);
}

void EdgeElement::updateData()
{
	if (mMoving) {
		return;
	}

	if (mModelUpdateIsCalled) {
		Element::updateData();
		mElementImpl->updateData(this);
		update();
		mModelUpdateIsCalled = false;
		return;
	}

	Element::updateData();

	setPos(mGraphicalAssistApi.position(id()));
	QPolygonF newLine = mGraphicalAssistApi.configuration(id());

	if (!newLine.isEmpty()) {
		mLine = newLine;
	}

	qReal::Id idFrom = mGraphicalAssistApi.from(id());
	qReal::Id idTo = mGraphicalAssistApi.to(id());

	if (mSrc) {
		mSrc->delEdge(this);
	}
	if (mDst && (mDst != mSrc)) {
		mDst->delEdge(this);
	}

	mSrc = dynamic_cast<NodeElement *>(static_cast<EditorViewScene *>(scene())->getElem(idFrom));
	mDst = dynamic_cast<NodeElement *>(static_cast<EditorViewScene *>(scene())->getElem(idTo));

	if (mSrc) {
		mSrc->addEdge(this);
	}
	if (mDst && (mDst != mSrc)) {
		mDst->addEdge(this);
	}

	mPortFrom = mGraphicalAssistApi.fromPort(id());
	mPortTo = mGraphicalAssistApi.toPort(id());

	mElementImpl->updateData(this);

	update();
	updateLongestPart();
}

void EdgeElement::removeLink(NodeElement const *from)
{
	if (mSrc == from) {
		mSrc = NULL;
	}

	if (mDst == from) {
		mDst = NULL;
	}

	mIsLoop = false;

	highlight();
}

QStringList EdgeElement::fromPortTypes() const
{
	return mElementImpl->fromPortTypes();
}

QStringList EdgeElement::toPortTypes() const
{
	return mElementImpl->toPortTypes();
}

void EdgeElement::placeStartTo(QPointF const &place)
{
	mLine[0] = place;
}

void EdgeElement::placeEndTo(QPointF const &place)
{
	prepareGeometryChange();
	mLine[mLine.size() - 1] = place;

	mHandler->adjust();

	mModelUpdateIsCalled = true;
	mGraphicalAssistApi.setPosition(id(), this->pos());

	updateLongestPart();
}

void EdgeElement::moveConnection(NodeElement *node, qreal const portId) {
	//expected that the id will change only fractional part
	if ((!mIsLoop || ((int) mPortFrom == (int) portId)) && (node == mSrc)) {
		setFromPort(portId);
	}
	if ((!mIsLoop || ((int) mPortTo == (int) portId)) && (node == mDst)) {
		setToPort(portId);
	}
}

void EdgeElement::drawStartArrow(QPainter *painter) const
{
	if (mElementImpl)
		mElementImpl->drawStartArrow(painter);
}

void EdgeElement::drawEndArrow(QPainter *painter) const
{
	if (mElementImpl)
		mElementImpl->drawEndArrow(painter);
}

void EdgeElement::setColorRect(bool bl) // method is empty
{
	Q_UNUSED(bl);
}

void EdgeElement::highlight(QColor const color)
{
	mColor = color;
	update();
}

void EdgeElement::redrawing(QPointF const &pos)
{
	Q_UNUSED(pos);
	if (mIsLoop) { // it's row prevention of transform to the point before fix #602
		createLoopEdge();
	}
	initLineHandler();
	layOut();
	setGraphicApiPos();
	saveConfiguration();
}

QPointF* EdgeElement::haveIntersection(QPointF const &pos1, QPointF const &pos2, QPointF const &pos3
		, QPointF const &pos4)
{
	// use equation of line for solution
	qreal a1 = pos1.y() - pos2.y();
	qreal b1 = pos2.x() - pos1.x();
	qreal c1 = pos1.x() * pos2.y() - pos2.x() * pos1.y();
	qreal a2 = pos3.y() - pos4.y();
	qreal b2 = pos4.x() - pos3.x();
	qreal c2 = pos3.x() * pos4.y() - pos4.x() * pos3.y();
	if (abs(a2 * b1 - a1 * b2) < epsilon) {
		return NULL;
	}
	qreal y = 0;
	qreal x = 0;
	if (abs(a2) > epsilon) {
		y = (-c1 + a1 * c2 / a2)/(-(a1 / a2) * b2 + b1);
		x = (b2 * y + c2) / (-a2);
	} else {
		y = pos3.y();
		x = -(b1 * y + c1) / a1;
	}
	QPointF* cut = new QPointF(x, y);

	QPainterPath path;
	QPainterPathStroker ps;
	ps.setWidth(1);
	path.moveTo(pos1);
	path.lineTo(pos2);
	if (ps.createStroke(path).contains(QPointF(cut->x(), cut->y()))) {
		QPainterPath path1;
		QPainterPathStroker ps1;
		ps1.setWidth(1);
		path1.moveTo(pos3);
		path1.lineTo(pos4);
		if (ps1.createStroke(path1).contains(QPointF(cut->x(), cut->y()))) {
			return cut;
		}
	}
	return NULL;

}

EdgeData& EdgeElement::data()
{
	mData.id = id();
	mData.logicalId = logicalId();
	mData.srcId = src() ? src()->id() : Id::rootId();
	mData.dstId = dst() ? dst()->id() : Id::rootId();

	mData.portFrom = mPortFrom;
	mData.portTo = mPortTo;

	mData.configuration = mGraphicalAssistApi.configuration(mId);
	mData.pos = mGraphicalAssistApi.position(mId);

	return mData;
}

void EdgeElement::deleteLoop(int startPos)
{
	for (int i = startPos; i < mLine.size() - 3; ++i)
	{
		bool isCut = false;
		for (int j = i + 2; j < mLine.size() - 1; ++j)
		{
			QPointF* cut = haveIntersection(mLine[i], mLine[i + 1], mLine[j], mLine[j + 1]);
			if (cut)
			{
				if ((i != 0) || !((j == mLine.size() - 2)
						&& (QLineF(mLine.first(), mLine.last()).length() < (kvadratik * 2))))
				{
					QPointF const pos = QPointF(cut->x(), cut->y());
					QPainterPath path;
					QPainterPathStroker ps;
					ps.setWidth(kvadratik);
					for (int k = 0; k < mLine.size() - 1; ++k) {
						path.moveTo(mLine[k]);
						path.lineTo(mLine[k + 1]);
						if (ps.createStroke(path).contains(pos)) {
							mLine.insert(k + 1, pos);
							break;
						}
					}
					mLine.remove(i + 2, j - i);
					deleteLoop(i);
					isCut = true;
					break;
				}
				delete(cut);
			}
		}
		if (isCut)
			break;
	}
}

void EdgeElement::deleteLoops()
{
	if (mIsLoop) {
		return;
	}
	prepareGeometryChange();
	deleteLoop(0);
}

void EdgeElement::deleteSegmentHandler(QPointF const &pos)
{
	prepareGeometryChange();
	QPainterPath path;
	QPainterPathStroker ps;
	ps.setWidth(kvadratik / 2);
	for (int i = 0; i < mLine.size() - 1; ++i) {
		path.moveTo(mLine[i]);
		path.lineTo(mLine[i + 1]);
		if (ps.createStroke(path).contains(pos) && i != 0 && (i + 1 != mLine.size() - 1)) {
			delPointHandler(mLine[i]);
			delPointHandler(mLine[i]);
			for (int i = 0; i < mLine.size() - 2; i++) {
				if (removeOneLinePoints(i)) {
					i--;
				}
			}
			updateLongestPart();
			break;
		}
	}
	arrangeAndAdjustHandler(pos);
	saveConfiguration();
}

void EdgeElement::saveConfiguration()
{
	mModelUpdateIsCalled = true;
	mGraphicalAssistApi.setConfiguration(id(), mLine.toPolygon());
}

void EdgeElement::reverseHandler(QPointF const &pos1)
{
	Q_UNUSED(pos1);

	int length = mLine.size();
	for (int i = 0; i < (length >> 1); ++i) {
		QPointF tmp(mLine[i]);
		mLine[i] = mLine[length - 1 - i];
		mLine[length - 1 - i] = tmp;
	}

	reversingReconnectToPorts(mDst, mSrc);

	arrangeAndAdjustHandler(QPointF());

	updateLongestPart();
}

void EdgeElement::reversingReconnectToPorts(NodeElement *newSrc, NodeElement *newDst)
{
	mMoving = true;

	if (mIsLoop) {
		connectLoopEdge(newSrc);
		return;
	}

	setPos(pos() + mLine.first());
	mLine.translate(-mLine.first());

	mPortFrom = newSrc ? newSrc->portId(mapToItem(newSrc, mLine.first()), fromPortTypes()) : -1.0;
	mPortTo = newDst ? newDst->portId(mapToItem(newDst, mLine.last()), toPortTypes()) : -1.0;

	setSrc(NULL);
	setDst(NULL);

	if (mPortFrom >= -epsilon) {
		mSrc = newSrc;
		mSrc->addEdge(this);
	}
	if (mPortTo >= -epsilon) {
		mDst = newDst;
		mDst->addEdge(this);
	}
	mGraphicalAssistApi.setFrom(id(), (mSrc ? mSrc->id() : Id::rootId()));
	mGraphicalAssistApi.setFromPort(id(), mPortFrom);
	mGraphicalAssistApi.setTo(id(), (mDst ? mDst->id() : Id::rootId()));
	mGraphicalAssistApi.setToPort(id(), mPortTo);
	mLogicalAssistApi.setFrom(logicalId(), (mSrc ? mSrc->logicalId() : Id::rootId()));
	mLogicalAssistApi.setTo(logicalId(), (mDst ? mDst->logicalId() : Id::rootId()));

	mMoving = false;
}

QVariant EdgeElement::itemChange(GraphicsItemChange change, QVariant const &value)
{
	switch (change) {
	case ItemPositionHasChanged:
		if (mIsLoop) {
			return value;
		}
		if (isSelected() && (mSrc || mDst) && !mMoving) {
			if (mSrc && !mSrc->isSelected()) {
				prepareGeometryChange();
				mLine.first() = mapFromItem(mSrc, mSrc->portPos(mPortFrom));
			}
			if (mDst && !mDst->isSelected()) {
				prepareGeometryChange();
				mLine.last() = mapFromItem(mDst, mDst->portPos(mPortTo));
			}
		}
		updateLongestPart();
		return value;
	default:
		return QGraphicsItem::itemChange(change, value);
	}
}

void EdgeElement::setSrc(NodeElement *node)
{
	if (mSrc) {
		mSrc->delEdge(this);
	}
	mSrc = node;
	mGraphicalAssistApi.setFrom(id(), mSrc ? mSrc->id() : Id::rootId());
	mLogicalAssistApi.setFrom(logicalId(), mSrc ? mSrc->logicalId() : Id::rootId());
	if (node) {
		mSrc->addEdge(this);
	}
}

void EdgeElement::setDst(NodeElement *node)
{
	if (mDst) {
		mDst->delEdge(this);
	}
	mDst = node;
	mGraphicalAssistApi.setTo(id(), mDst ? mDst->id() : Id::rootId());
	mLogicalAssistApi.setTo(logicalId(), mDst ? mDst->logicalId() : Id::rootId());
	if (node) {
		mDst->addEdge(this);
	}
}

void EdgeElement::tuneForLinker()
{
	mMoving = true;
	setPos(pos() + mLine.first());
	mLine.translate(-mLine.first());
	mPortFrom = mSrc ? mSrc->portId(mapToItem(mSrc, mLine.first()), fromPortTypes()) : -1.0;
	mGraphicalAssistApi.setFromPort(id(), mPortFrom);
	layOut();
	mGraphicalAssistApi.setPosition(id(), pos());
	mMoving = false;
}

bool EdgeElement::isLoop()
{
	return mIsLoop;
}

void EdgeElement::alignToGrid()
{
	prepareGeometryChange();
	mHandler->alignToGrid();
	updateLongestPart();
}
