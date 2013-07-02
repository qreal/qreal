/** @file edgeelement.cpp
 * 	@brief class for an edge on a diagram
 * */
#include <QtWidgets/QStyleOptionGraphicsItem>
#include <QtWidgets/QStyle>
#include <QtGui/QTextDocument>
#include <QtWidgets/QMenu>
#include <QDebug>
#include <math.h>

#include "edgeElement.h"
#include "nodeElement.h"
#include "private/reshapeEdgeCommand.h"
#include "../view/editorViewScene.h"

using namespace qReal;

const double pi = 3.14159265358979;

const qreal epsilon = 0.00000000001;

/** @brief indicator of edges' movement */

EdgeElement::EdgeElement(ElementImpl *impl)
		: Element(impl)
		, mPenStyle(Qt::SolidLine), mPenWidth(1), mPenColor(Qt::black), mStartArrowStyle(NO_ARROW), mEndArrowStyle(NO_ARROW)
		, mSrc(NULL), mDst(NULL)
		, mPortFrom(0), mPortTo(0)
		, mDragPoint(noPort), mLongPart(0)
		, mAddPointAction(tr("Add point"), this)
		, mDelPointAction(tr("Delete point"), this)
		, mMinimizeAction(tr("Remove all points"), this)
		, mDelSegmentAction(tr("Remove segment"), this)
		, mReverseAction(tr("Reverse"), this)
		, mModelUpdateIsCalled(false)
		, mIsLoop(false)
		, mReshapeCommand(NULL)
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
	mLastLineIsLoop = false;

	mLeftButtonIsPressed = false;
	mSavedLineForChanges = mLine;

	setAcceptHoverEvents(true);

	connect(&mAddPointAction, SIGNAL(triggered(QPointF const &)), SLOT(addClosestPointHandler(QPointF const &)));
	connect(&mDelPointAction, SIGNAL(triggered(QPointF const &)), SLOT(delPointHandler(QPointF const &)));
	connect(&mDelPointAction, SIGNAL(triggered(QPointF const &)), SLOT(arrangeAndAdjustHandler(QPointF const &)));
	connect(&mReverseAction, SIGNAL(triggered(QPointF const)), SLOT(reverseHandler(QPointF const &)));
	connect(&mMinimizeAction, SIGNAL(triggered(QPointF const &)), SLOT(minimizeHandler(QPointF const &)));
	connect(&mDelSegmentAction, SIGNAL(triggered(QPointF const &)), SLOT(deleteSegmentHandler(QPointF const &)));
	connect(&mDelSegmentAction, SIGNAL(triggered(QPointF const &)), SLOT(arrangeAndAdjustHandler(QPointF const &)));

	mChaoticEdition = SettingsManager::value("ChaoticEdition").toBool();

	ElementTitleFactory factory;

	QList<ElementTitleInterface*> titles;
	mElementImpl->init(factory, titles);
	foreach (ElementTitleInterface *titleIface, titles) {
		ElementTitle *title = dynamic_cast<ElementTitle*>(titleIface);
		if (!title) {
			continue;
		}
		title->init(boundingRect());
		title->setParentItem(this);
		mTitles.append(title);
	}
}

EdgeElement::~EdgeElement()
{
	if (mSrc)
		mSrc->delEdge(this);
	if (mDst)
		mDst->delEdge(this);

	delete mElementImpl;
}

void EdgeElement::initTitles()
{
	Element::initTitles();
	updateLongestPart();
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
	mLine = line;
	saveConfiguration(QPointF());
}

static double lineAngle(const QLineF &line)
{
	double angle = ::acos(line.dx() / line.length());
	if (line.dy() >= 0)
		angle = 2 * pi - angle;

	return angle * 180 / pi;
}

static void drawChaosStar(QPainter *painter)
{
	painter->save();
	QPen pen;
	QColor color;
	color.setNamedColor("#c3dcc4");
	pen.setColor(color);
	painter->setPen(pen);

	for (int i = 0; i < 8; ++i) {
		painter->rotate(45 * i);
		painter->drawLine(0, 2, 0, 11);

		painter->save();
		painter->translate(0, 11);
		painter->rotate(30);
		painter->drawLine(0, 0, 0, -3);
		painter->rotate(-60);
		painter->drawLine(0, 0, 0, -3);
		painter->restore();
	}

	painter->drawArc(-2, -2, 4, 4, 0, 5760);
	painter->drawArc(-6, -6, 12, 12, 0, 5760);

	painter->restore();
}

void EdgeElement::drawPort(QPainter *painter) const
{
	QPen pen;
	QColor color;
	QPointF p1(-0.25,0);
	QPointF p2(0.25,0);

	color.setNamedColor("#c3dcc4");
	pen.setWidth(12);
	pen.setColor(color);
	painter->setPen(pen);
	painter->drawLine(p1, p2);

	color.setNamedColor("#465945");
	pen.setWidth(3);
	pen.setColor(color);
	painter->setPen(pen);
	painter->drawLine(p1, p2);
}

void EdgeElement::drawPorts(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
	if (option->state & (QStyle::State_Selected | QStyle::State_MouseOver)) {
		painter->setBrush(Qt::SolidPattern);
		foreach (QPointF const point, mLine) {
			// if the square mode is on then user can't edit links manually so there is no need in showing
			// intermediate link points to him
			if (SettingsManager::value("SquareLine").toBool() && !(point == mLine.first() || point == mLine.last())) {
				continue;
			}
			painter->save();
			painter->translate(point);
			if (mChaoticEdition) {
				drawChaosStar(painter);
			} else {
				drawPort(painter);
			}
			painter->restore();
		}
	}
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

void EdgeElement::paintSavedEdge(QPainter *painter) const
{
	if (!SettingsManager::value("PaintOldEdgeMode").toBool()) {
		return;
	}
	QColor color = QColor(SettingsManager::value("oldLineColor").toString());
	if (!(mSavedLineForChanges.size() < 2) && mLeftButtonIsPressed) {
		painter->save();
		setEdgePainter(painter, edgePen(painter, color, Qt::DashDotLine, mPenWidth), 0.5);
		painter->drawPolyline(mSavedLineForChanges);
		painter->restore();

		painter->save();
		painter->translate(mSavedLineForChanges[0]);
		painter->rotate(90 - lineAngle(QLineF(mSavedLineForChanges[1], mSavedLineForChanges[0])));
		setEdgePainter(painter, edgePen(painter, color, Qt::SolidLine, 3), 0.5);
		drawStartArrow(painter);
		painter->restore();

		painter->save();
		painter->translate(mSavedLineForChanges[mSavedLineForChanges.size() - 1]);
		painter->rotate(90 - lineAngle(QLineF(mSavedLineForChanges[mSavedLineForChanges.size() - 2], mSavedLineForChanges[mSavedLineForChanges.size() - 1])));
		setEdgePainter(painter, edgePen(painter, color, Qt::SolidLine, 3), 0.5);
		drawEndArrow(painter);
		painter->restore();
	}
}

void EdgeElement::paintChangedEdge(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
	Qt::PenStyle style(QPen(painter->pen()).style());

	painter->save();
	setEdgePainter(painter, edgePen(painter, mColor, mPenStyle, mPenWidth), painter->opacity());
	painter->drawPolyline(mLine);
	painter->restore();

	painter->save();
	painter->translate(mLine[0]);
	painter->drawText(QPointF(10, 20), mFromMult);
	painter->rotate(90 - lineAngle(QLineF(mLine[1], mLine[0])));
	setEdgePainter(painter, edgePen(painter, mColor, style, 3), painter->opacity());
	drawStartArrow(painter);
	painter->restore();

	painter->save();
	painter->translate(mLine[mLine.size() - 1]);
	painter->drawText(QPointF(10, 20), mToMult);
	painter->rotate(90 - lineAngle(QLineF(mLine[mLine.size() - 2], mLine[mLine.size() - 1])));
	setEdgePainter(painter, edgePen(painter, mColor, style, 3), painter->opacity());
	drawEndArrow(painter);
	painter->restore();

	drawPorts(painter, option);
}

void EdgeElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget*)
{
	paintSavedEdge(painter);
	paintChangedEdge(painter, option);
}

QPainterPath EdgeElement::shape() const
{
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);

	QPainterPathStroker ps;
	ps.setWidth(kvadratik - 2.5);

	path.addPolygon(mLine);
	path = ps.createStroke(path);

	foreach (QPointF const point, mLine) {
		path.addRect(getPortRect(point).adjusted(1, 1, -1, -1));
	}

	return path;
}

QRectF EdgeElement::getPortRect(QPointF const &point)
{
	return QRectF(point - QPointF(kvadratik - 5, kvadratik - 5), QSizeF(kvadratik, kvadratik));
}

int EdgeElement::getPoint(const QPointF &location)
{
	for (int i = 0; i < mLine.size(); ++i)
		if (getPortRect(mLine[i]).contains(location))
			return i;

	return 	noPort;
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

	if (mTitles.count() == 1) {
		ElementTitle *title = mTitles[0];
		qreal x = (mLine[maxIdx].x() + mLine[maxIdx + 1].x()) / 2;
		qreal y = (mLine[maxIdx].y() + mLine[maxIdx + 1].y()) / 2;
		x -= title->boundingRect().width() / 2;
		y -= title->boundingRect().height() / 2;
		title->setPos(x, y);

		QLineF longest(mLine[maxIdx], mLine[mLongPart + 1]);

		if (mChaoticEdition) {
			title->setRotation(title->rotation() + (-lineAngle(longest)));
		}
	}
}

void EdgeElement::connectToPort()
{
	mMoving = true;

	NodeElement *newSrc = getNodeAt(mLine.first(), true);
	NodeElement *newDst = getNodeAt(mLine.last(), false);

	mIsLoop = ((newSrc == newDst) && newSrc);

	if (newSrc == newDst && newSrc && mLine.size() <= 3) {
		if (!mLastLine.isEmpty()) {
			if (mLastLine.size() > 2 && !mLastLineIsLoop) {
				mIsLoop = false;
			}
			mLine = mLastLine;
			mMoving = false;
			return;
		} else {
			// FIXME: need createLoopEdge()'s implementation. linker's edges, for example, transforms to point now (see #602)
			mIsLoop = true;
			connectLoopEdge(newSrc);
			createLoopEdge();
			return;
		}
	}

	if (mIsLoop) {
		connectLoopEdge(newSrc);
		return;
	}

	setPos(pos() + mLine.first());
	mLine.translate(-mLine.first());

	mPortFrom = newSrc ? newSrc->portId(mapToItem(newSrc, mLine.first())) : -1.0;
	mPortTo = newDst ? newDst->portId(mapToItem(newDst, mLine.last())) : -1.0;

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

	mGraphicalAssistApi->setFrom(id(), (mSrc ? mSrc->id() : Id::rootId()));
	mGraphicalAssistApi->setFromPort(id(), mPortFrom);

	if (mPortTo >= -epsilon) {
		mDst = newDst;
		mDst->addEdge(this);
	}

	mGraphicalAssistApi->setTo(id(), (mDst ? mDst->id() : Id::rootId()));
	mGraphicalAssistApi->setToPort(id(), mPortTo);

	mLogicalAssistApi->setFrom(logicalId(), (mSrc ? mSrc->logicalId() : Id::rootId()));
	mLogicalAssistApi->setTo(logicalId(), (mDst ? mDst->logicalId() : Id::rootId()));

	adjustLink();
	arrangeSrcAndDst();

	mGraphicalAssistApi->setPosition(id(), pos());
	saveConfiguration(QPointF());

	mMoving = false;

	if (newSrc && newDst) {
		highlight(mPenColor);
	} else {
		highlight(Qt::red);
	}
}

void EdgeElement::connectLoopEdge(NodeElement *newMaster)
{
	setPos(pos() + mLine.first());
	mLine.translate(-mLine.first());

	mPortFrom = newMaster ? newMaster->portId(mapToItem(newMaster, mLine.first())) : -1.0;
	mPortTo = mPortFrom;

	if (mPortFrom >= -epsilon) {
		newMaster->delEdge(this);
		mSrc = newMaster;
		mDst = newMaster;
		mSrc->addEdge(this);
	}

	mGraphicalAssistApi->setFrom(id(), (mSrc ? mSrc->id() : Id::rootId()));
	mGraphicalAssistApi->setFromPort(id(), mPortFrom);

	mGraphicalAssistApi->setTo(id(), (mDst ? mDst->id() : Id::rootId()));
	mGraphicalAssistApi->setToPort(id(), mPortTo);

	mLogicalAssistApi->setFrom(logicalId(), (mSrc ? mSrc->logicalId() : Id::rootId()));
	mLogicalAssistApi->setTo(logicalId(), (mDst ? mDst->logicalId() : Id::rootId()));

	mGraphicalAssistApi->setPosition(id(), pos());
	saveConfiguration(QPointF());

	mMoving = false;

	highlight(mPenColor);
}

void EdgeElement::createLoopEdge() // nice implementation makes sense after #602 fixed!
{
	int direct = defineDirection(true);
	mLine.remove(1, mLine.size() - 1);
	bool isVertical = direct & 1;
	if (direct > 0) {
		direct = 1;
	} else {
		direct = -1;
	}
	if (SettingsManager::value("SquareLine").toBool()) {
		// creating square
		qreal newY = direct * 3 * kvadratik + mLine.first().y();
		qreal newX = direct * 3 * kvadratik + mLine.first().x();
		mLine.insert(1, QPointF(mLine.first().x(), newY));
		mLine.insert(2, QPointF(newX, newY));
		mLine.insert(3, QPointF(newX, mLine.first().y()));
		mLine.insert(4, QPointF(mLine.first()));
	} else {
		// creating equilateral triangle
		if (isVertical) {
			qreal newY = direct * 3 * kvadratik + mLine.first().y();
			mLine.insert(1, QPointF(mLine.first().x() - 1.5 * kvadratik, newY));
			mLine.insert(2, QPointF(mLine.first().x() + 1.5 * kvadratik, newY));
			mLine.insert(3, QPointF(mLine.first()));
		} else {
			qreal newX = direct * 3 * kvadratik + mLine.first().x();
			mLine.insert(1, QPointF(newX, mLine.first().y() + 1.5 * kvadratik));
			mLine.insert(2, QPointF(newX, mLine.first().y() - 1.5 * kvadratik));
			mLine.insert(3, QPointF(mLine.first()));
		}
	}
	mIsLoop = true;
}

bool EdgeElement::initPossibleEdges()
{
	if (!possibleEdges.isEmpty())
		return true;
	QString editor = id().editor();
	//TODO: do a code generation for diagrams
	QString diagram = id().diagram();
	QList<StringPossibleEdge> stringPossibleEdges = mGraphicalAssistApi->editorManagerInterface().possibleEdges(editor, id().element());
	foreach (StringPossibleEdge pEdge, stringPossibleEdges) {
		QPair<qReal::Id, qReal::Id> nodes(Id(editor, diagram, pEdge.first.first),
		Id(editor, diagram, pEdge.first.second));
		QPair<bool, qReal::Id> edge(pEdge.second.first, Id(editor, diagram, pEdge.second.second));
		PossibleEdge possibleEdge(nodes, edge);
		possibleEdges.push_back(possibleEdge);
	}

	return (!possibleEdges.isEmpty());
}

void EdgeElement::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::RightButton) {
		event->accept();
		if (mDragPoint == noDrag) {
			return;
		}
	}

	mReshapeCommand = new commands::ReshapeEdgeCommand(this);
	mReshapeCommand->startTracking();

	if (!SettingsManager::value("SquareLine").toBool() && (event->modifiers() & Qt::AltModifier)
		&& (getPoint(event->pos()) != noPort) && (event->button() == Qt::LeftButton) && delPointActionIsPossible(event->pos()))
	{
		delPointHandler(event->pos());
		return;
	}
	if (event->button() == Qt::RightButton && mLeftButtonIsPressed && event->buttons() & Qt::LeftButton) {
		prepareGeometryChange();
		mLine = mSavedLineForChanges;
		mDragPoint = noDrag;
		mLeftButtonIsPressed = false;
		return;
	}
	if (event->button() == Qt::LeftButton && !(event->modifiers())) {
		mLeftButtonIsPressed = true;
	}

	mDragPoint = getPoint(event->pos());

	if ((mSrc && mDst && mSrc->isSelected() && mDst->isSelected() && isSelected())
		|| (mSrc && !mDst && mSrc->isSelected() && isSelected())
		|| (mDst && !mSrc && mDst->isSelected() && isSelected()))
	{
		mLeftButtonIsPressed = false;
		mDragPoint = noPort;
		Element::mousePressEvent(event);
	}
	else if (mDragPoint == noPort) {
		Element::mousePressEvent(event);
		if ((mSrc) || (mDst)) {
			if ((event->button() != Qt::RightButton) && !event->modifiers()) {
				mSavedLineForChanges = mLine;
				addPointHandler(event->pos());
			}
		}
	} else if (mDragPoint != noPort && (event->button() != Qt::RightButton)) {
		mLastLineIsLoop = mIsLoop;
		mLastLine = mLine;	// saving info in case we need to rollback (see #4)
		mSavedLineForChanges = mLine;
	}
}

void EdgeElement::addClosestPointHandler(QPointF const &pos)
{
	QPainterPath path;
	QPainterPathStroker ps;
	ps.setWidth(kvadratik);
	int start = -1;
	for (int i = 0; i < mLine.size() - 1; ++i) {
		path.moveTo(mLine[i]);
		path.lineTo(mLine[i + 1]);
		if (ps.createStroke(path).contains(pos)) {
			start = i;
			break;
		}
	}

	if (start == -1) {
		return;
	}

	qreal x = mLine[start + 1].x() - mLine[start].x();
	qreal y = mLine[start + 1].y() - mLine[start].y();
	qreal x1 = pos.x() - mLine[start].x();
	qreal y1 = pos.y() - mLine[start].y();
	// normalize
	x = x / lengthOfSegment(mLine[start], mLine[start + 1]);
	y = y / lengthOfSegment(mLine[start], mLine[start + 1]);

	qreal scalar = x * x1 + y * y1;
	qreal shiftX = scalar * x;
	qreal shiftY = scalar * y;

	QPointF basePoint(mLine[start].x() + shiftX, mLine[start].y() + shiftY);

	mLine.insert(start + 1, basePoint);

	updateLongestPart();
	update();
}

bool EdgeElement::isDividable()
{
	return mElementImpl->isDividable();
}

void EdgeElement::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::RightButton) {
		event->accept();
		return;
	}
	if (mSrc) {
		mSrc->setPortsVisible(true);
	}
	if (mDst) {
		mDst->setPortsVisible(true);
	}
	if (!mLeftButtonIsPressed && mDragPoint == noDrag) {
		return;
	}

	if (mDragPoint == noPort) {
		Element::mouseMoveEvent(event);
	} else {
		if (mDragPoint > mLine.size() - 1) {
			mDragPoint = overPointMax;
			prepareGeometryChange();
			mLine = mSavedLineForChanges;
			return;
		}
		if (mDragPoint < 0) {
			return;
		}
		prepareGeometryChange();

		mLine[mDragPoint] = event->pos();

		if (SettingsManager::value("SquareLine").toBool()) {
			squarizeAndAdjustHandler();
		} else {
			if (SettingsManager::value("ActivateGrid").toBool()) {
				int const indexGrid = SettingsManager::value("IndexGrid").toInt();
				mLine[mDragPoint] = alignedPoint(event->pos(), indexGrid);
			}
		}

		updateLongestPart();
	}
}

void EdgeElement::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	mDragPoint = noPort;
	scene()->update();
	if (event->button() == Qt::RightButton) {
		event->accept();
		return;
	}
	if (mLeftButtonIsPressed && event->button() == Qt::LeftButton) {
		mLeftButtonIsPressed = false;
	}

	Element::mouseReleaseEvent(event);

	if (mSrc) {
		mSrc->setPortsVisible(false);
	}

	if (mDst) {
		mDst->setPortsVisible(false);
	}

	connectToPort();

	delCloseLinePoints();

	adjustNeighborLinks();
	arrangeSrcAndDst();

	prepareGeometryChange();
	adjustNeighborLinks();

	setGraphicApiPos();
	saveConfiguration(QPointF());

	mReshapeCommand->stopTracking();
	mController->execute(mReshapeCommand);
	// Undo stack took ownership
	mReshapeCommand = NULL;
}

qreal EdgeElement::lengthOfSegment(QPointF const &pos1, QPointF const &pos2) const
{
	qreal len = sqrt(((pos1.x() - pos2.x()) * (pos1.x() - pos2.x())) + ((pos1.y() - pos2.y()) * (pos1.y() - pos2.y())));
	return len;
}

void EdgeElement::delClosePoints()
{
	int const rad = kvadratik * 2;
	for (int i = 0; i < mLine.size() - 1; i++) {
		if (lengthOfSegment(mLine[i], mLine[i + 1]) < rad) {
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
			return innermostChild(items, e);
		}
	}
	return NULL;
}

NodeElement *EdgeElement::innermostChild(QList<QGraphicsItem *> const &items, NodeElement *element) const
{
	foreach (NodeElement *child, element->childNodes()) {
		if (items.contains(child)) {
			return innermostChild(items, child);
		}
	}
	return element;
}

QList<ContextMenuAction*> EdgeElement::contextMenuActions(const QPointF &pos)
{
	QList<ContextMenuAction*> result;
	if (delPointActionIsPossible(pos)) {
		result.push_back(&mDelPointAction);
	}
	if (addPointActionIsPossible(pos)) {
		result.push_back(&mAddPointAction);
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
	if (mIsLoop && mLine.size() <= 4) {
		return false;
	}
	if (!SettingsManager::value("SquareLine").toBool()) {
		int pointIndex = getPoint(pos);
		// it is understood that there is a point and its index is equal to the index of the first and last (end) points
		return (pointIndex != noPort && pointIndex != mLine.count() - 1 && pointIndex);
	} else {
		return false;
	}
}

bool EdgeElement::addPointActionIsPossible(const QPointF &pos)
{
	int pointIndex = getPoint(pos);
	return !(mSrc || mDst) && (pointIndex == noPort); // point's adding is unneeded now besides alone link's event
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
	if (mIsLoop) {
		return false;
	}
	return !(mLine.size() == 2) && !(mSrc == mDst) && !(SettingsManager::value("SquareLine").toBool() && mLine.size() == 3);
}

bool EdgeElement::reverseActionIsPossible()
{
	return true;
}

QList<PossibleEdge> EdgeElement::getPossibleEdges()
{
	return possibleEdges;
}

void EdgeElement::delPointHandler(QPointF const &pos)
{
	int pointIndex = getPoint(pos);
	// it is understood that there is a point and its index is equal to the index of the first and last (end) points
	if (pointIndex != noPort && pointIndex != mLine.count() - 1 && pointIndex) {
		prepareGeometryChange();
		mLine.remove(pointIndex);
	}
}

void EdgeElement::arrangeAndAdjustHandler(QPointF const &pos)
{
	Q_UNUSED(pos);
	adjustNeighborLinks();
	arrangeSrcAndDst();
	mMoving = true;
	mGraphicalAssistApi->setPosition(id(), this->pos());
	mMoving = false;
	update();
	saveConfiguration(QPointF());
}

void EdgeElement::setGraphicApiPos()
{
	mMoving = true;
	mGraphicalAssistApi->setPosition(id(), this->pos());
	mMoving = false;
}

void EdgeElement::addPointHandler(QPointF const &pos)
{
	QPainterPath path;
	QPainterPathStroker ps;
	ps.setWidth(kvadratik); // expressly this width > width of shape() path (see #569 and #165)
	for (int i = 0; i < mLine.size() - 1; ++i) {
		path.moveTo(mLine[i]);
		path.lineTo(mLine[i + 1]);
		if (ps.createStroke(path).contains(pos)) {
			mLine.insert(i + 1, pos);
			updateLongestPart();
			mDragPoint = i + 1;
			update();
			break;
		}
	}
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
		mDragPoint = 0;
	}

	if (mLine.endsWith(pos.toPoint())) {
		mLine.insert(mLine.size() - 1, pos);
		mDragPoint = mLine.size() - 1;
	}
}

void EdgeElement::squarizeAndAdjustHandler()
{
	squarize();
	deleteLoops();
	delCloseLinePoints();
	arrangeSrcAndDst();
	squarize();
	updateLongestPart();
}

void EdgeElement::squarize()
{
	mLine = QPolygonF() << mLine.first() << mLine.last();

	// we don't need to correct the already straight line
	if (mLine.first().x() == mLine.last().x() || mLine.first().y() == mLine.last().y()) {
		return;
	}

	int type = defineType();

	switch (type) {
	case Vertical:
		verticalSquareLine();
		break;
	case Horizontal:
		horizontalSquareLine();
		break;
	case VerticalTurn:
		verticalTurningSquareLine();
		break;
	case HorizontalTurn:
		horizontalTurningSquareLine();
		break;
	default:
		qDebug() << "incorrect link type";
	}
}

int EdgeElement::defineType()
{
	if (!(mSrc && mDst)) {
		return HorizontalTurn;
	}

	// defining type of square link by looking at sides of nodes which the link is connected to
	int startSide = defineSide(mPortFrom);
	int endSide = defineSide(mPortTo);

	if (startSide == Top || startSide == Bottom) {
		if (endSide == Top || endSide == Bottom) {
			return Vertical;
		} else {
			return VerticalTurn;
		}
	} else if (endSide == Left || endSide == Right) {
		return Horizontal;
	} else {
		return HorizontalTurn;
	}
}

int EdgeElement::defineSide(qreal port)
{
	NodeElement *node = (port == mPortFrom) ? mSrc : mDst;
	QPointF pos = node->portPos(port);
	QRectF bounds = node->boundingRect();

	// divide bounding rectangle with it's diagonals, then determine in which part the port lies
	bool top = pos.y() < bounds.height() / bounds.width() * pos.x();
	bool left = pos.y() / bounds.height() + pos.x() / bounds.width() < 1;

	if (top) {
		if (left) {
			return Top;
		} else {
			return Right;
		}
	}
	if (left) {
		return Left;
	} else {
		return Bottom;
	}
}

void EdgeElement::horizontalSquareLine()
{
	QPointF insertPoint1 = mLine.first();
	QPointF insertPoint2 = mLine.last();

	insertPoint1.setX((insertPoint1.x() + insertPoint2.x()) / 2);
	insertPoint2.setX(insertPoint1.x());

	mLine.insert(1, insertPoint1);
	mLine.insert(2, insertPoint2);
}

void EdgeElement::verticalSquareLine()
{
	QPointF insertPoint1 = mLine.first();
	QPointF insertPoint2 = mLine.last();

	insertPoint1.setY((insertPoint1.y() + insertPoint2.y()) / 2);
	insertPoint2.setY(insertPoint1.y());

	mLine.insert(1, insertPoint1);
	mLine.insert(2, insertPoint2);
}

void EdgeElement::horizontalTurningSquareLine()
{
	QPointF insertPoint = mLine.first();
	insertPoint.setX(mLine.last().x());
	mLine.insert(1, insertPoint);
}

void EdgeElement::verticalTurningSquareLine()
{
	QPointF insertPoint = mLine.first();
	insertPoint.setY(mLine.last().y());
	mLine.insert(1, insertPoint);
}

void EdgeElement::minimizeHandler(const QPointF &pos)
{
	Q_UNUSED(pos);
	QPolygonF newMLine;
	newMLine << mLine.first() << mLine.last();
	prepareGeometryChange();
	mLine = newMLine;
	updateLongestPart();
	adjustNeighborLinks();
	arrangeSrcAndDst();
	saveConfiguration(QPointF());
}

void EdgeElement::adjustLink(bool isDragging)
{
	if (!isDragging) {
		if (mSrc) {
			prepareGeometryChange();
			mLine.first() = mapFromItem(mSrc, mSrc->portPos(mPortFrom));
		}
		if (mDst) {
			prepareGeometryChange();
			mLine.last() = mapFromItem(mDst, mDst->portPos(mPortTo));
		}
		delCloseLinePoints();
		deleteLoops();
		updateLongestPart();
	} else {
		if (isSelected()) {
			if (mSrc && !mSrc->isSelected()) {
				prepareGeometryChange();
				mLine.first() = mapFromItem(mSrc, mSrc->portPos(mPortFrom));
			}
			if (mDst && !mDst->isSelected()) {
				prepareGeometryChange();
				mLine.last() = mapFromItem(mDst, mDst->portPos(mPortTo));
			}
			updateLongestPart();
		} else {
			if (mSrc && mSrc->isSelected()) {
				prepareGeometryChange();
				mLine.first() = mapFromItem(mSrc, mSrc->portPos(mPortFrom));
			}
			if (mDst && mDst->isSelected()) {
				prepareGeometryChange();
				mLine.last() = mapFromItem(mDst, mDst->portPos(mPortTo));
			}
			updateLongestPart();
		}
	}
	if (SettingsManager::value("SquareLine").toBool()) {
		squarizeAndAdjustHandler();
	}
}

bool EdgeElement::shouldReconnect() const
{
	if (mSrc) {
		qreal newFrom = mSrc->portId(mapToItem(mSrc, mLine[1]));
		if (floor(newFrom) != floor(mPortFrom))
			return true;
	}
	if (mDst) {
		qreal newTo = mDst->portId(mapToItem(mDst, mLine[mLine.count() - 2]));
		if (floor(newTo) != floor(mPortTo))
			return true;
	}
	return false;
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

qreal EdgeElement::portIdOn(NodeElement const *node) const
{
	if (node == mSrc)
		return mPortFrom;
	if (node == mDst)
		return mPortTo;
	return -1;
}

QPointF EdgeElement::nextFrom(NodeElement const *node) const
{
	if (node == mSrc)
		return mapToItem(mSrc, mLine[1]);
	if (node == mDst)
		return mapToItem(mDst, mLine[mLine.count() - 2]);
	return QPointF();
}

QPointF EdgeElement::connectionPoint(NodeElement const *node) const
{
	if (node == mSrc)
		return mapToItem(mSrc, mLine[0]);
	if (node == mDst)
		return mapToItem(mDst, mLine[mLine.count() - 1]);
	return QPointF();

}

NodeElement* EdgeElement::otherSide(NodeElement const *node) const
{
	if (node == mSrc)
		return mDst;
	if (node == mDst)
		return mSrc;
	return 0;
}

bool EdgeElement::reconnectToNearestPorts(bool reconnectSrc, bool reconnectDst, bool jumpsOnly)
{
	bool reconnectedSrc = false;
	bool reconnectedDst = false;
	if (mSrc && reconnectSrc) {
		qreal newFrom = mSrc->portId(mapToItem(mSrc, mLine[1]));
		reconnectedSrc = (NodeElement::portNumber(newFrom) != NodeElement::portNumber(mPortFrom));
		if (!jumpsOnly || reconnectedSrc) {
			mPortFrom = newFrom;
			mModelUpdateIsCalled = true;
			mGraphicalAssistApi->setFromPort(id(), mPortFrom);
		}

	}
	if (mDst && reconnectDst) {
		qreal newTo = mDst->portId(mapToItem(mDst, mLine[mLine.count() - 2]));
		reconnectedDst = (NodeElement::portNumber(newTo) != NodeElement::portNumber(mPortTo));
		if (!jumpsOnly || reconnectedDst) {
			mPortTo = newTo;
			mModelUpdateIsCalled = true;
			mGraphicalAssistApi->setToPort(id(), mPortTo);
		}
	}

	return reconnectedSrc || reconnectedDst;
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

	setPos(mGraphicalAssistApi->position(id()));
	QPolygonF newLine = mGraphicalAssistApi->configuration(id());

	if (!newLine.isEmpty()) {
		mLine = newLine;
	}

	qReal::Id idFrom = mGraphicalAssistApi->from(id());
	qReal::Id idTo = mGraphicalAssistApi->to(id());

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

	mPortFrom = mGraphicalAssistApi->fromPort(id());
	mPortTo = mGraphicalAssistApi->toPort(id());

	adjustLink();
	mElementImpl->updateData(this);

	update();
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

void EdgeElement::placeStartTo(QPointF const &place)
{
	mLine[0] = place;
}

void EdgeElement::placeEndTo(QPointF const &place)
{
	prepareGeometryChange();
	mLine[mLine.size() - 1] = place;

	if (SettingsManager::value("SquareLine").toBool()) {
		squarizeAndAdjustHandler();
	}

	mModelUpdateIsCalled = true;
	mGraphicalAssistApi->setPosition(id(), this->pos());

	updateLongestPart();
}

void EdgeElement::moveConnection(NodeElement *node, qreal const portId) {
	if (node == mSrc) {
		mPortFrom = portId;
		mModelUpdateIsCalled = true;
		mGraphicalAssistApi->setFromPort(id(), mPortFrom);
	}
	if (node == mDst) {
		mPortTo = portId;
		mModelUpdateIsCalled = true;
		mGraphicalAssistApi->setToPort(id(), mPortTo);
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

int EdgeElement::defineDirection(bool from)
{
	qreal port = from ? mPortFrom : mPortTo;
	int direct = 0;
	if (port < 2 && port > 1) {
		direct = top;
		if ((!from && (mLine[mLine.size() - 2].y() > mLine.last().y()))
				|| (from && (mLine[1].y() < mLine[0].y()))) {
			direct = topInsideNode;
		}
	}
	if (port < 1 && port > 0) {
		direct = left;
		if ((!from && (mLine[mLine.size() - 2].x() > mLine.last().x()))
				|| (from && (mLine[1].x() < mLine[0].x()))) {
			direct = leftInsideNode;
		}
	}
	if (port < 3 && port > 2) {
		direct = right;
		if ((!from && (mLine[mLine.size() - 2].x() < mLine.last().x()))
				|| (from && (mLine[1].x() > mLine[0].x()))) {
			direct = rightInsideNode;
		}
	}
	if (port < 4 && port > 3) {
		direct = bottom;
		if ((!from && (mLine[mLine.size() - 2].y() < mLine.last().y()))
				|| (from && (mLine[1].y() > mLine[0].y()))) {
			direct = bottomInsideNode;
		}
	}
	return direct;
}

void EdgeElement::redrawing(QPointF const &pos)
{
	Q_UNUSED(pos);
	if (mIsLoop) { // it's row prevention of transform to the point before fix #602
		createLoopEdge();
	}
	adjustNeighborLinks();
	arrangeSrcAndDst();
	prepareGeometryChange();
	adjustLink();
	mMoving = true;
	mGraphicalAssistApi->setPosition(id(), this->pos());
	mMoving = false;
	saveConfiguration(QPointF());
}

void EdgeElement::adjustNeighborLinks() // require a lot of resources for the beauty
{
	if (mSrc) {
		mSrc->adjustLinks();
	}
	if (mDst) {
		mDst->adjustLinks();
	}
	adjustLink();
}

QPointF* EdgeElement::haveIntersection(QPointF const &pos1, QPointF const &pos2, QPointF const &pos3, QPointF const &pos4)
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

	mData.configuration = mGraphicalAssistApi->configuration(mId);
	mData.pos = mGraphicalAssistApi->position(mId);

	return mData;
}

void EdgeElement::deleteFromScene()
{
	if (mSrc) {
		mSrc->delEdge(this);
		mSrc->arrangeLinks();
		mModelUpdateIsCalled = true;
		mGraphicalAssistApi->setFrom(id(), Id::rootId());
		mLogicalAssistApi->setFrom(logicalId(), Id::rootId());
	}

	if (mDst) {
		mDst->delEdge(this);
		mDst->arrangeLinks();
		mModelUpdateIsCalled = true;
		mGraphicalAssistApi->setTo(id(), Id::rootId());
		mLogicalAssistApi->setTo(logicalId(), Id::rootId());
	}
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
				if ((i != 0) || !((j == mLine.size() - 2) && (lengthOfSegment(mLine.first(), mLine.last()) < (kvadratik * 2))))
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
	// It's very rough prevention of transforming in the point now
	// should be adjusted deleteLoop and delCloseLinePoints to a good drawing considering isLoop (after fix #602)
	// It's need to drawing without point's links and "QTransform::rotate with NaN called" accordingly
	if (mIsLoop && ((SettingsManager::value("SquareLine").toBool() && mLine.size() <= 6) || mLine.size() <= 5)) {
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
	saveConfiguration(QPointF());
}

void EdgeElement::saveConfiguration(QPointF const &pos)
{
	Q_UNUSED(pos);
	mModelUpdateIsCalled = true;
	mGraphicalAssistApi->setConfiguration(id(), mLine.toPolygon());
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

	mPortFrom = newSrc ? newSrc->portId(mapToItem(newSrc, mLine.first())) : -1.0;
	mPortTo = newDst ? newDst->portId(mapToItem(newDst, mLine.last())) : -1.0;

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
	mGraphicalAssistApi->setFrom(id(), (mSrc ? mSrc->id() : Id::rootId()));
	mGraphicalAssistApi->setFromPort(id(), mPortFrom);
	mGraphicalAssistApi->setTo(id(), (mDst ? mDst->id() : Id::rootId()));
	mGraphicalAssistApi->setToPort(id(), mPortTo);
	mLogicalAssistApi->setFrom(logicalId(), (mSrc ? mSrc->logicalId() : Id::rootId()));
	mLogicalAssistApi->setTo(logicalId(), (mDst ? mDst->logicalId() : Id::rootId()));

	mMoving = false;
}

QVariant EdgeElement::itemChange(GraphicsItemChange change, QVariant const &value)
{
	switch (change) {
	case ItemPositionHasChanged:
		if (isSelected() && (mSrc || mDst) && !mMoving) {
			if (mSrc && !mSrc->isSelected()) {
				prepareGeometryChange();
				mLine.first() = mapFromItem(mSrc, mSrc->portPos(mPortFrom));
				updateLongestPart();
			}
			if (mDst && !mDst->isSelected()) {
				prepareGeometryChange();
				mLine.last() = mapFromItem(mDst, mDst->portPos(mPortTo));
				updateLongestPart();
			}
		}
		if (SettingsManager::value("SquareLine").toBool()) {
			squarizeAndAdjustHandler();
		}
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
	if (node) {
		mDst->addEdge(this);
	}
}

void EdgeElement::tuneForLinker()
{
	mMoving = true;
	setPos(pos() + mLine.first());
	mLine.translate(-mLine.first());
	mPortFrom = mSrc ? mSrc->portId(mapToItem(mSrc, mLine.first())) : -1.0;
	mGraphicalAssistApi->setFromPort(id(), mPortFrom);
	adjustNeighborLinks();
	arrangeSrcAndDst();
	mGraphicalAssistApi->setPosition(id(), pos());
	mMoving = false;
}

bool EdgeElement::isLoop()
{
	return mIsLoop;
}

void EdgeElement::alignToGrid()
{
	if (mLine.size() >= 3 && !SettingsManager::value("SquareLine").toBool()) {
		int const indexGrid = SettingsManager::value("IndexGrid").toInt();

		prepareGeometryChange();

		for (int i = 1; i < mLine.size() - 1; ++i) {
			mLine[i] = alignedPoint(mLine[i], indexGrid);
		}

		update();
		updateLongestPart();
	}
}

QPointF EdgeElement::alignedPoint(QPointF const &point, int const indexGrid) const
{
	QPointF result = mapToScene(point);

	int const coefX = static_cast<int>(result.x()) / indexGrid;
	int const coefY = static_cast<int>(result.y()) / indexGrid;

	result = QPointF(SceneGridHandler::alignedCoordinate(result.x(), coefX, indexGrid)
			, SceneGridHandler::alignedCoordinate(result.y(), coefY, indexGrid));

	return mapFromScene(result);
}
