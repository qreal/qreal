/* Copyright 2007-2016 QReal Research Group
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

/** @file edgeElement.cpp
*	@brief class for an edge on a diagram
**/

#include <QtCore/QtMath>
#include <QtWidgets/QStyleOptionGraphicsItem>
#include <QtWidgets/QStyle>
#include <QtGui/QTextDocument>
#include <QtWidgets/QMenu>

#include <math.h>
#include <qrkernel/logging.h>
#include <qrutils/mathUtils/geometry.h>
#include <models/models.h>

#include "editor/edgeElement.h"
#include "editor/nodeElement.h"
#include "editor/editorViewScene.h"
#include "editor/labels/label.h"
#include "editor/labels/labelFactory.h"

#include "editor/private/lineFactory.h"
#include "editor/private/lineHandler.h"

using namespace qReal;
using namespace qReal::gui::editor;
using namespace enums;

const qreal epsilon = 0.00000000001;

const int rightRotation = 1;// the difference between the elements of NodeSide
const int maxReductCoeff = 16;

/** @brief indicator of edges' movement */

EdgeElement::EdgeElement(
		ElementImpl *impl
		, const Id &id
		, const models::Models &models)
	: Element(impl, id, models)
	, mPenStyle(Qt::SolidLine)
	, mPenWidth(1)
	, mPenColor(Qt::black)
	, mSrc(nullptr)
	, mDst(nullptr)
	, mLineFactory(new LineFactory(this))
	, mHandler(nullptr)
	, mPortFrom(0)
	, mPortTo(0)
	, mDragType(noPort)
	, mLongPart(0)
	, mReverseAction(tr("Reverse"), this)
	, mChangeShapeAction(tr("Change shape type"), this)
	, mBreakPointPressed(false)
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

	LabelFactory factory(models.graphicalModelAssistApi(), mId);
	QList<LabelInterface*> titles;

	mElementImpl->init(factory, titles);
	for (LabelInterface *titleIface : titles) {
		Label *title = dynamic_cast<Label*>(titleIface);
		if (!title) {
			continue;
		}

		title->init(boundingRect());
		title->setParentItem(this);
		title->setShouldCenter(false);
		mLabels.append(title);
	}

	mShapeType = static_cast<enums::linkShape::LinkShape>(SettingsManager::value("LineType").toInt());
	initLineHandler();
	mChangeShapeAction.setMenu(mLineFactory->shapeTypeMenu());
}

EdgeElement::~EdgeElement()
{
	if (mSrc) {
		mSrc->delEdge(this);
	}

	if (mDst) {
		mDst->delEdge(this);
	}

	delete mElementImpl;
	delete mLineFactory;
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
	mHandler = mLineFactory->createHandler(mShapeType);
	mHandler->connectAction(&mReverseAction, this, SLOT(reverse()));
}

void EdgeElement::changeShapeType(const linkShape::LinkShape shapeType)
{
	mShapeType = shapeType;
	mGraphicalAssistApi.mutableGraphicalRepoApi().setProperty(id(), "linkShape"
			, mLineFactory->shapeToString(shapeType));
	initLineHandler();
	layOut();
}

QRectF EdgeElement::boundingRect() const
{
	return mLine.boundingRect().adjusted(-20, -20, 20, 20);
}

QPolygonF EdgeElement::line() const
{
	return mLine;
}

void EdgeElement::setLine(const QPolygonF &line)
{
	prepareGeometryChange();
	mLine = line;
	saveConfiguration();
	update();
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

void EdgeElement::setFromPort(const qreal fromPort)
{
	mPortFrom = fromPort;
	mModelUpdateIsCalled = true;
	mGraphicalAssistApi.setFromPort(id(), mPortFrom);
}

void EdgeElement::setToPort(const qreal toPort)
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

void EdgeElement::paintEdge(QPainter *painter, const QStyleOptionGraphicsItem *option, bool drawSavedLine) const
{
	painter->save();

	if (drawSavedLine) {
		const QColor color = QColor(SettingsManager::value("oldLineColor").toString());
		setEdgePainter(painter, edgePen(painter, color, Qt::DashDotLine, mPenWidth), 0.5);
	} else {
		setEdgePainter(painter, edgePen(painter, mColor, mPenStyle, mPenWidth), painter->opacity());
	}

	mHandler->drawLine(painter, drawSavedLine);

	drawArrows(painter, drawSavedLine);

	if ((option->state & (QStyle::State_Selected | QStyle::State_MouseOver)) && !drawSavedLine) {
		painter->setBrush(Qt::SolidPattern);
		mHandler->drawPorts(painter);
	}

	painter->restore();
}

void EdgeElement::drawArrows(QPainter *painter, bool savedLine) const
{
	Qt::PenStyle style(QPen(painter->pen()).style());

	painter->save();

	if (savedLine) {
		const QColor color = QColor(SettingsManager::value("oldLineColor").toString());
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
	ps.setWidth(stripeWidth);

	path = ps.createStroke(path);

	for (const QPointF &point : mLine) {
		path.addRect(QRectF(point - QPointF(stripeWidth / 2, stripeWidth / 2)
				, QSizeF(stripeWidth, stripeWidth)).adjusted(1, 1, -1, -1));
	}

	return path;
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
	}
}

void EdgeElement::connectToPort()
{
	mMoving = true;

	NodeElement *newSrc = getNodeAt(mLine.first(), true);
	NodeElement *newDst = getNodeAt(mLine.last(), false);

	mIsLoop = ((newSrc == newDst) && newSrc);

	if (mIsLoop) {
		connectLoopEdge(newSrc);
		createLoopEdge();
		return;
	}

	mPortFrom = newSrc ? newSrc->portId(mapToItem(newSrc, mLine.first()), fromPortTypes()) : -1.0;
	mPortTo = newDst ? newDst->portId(mapToItem(newDst, mLine.last()), toPortTypes()) : -1.0;

	if (mSrc) {
		mSrc->delEdge(this);
		mSrc = nullptr;
	}

	if (mDst) {
		mDst->delEdge(this);
		mDst = nullptr;
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

	setGraphicApiPos();
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

	setGraphicApiPos();
	saveConfiguration();
}

void EdgeElement::connectLoopEdge(NodeElement *newMaster)
{
	mPortFrom = newMaster ? newMaster->portId(mapToItem(newMaster, mLine.first()), fromPortTypes()) : -1.0;
	mPortTo = newMaster ? newMaster->portId(mapToItem(newMaster, mLine.last()), toPortTypes()) : -1.0;

	if (mSrc) {
		mSrc->delEdge(this);
	}

	if (mDst) {
		mDst->delEdge(this);
	}

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

	const NodeSide startSide = defineNodePortSide(true);
	const NodeSide endSide = defineNodePortSide(false);

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

	setGraphicApiPos();
	saveConfiguration();

	mIsLoop = true;
}

QPointF EdgeElement::boundingRectIndent(const QPointF &point, EdgeElement::NodeSide direction)
{
	QPointF newPoint;
	QRectF bounds = mSrc->boundingRect();
	int reductFactor = indentReductCoeff();

	switch (direction)
	{
	case top:
	{
		QPointF topPoint = mapToItem(mSrc, QPointF(point.x(), 0));
		newPoint = mapFromItem(mSrc, QPointF(topPoint.x(), bounds.top() - bounds.height() / reductFactor));
		break;
	}

	case bottom:
	{
		QPointF bottomPoint = mapToItem(mSrc, QPointF(point.x(), 0));
		newPoint = mapFromItem(mSrc, QPointF(bottomPoint.x(), bounds.bottom() + bounds.height() / reductFactor));
		break;
	}

	case left:
	{
		QPointF leftPoint = mapToItem(mSrc, QPointF(0, point.y()));
		newPoint = mapFromItem(mSrc, QPointF(bounds.left() - bounds.width() / reductFactor, leftPoint.y()));
		break;
	}

	case right:
	{
		QPointF rightPoint = mapToItem(mSrc, QPointF(0, point.y()));
		newPoint = mapFromItem(mSrc, QPointF(bounds.right() + bounds.width() / reductFactor, rightPoint.y()));
		break;
	}

	default:
		qDebug() << "incorrect direction";
	}

	return newPoint;
}

int EdgeElement::indentReductCoeff()
{
	return (maxReductCoeff - SettingsManager::value("LoopEdgeBoundsIndent").toInt());
}

void EdgeElement::searchNextPort()
{
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
	for (StringPossibleEdge pEdge : stringPossibleEdges) {
		QPair<bool, Id> edge(pEdge.second.first, Id(editor, diagram, pEdge.second.second));

		QStringList fromElements;
		QStringList toElements;
		for (const QString &element : elements) {
			if (mGraphicalAssistApi.editorManagerInterface().portTypes(Id(editor, diagram, element))
					.contains(pEdge.first.first)) {
				fromElements << element;
			}

			if (mGraphicalAssistApi.editorManagerInterface().portTypes(Id(editor, diagram, element))
					.contains(pEdge.first.second)) {
				toElements << element;
			}
		}

		for (const QString &fromElement : fromElements) {
			for (const QString &toElement : toElements) {
				QPair<Id, Id> nodes(Id(editor, diagram, fromElement),	Id(editor, diagram, toElement));
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

	Element::mousePressEvent(event);

	if ((mSrc && mSrc->isSelected() && isSelected()) || (mDst && mDst->isSelected() && isSelected())) {
		mDragType = wholeEdge;
		if (mSrc && mSrc->isSelected()) {
			mSrc->startResize();
		} else {
			mDst->startResize();
		}
	} else if (event->button() == Qt::LeftButton && !event->modifiers()) {
		mDragType = mHandler->startMovingEdge(event->pos());
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
			mDragType = noPort;
			mHandler->rejectMovingEdge();
			return;
		}

		mHandler->moveEdge(event->pos());
	}
}

void EdgeElement::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::RightButton) {
		event->accept();
		return;
	}

	Element::mouseReleaseEvent(event);

	if (mDragType == wholeEdge) {
		if (mSrc && mSrc->isSelected()) {
			mSrc->endResize();
		} else {
			mDst->endResize();
		}
	} else {
		mHandler->endMovingEdge();
	}
}

// NOTE: using don`t forget about possible nodeElement`s overlaps (different Z-value)
// connecting to the innermost node at the point
NodeElement *EdgeElement::getNodeAt(const QPointF &position, bool isStart)
{
	QPainterPath circlePath;
	const int searchAreaRadius = SettingsManager::value("IndexGrid", 25).toInt() / 2;
	const QPointF positionInSceneCoordinates = mapToScene(position);
	circlePath.addEllipse(positionInSceneCoordinates, searchAreaRadius, searchAreaRadius);
//	qDebug() << scene()->items();
//	qDebug() << circlePath;
	QList<QGraphicsItem*> const items = scene()->items(circlePath);

	qreal minimalDistance = 10e10;  // Very large number
	NodeElement *closestNode = nullptr;

	// Searching for the node with closest port to our point
	for (QGraphicsItem * const item : items) {
		NodeElement * const currentNode = dynamic_cast<NodeElement *>(item);
		if (currentNode) {
			const QPointF nearestPortPoint = currentNode->closestPortPoint(positionInSceneCoordinates
					, isStart ? fromPortTypes() : toPortTypes());
			const qreal currentDistance = mathUtils::Geometry::distance(positionInSceneCoordinates, nearestPortPoint);
			if (currentDistance < minimalDistance) {
				minimalDistance = currentDistance;
				closestNode = currentNode;
			}
		}
	}

	return closestNode;
}

NodeElement *EdgeElement::innermostChild(const QList<QGraphicsItem *> &items, NodeElement * const element) const
{
	for (NodeElement *child : element->childNodes()) {
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

	return nullptr;
}

bool EdgeElement::reverseActionIsPossible() const
{
	if (mIsLoop) {
		return false;
	}

	return !(mSrc && !canConnect(mSrc, false)) && !(mDst && !canConnect(mDst, true));
}

bool EdgeElement::canConnect(const NodeElement * const node, bool from) const
{
	QSet<QString> nodePortTypes = mGraphicalAssistApi.editorManagerInterface().portTypes(node->id().type()).toSet();
	QSet<QString> edgePortTypes = from ? mElementImpl->fromPortTypes().toSet() : mElementImpl->toPortTypes().toSet();

	return !nodePortTypes.intersect(edgePortTypes).empty();
}

void EdgeElement::reverse()
{
	const int length = mLine.size();
	for (int i = 0; i < length / 2; ++i) {
		qSwap(mLine[i], mLine[length - 1 - i]);
	}

	reversingReconnectToPorts(mDst, mSrc);
	layOut();
}

void EdgeElement::reversingReconnectToPorts(NodeElement *newSrc, NodeElement *newDst)
{
	mMoving = true;

	mPortFrom = newSrc ? newSrc->portId(mapToItem(newSrc, mLine.first()), fromPortTypes()) : -1.0;
	mPortTo = newDst ? newDst->portId(mapToItem(newDst, mLine.last()), toPortTypes()) : -1.0;

	setSrc(nullptr);
	setDst(nullptr);

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

void EdgeElement::setGraphicApiPos()
{
	mMoving = true;
	mGraphicalAssistApi.setPosition(id(), this->pos());
	mMoving = false;
}

void EdgeElement::saveConfiguration()
{
	mModelUpdateIsCalled = true;
	mGraphicalAssistApi.setConfiguration(id(), mLine.toPolygon());
}

bool EdgeElement::isBreakPointPressed()
{
	return mBreakPointPressed;
}

void EdgeElement::breakPointUnpressed()
{
	mBreakPointPressed = false;
}

void EdgeElement::breakPointHandler(const QPointF &pos)
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

QList<PossibleEdge> EdgeElement::getPossibleEdges()
{
	return mPossibleEdges;
}

EdgeElement::NodeSide EdgeElement::defineNodePortSide(bool isStart) const
{
	const NodeElement * const node = isStart ? mSrc : mDst;
	if (!node) {
		return isStart ? right : top;
	}

	QPointF pos = node->portPos(isStart ? mPortFrom : mPortTo);
	const QRectF bounds = node->contentsRect();

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

void EdgeElement::adjustLink()
{
	mHandler->adjust();
}

NodeElement *EdgeElement::src() const
{
	return mSrc;
}

NodeElement *EdgeElement::dst() const
{
	return mDst;
}

QPair<qreal, qreal> EdgeElement::portIdOn(const NodeElement *node) const
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

EdgeArrangeCriteria EdgeElement::arrangeCriteria(const NodeElement *node, const QLineF &portLine) const
{
	return mHandler->arrangeCriteria(node, portLine);
}

NodeElement * EdgeElement::otherSide(const NodeElement *node) const
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

	Id idFrom = mGraphicalAssistApi.from(id());
	Id idTo = mGraphicalAssistApi.to(id());

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
	highlight(isHanging() ? Qt::red : mPenColor);
}

void EdgeElement::removeLink(const NodeElement *from)
{
	if (mSrc == from) {
		mSrc = nullptr;
	}

	if (mDst == from) {
		mDst = nullptr;
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

void EdgeElement::placeStartTo(const QPointF &place)
{
	mLine[0] = place;
}

void EdgeElement::placeEndTo(const QPointF &place)
{
	prepareGeometryChange();
	mLine[mLine.size() - 1] = place;

	mHandler->adjust();

	mModelUpdateIsCalled = true;
	mGraphicalAssistApi.setPosition(id(), this->pos());

	updateLongestPart();
}

void EdgeElement::moveConnection(NodeElement *node, const qreal portId)
{
	// Expected that the id will change only fractional part
	if ((!mIsLoop || ((int) mPortFrom == (int) portId)) && (node == mSrc)) {
		setFromPort(portId);
	}

	if ((!mIsLoop || ((int) mPortTo == (int) portId)) && (node == mDst)) {
		setToPort(portId);
	}
}

void EdgeElement::arrangeLinearPorts()
{
	if (mSrc) {
		mSrc->arrangeLinearPorts();
	}

	if (mDst) {
		mDst->arrangeLinearPorts();
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

void EdgeElement::highlight(const QColor &color)
{
	mColor = color;
	update();
}

EdgeInfo EdgeElement::data()
{
	EdgeInfo result(id()
			, logicalId()
			, mLogicalAssistApi.parent(logicalId())
			, mGraphicalAssistApi.parent(id())
			, mPortFrom
			, mPortTo
			, mGraphicalAssistApi.configuration(mId)
			, mShapeType
	);

	result.setSrcId(src() ? src()->id() : Id::rootId());
	result.setDstId(dst() ? dst()->id() : Id::rootId());

	const QMap<QString, QVariant> properties = mGraphicalAssistApi.properties(logicalId());
	for (const QString &property : properties.keys()) {
		if (property != "from" && property != "to") {
			result.setLogicalProperty(property, properties[property]);
		}
	}

	result.setGraphicalProperty("position", mGraphicalAssistApi.position(mId));

	return result;
}

QVariant EdgeElement::itemChange(GraphicsItemChange change, const QVariant &value)
{
	switch (change)
	{
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

bool EdgeElement::isHanging() const
{
	return !mSrc || !mDst;
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


void EdgeElement::setPos(qreal x, qreal y)
{
	setPos(QPointF(x, y));
}

void EdgeElement::setPos(const QPointF &pos)
{
	if (qIsNaN(pos.x()) || qIsNaN(pos.y())) {
		Element::setPos(QPointF());
		QLOG_WARN() << "NaN passed to EdgeElement::setPos(). That means that something went wrong."\
				"Learn to reproduce this message. The position has been set to (0,0).";
	} else {
		Element::setPos(pos);
	}
}
