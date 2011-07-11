/** @file edgeelement.cpp
 * 	@brief class for an edge on a diagram
 * */
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtGui/QStyle>
#include <QtGui/QTextDocument>
#include <QtGui/QMenu>
#include <QDebug>
#include <math.h>

#include "edgeElement.h"
#include "nodeElement.h"
#include "../view/editorviewscene.h"
#include "../pluginInterface/editorInterface.h"

using namespace qReal;

const double pi = 3.14159265358979;

/** @brief indicator of edges' movement */
// static bool moving = false;

EdgeElement::EdgeElement(ElementImpl *impl)
	: mPenStyle(Qt::SolidLine), mStartArrowStyle(NO_ARROW), mEndArrowStyle(NO_ARROW)
	, mSrc(NULL), mDst(NULL)
	, mPortFrom(0), mPortTo(0)
	, mDragPoint(-1), mLongPart(0), mBeginning(NULL), mEnding(NULL)
	, mAddPointAction(tr("Add point"), this)
	, mDelPointAction(tr("Delete point"), this)
	, mSquarizeAction(tr("Squarize"), this)
	, mMinimizeAction(tr("Minimize"), this)
	, mElementImpl(impl)
	, mLastDragPoint(-1)
{
	mPenStyle = mElementImpl->getPenStyle();
	setZValue(100);
	setFlag(ItemIsMovable, true);
	// FIXME: draws strangely...
	setFlag(ItemClipsToShape, false);
	setFlag(ItemClipsChildrenToShape, false);

	mLine << QPointF(0, 0) << QPointF(200, 60);

	setAcceptHoverEvents(true);

	connect(&mAddPointAction, SIGNAL(triggered(QPointF const &)), SLOT(addPointHandler(QPointF const &)));
	connect(&mDelPointAction, SIGNAL(triggered(QPointF const &)), SLOT(delPointHandler(QPointF const &)));
	connect(&mSquarizeAction, SIGNAL(triggered(QPointF const &)), SLOT(squarizeHandler(QPointF const &)));
	connect(&mMinimizeAction, SIGNAL(triggered(QPointF const &)), SLOT(minimizeHandler(QPointF const &)));

	mChaoticEdition = SettingsManager::value("ChaoticEdition", false).toBool();

	ElementTitleFactory factory;

	QList<ElementTitleInterface*> titles;
	mElementImpl->init(factory, titles);
	foreach (ElementTitleInterface *titleIface, titles){
		ElementTitle *title = dynamic_cast<ElementTitle*>(titleIface);
		if (!title)
			continue;
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
	pen.setWidth(11);
	pen.setColor(color);
	painter->setPen(pen);
	painter->drawLine(p1, p2);

	color.setNamedColor("#465945");
	pen.setWidth(3);
	pen.setColor(color);
	painter->setPen(pen);
	painter->drawLine(p1, p2);
}

void EdgeElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget*)
{
	painter->save();
	QPen pen = painter->pen();
	pen.setColor(mColor);
	pen.setStyle(mPenStyle);
	pen.setWidth(3);  // TODO: Move line width to a configuration parameter
	painter->setPen(pen);
	painter->drawPolyline(mLine);
	painter->restore();

	painter->save();
	painter->translate(mLine[0]);
	painter->drawText(QPointF(10, 20), mFromMult);
	painter->rotate(90 - lineAngle(QLineF(mLine[1], mLine[0])));
	pen = painter->pen();
	pen.setColor(mColor);
	pen.setWidth(3);
	painter->setPen(pen);
	drawStartArrow(painter);
	painter->restore();

	painter->save();
	painter->translate(mLine[mLine.size() - 1]);
	painter->drawText(QPointF(10, 20), mToMult);
	painter->rotate(90 - lineAngle(QLineF(mLine[mLine.size() - 2], mLine[mLine.size() - 1])));
	pen = painter->pen();
	pen.setColor(mColor);
	pen.setWidth(3);
	painter->setPen(pen);
	drawEndArrow(painter);
	painter->restore();

	if (option->state & (QStyle::State_Selected | QStyle::State_MouseOver)) {
		painter->setBrush(Qt::SolidPattern);
		foreach (QPointF const point, mLine) {

			painter->save();
			painter->translate(point);

			if (mChaoticEdition)
				drawChaosStar(painter);
			else
				drawPort(painter);

			painter->restore();
		}
	}
}

QPainterPath EdgeElement::shape() const
{
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);

	QPainterPathStroker ps;
		ps.setWidth(kvadratik - 5);

	path.addPolygon(mLine);
	path = ps.createStroke(path);

	foreach (QPointF const point, mLine) {
		path.addRect(getPortRect(point));
	}

	return path;
}

QRectF EdgeElement::getPortRect(QPointF const &point)
{
		return QRectF(point - QPointF(kvadratik - 5, kvadratik - 5), QSizeF(kvadratik * 2, kvadratik * 2));
}

int EdgeElement::getPoint(const QPointF &location)
{
	for (int i = 0; i < mLine.size(); ++i)
		if (getPortRect(mLine[i]).contains(location))
			return i;

	return -1;
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

		if (mChaoticEdition)
			title->rotate(-lineAngle(longest));
	}
}

void EdgeElement::connectToPort()
{
	setPos(pos() + mLine.first());
	mLine.translate(-mLine.first());

	mMoving = true;

	// Now we check whether start or end have been connected
	NodeElement *newSrc = getNodeAt(mLine.first());
	NodeElement *newDst = getNodeAt(mLine.last());

	// fix for #4. otherwise we have 2 annoying situations when
	// connecting both ends of an edge to the same node:
	// if mLine.size() is 2, then an edge will become a point
	// if mLine.size() is 3, then two parts of an edge will overlap
	if (newSrc == newDst && newSrc && mLine.size() <= 3) {
		if (!mLastLine.isEmpty())
			mLine = mLastLine;

		mGraphicalAssistApi->setPosition(id(), mLastPos);
		mGraphicalAssistApi->setConfiguration(id(), mLine.toPolygon());

		mMoving = false;
		updateData();

		return;
	}

	mPortFrom = newSrc ? newSrc->getPortId(mapToItem(newSrc, mLine.first())) : -1.0;

	if (mSrc) {
		mSrc->delEdge(this);
		mSrc = 0;
	}

	if (mPortFrom >= 0.0) {
		mSrc = newSrc;
		mSrc->addEdge(this);
	}

	mLogicalAssistApi->setFrom(logicalId(), (mSrc ? mSrc->logicalId() : Id::rootId()));
	mGraphicalAssistApi->setFrom(id(), (mSrc ? mSrc->id() : Id::rootId()));
	mGraphicalAssistApi->setFromPort(id(), mPortFrom);

	mPortTo = newDst ? newDst->getPortId(mapToItem(newDst, mLine.last())) : -1.0;

	if (mDst) {
		mDst->delEdge(this);
		mDst = 0;
	}

	if (mPortTo >= 0.0) {
		mDst = newDst;
		mDst->addEdge(this);
	}

	mLogicalAssistApi->setTo(logicalId(), (mDst ? mDst->logicalId() : Id::rootId()));
	mGraphicalAssistApi->setTo(id(), (mDst ? mDst->id() : Id::rootId()));
	mGraphicalAssistApi->setToPort(id(), mPortTo);

	setFlag(ItemIsMovable, !(mDst || mSrc));

	mGraphicalAssistApi->setPosition(id(), pos());
	mGraphicalAssistApi->setConfiguration(id(), mLine.toPolygon());

	mMoving = false;
	if (SettingsManager::value("SquareLine", true).toBool())
		squarizeHandler(QPointF());
	adjustLink();
	arrangeSrcAndDst();
	update();

}

bool EdgeElement::initPossibleEdges()
{
	if (!possibleEdges.isEmpty())
		return true;
	QString editor = id().editor();
	//TODO: do a code generation for diagrams
	QString diagram = id().diagram();
	EditorInterface * editorInterface = mGraphicalAssistApi->editorManager().editorInterface(editor);
	QList<StringPossibleEdge> stringPossibleEdges = editorInterface->getPossibleEdges(id().element());
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
	mDragPoint = -1;
	mDragPoint = getPoint(event->pos());

	if (mDragPoint == -1) {
		Element::mousePressEvent(event);
		if ((mSrc != NULL) || (mDst != NULL))
			if (event->buttons() != Qt::RightButton)
				addPointHandler(event->pos());
	} else {
		// saving info in case we need to rollback (see #4)
		mLastDragPoint = mDragPoint;
		mLastPos = pos();
		mLastLine = mLine;
	}
}

void EdgeElement::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	NodeElement *new_src = getNodeAt(mLine.first());
	NodeElement *new_dst = getNodeAt(mLine.back());

	if (mBeginning) {
		if (mBeginning != new_src) {
			mBeginning->setPortsVisible(false);
		}
	}

	if (mEnding) {
		if (mEnding != new_dst) {
			mEnding->setPortsVisible(false);
		}
	}

	mBeginning = new_src;
	mEnding = new_dst;

	if (mBeginning)
		mBeginning->setPortsVisible(true);

	if (mEnding)
		mEnding->setPortsVisible(true);

	if (mDragPoint == -1) {
		Element::mouseMoveEvent(event);
	} else {
		prepareGeometryChange();
		mLine[mDragPoint] = event->pos();
		updateLongestPart();
	}
}

void EdgeElement::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	bool deleteCurrentPoint = false;

	if  (mLine.size() >= 3) {

		if ((mDragPoint > 0) && (mDragPoint < (mLine.size() - 1))) {

			QPainterPath path;
			QPainterPathStroker neighbourhood;
			neighbourhood.setWidth(20);

			path.moveTo(mLine[mDragPoint - 1]);
			path.lineTo(mLine[mDragPoint + 1]);

			if (neighbourhood.createStroke(path).contains(mLine[mDragPoint])) {

				delPointHandler(mLine[mDragPoint]);
				mDragPoint -= 1;
				deleteCurrentPoint = true;
			}
		}

		// try to eliminate unneeded points

		if ((mDragPoint != -1) && (mDragPoint < (mLine.size() - 2))) {
			removeUnneededPoints(mDragPoint);
			if (deleteCurrentPoint)
				mDragPoint += 1;
		}

		if (mDragPoint >= 2)
			removeUnneededPoints(mDragPoint - 2);


	}

	if (mDragPoint == -1)
		Element::mouseReleaseEvent(event);
	else
		mDragPoint = -1;

	if (SettingsManager::value("SquareLine", true).toBool())
		squarizeHandler(QPointF());

	connectToPort();

	if (mBeginning)
		mBeginning->setPortsVisible(false);

	if (mEnding)
		mEnding->setPortsVisible(false);

	// cleanup after moving/resizing
	mBeginning = mEnding = NULL;

	arrangeSrcAndDst();

}

void EdgeElement::removeUnneededPoints(int startingPoint)
{
	if (startingPoint + 2 < mLine.size()) {
		if ((mLine[startingPoint].x() == mLine[startingPoint + 1].x() &&
			mLine[startingPoint].x() == mLine[startingPoint + 2].x()) ||
			(mLine[startingPoint].y() == mLine[startingPoint + 1].y() &&
			mLine[startingPoint].y() == mLine[startingPoint + 2].y()))
		{
			delPointHandler(mLine[startingPoint + 1]);
		}
	}
}

NodeElement *EdgeElement::getNodeAt(QPointF const &position)
{
	QPainterPath circlePath;
	circlePath.addEllipse(mapToScene(position), 12, 12);
	foreach (QGraphicsItem *item, scene()->items(circlePath)) {
		NodeElement *e = dynamic_cast<NodeElement *>(item);
		if (e) {
			return e;
		}
	}
	return NULL;
}

QList<ContextMenuAction*> EdgeElement::contextMenuActions()
{
	QList<ContextMenuAction*> result;
	result.push_back(&mAddPointAction);
	result.push_back(&mDelPointAction);
	result.push_back(&mSquarizeAction);
	result.push_back(&mMinimizeAction);
	return result;
}

QList<PossibleEdge> EdgeElement::getPossibleEdges()
{
	return possibleEdges;
}

void EdgeElement::delPointHandler(QPointF const &pos)
{
	int pointIndex = getPoint(pos);
	if (pointIndex != -1) {
		prepareGeometryChange();
		mLine.remove(pointIndex);
		mLongPart = 0;
		update();
	}
}

void EdgeElement::addPointHandler(QPointF const &pos)
{
	for (int i = 0; i < mLine.size() - 1; ++i) {
		QPainterPath path;
		QPainterPathStroker ps;
				ps.setWidth(kvadratik - 5);

		path.moveTo(mLine[i]);
		path.lineTo(mLine[i + 1]);
		if (ps.createStroke(path).contains(pos)) {
			mLine.insert(i + 1, pos);
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

void EdgeElement::squarizeHandler(QPointF const &pos)
{
	Q_UNUSED(pos);
	prepareGeometryChange();
	int i = 0;
	while (!mLine.endsWith(mLine[i])) {

		//don't make new point between first-second & (last-1)-last. just drag them.
		if (i == 0 && mLine.size() > 2) {
			if (mLine[i + 1].y() == mLine[i + 2].y()) {
				mLine[i + 1].setX(mLine[i].x());
				i += 2;
				continue;
			}
			if (mLine[i + 1].x() == mLine[i + 2].x()) {
				mLine[i + 1].setY(mLine[i].y());
				i += 2;
				continue;
			}
		}

		if (i == mLine.size() - 3) {
			if (mLine[i + 1].y() == mLine[i].y()) {
				mLine[i + 1].setX(mLine[i + 2].x());
				i += 2;
				continue;
			}
			if (mLine[i + 1].x() == mLine[i].x()) {
				mLine[i + 1].setY(mLine[i + 2].y());
				break;
			}
		}
		//if 3 points have same X or Y coordinate delete point between them.
		if (i < mLine.size() - 3) {
			if (mLine[i + 1].x() == mLine[i].x() && mLine[i + 2].x() == mLine[i].x()) {
				mLine.remove(i + 1);
				i++;
				continue;
			}
			if (mLine[i + 1].y() == mLine[i].y() && mLine[i + 2].y() == mLine[i].y()) {
				mLine.remove(i + 1);
				i++;
				continue;
			}
		}


		QPointF insPoint = mLine[i]; //point to insert between 2 others to make right angle

		// we need moar comments!!111
		//dont make new points on line
		if (insPoint.x() == mLine[i + 1].x() || insPoint.y() == mLine[i + 1].y()) {
			i++;
			continue;
		}
		insPoint.setX(mLine[i + 1].x());
		if (mLine[i].x() == insPoint.x() && mLine[i].y() == insPoint.y()) {
			i++;
			continue;
		}

		mLine.insert(i + 1, insPoint); //insert new point to make right angle
		i += 2;
	}

	update();
}

void EdgeElement::minimizeHandler(const QPointF &pos) {
	Q_UNUSED(pos);
	QPolygonF newMLine;

	newMLine << mLine.first() << mLine.last();
	mLine = newMLine;
	mGraphicalAssistApi->setConfiguration(id(), mLine.toPolygon());
}

void EdgeElement::adjustLink()
{
	prepareGeometryChange();
	if (mSrc)
		mLine.first() = mapFromItem(mSrc, mSrc->getPortPos(mPortFrom));
	if (mDst)
		mLine.last() = mapFromItem(mDst, mDst->getPortPos(mPortTo));
	updateLongestPart();
	for (int i = 0; i < mLine.size() - 2; i++)
		removeUnneededPoints(i);
	if (SettingsManager::value("SquareLine", true).toBool())
		squarizeHandler(QPointF());
}

bool EdgeElement::shouldReconnect() const
{
	if (mSrc) {
		qreal newFrom = mSrc->getPortId(mapToItem(mSrc, mLine[1]));
		if (floor(newFrom) != floor(mPortFrom))
			return true;
	}
	if (mDst) {
		qreal newTo = mDst->getPortId(mapToItem(mDst, mLine[mLine.count() - 2]));
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
		qreal newFrom = mSrc->getPortId(mapToItem(mSrc, mLine[1]));
		reconnectedSrc = (NodeElement::portId(newFrom) != NodeElement::portId(mPortFrom));
		if (!jumpsOnly || reconnectedSrc) {
			mPortFrom = newFrom;
			mGraphicalAssistApi->setFromPort(id(), mPortFrom);
		}

	}
	if (mDst && reconnectDst) {
		qreal newTo = mDst->getPortId(mapToItem(mDst, mLine[mLine.count() - 2]));
		reconnectedDst = (NodeElement::portId(newTo) != NodeElement::portId(mPortTo));
		if (!jumpsOnly || reconnectedDst) {
			mPortTo = newTo;
			mGraphicalAssistApi->setToPort(id(), mPortTo);
		}
	}

	return reconnectedSrc || reconnectedDst;
}

void EdgeElement::updateData()
{
	if (mMoving)
		return;

	Element::updateData();

	setPos(mGraphicalAssistApi->position(id()));
	QPolygonF newLine = mGraphicalAssistApi->configuration(id());

	if (!newLine.isEmpty())
		mLine = newLine;

	qReal::Id idFrom = mGraphicalAssistApi->from(id());
	qReal::Id idTo = mGraphicalAssistApi->to(id());

	if (mSrc)
		mSrc->delEdge(this);
	if (mDst)
		mDst->delEdge(this);

	mSrc = dynamic_cast<NodeElement *>(static_cast<EditorViewScene *>(scene())->getElem(idFrom));
	mDst = dynamic_cast<NodeElement *>(static_cast<EditorViewScene *>(scene())->getElem(idTo));

	if (mSrc)
		mSrc->addEdge(this);
	if (mDst)
		mDst->addEdge(this);

	setFlag(ItemIsMovable, !(mDst || mSrc));

	mPortFrom = mGraphicalAssistApi->fromPort(id());
	mPortTo = mGraphicalAssistApi->toPort(id());

	adjustLink();
	if (SettingsManager::value("SquareLine", true).toBool())
		squarizeHandler(QPointF());
	mElementImpl->updateData(this);
	update();
}

void EdgeElement::removeLink(NodeElement const *from)
{
	if (mSrc == from)
		mSrc = NULL;

	if (mDst == from)
		mDst = NULL;
}

void EdgeElement::placeStartTo(QPointF const &place)
{
	mLine[0] = place;
	updateLongestPart();
	if (SettingsManager::value("SquareLine", true).toBool())
		squarizeHandler(QPointF());
	adjustLink();

}

void EdgeElement::placeEndTo(QPointF const &place)
{
	mLine[mLine.size() - 1] = place;
	updateLongestPart();
	if (SettingsManager::value("SquareLine", true).toBool())
		squarizeHandler(QPointF());
	adjustLink();
}

void EdgeElement::moveConnection(NodeElement *node, qreal const portId) {
	if (node == mSrc) {
		mPortFrom = portId;
		mGraphicalAssistApi->setFromPort(id(), mPortFrom);
	}
	if (node == mDst) {
		mPortTo = portId;
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

void EdgeElement::setColorRect(bool bl)
{
	Q_UNUSED(bl);
}

QPointF EdgeElement::from() const
{
	return mLine.first() + pos();
}

QPointF EdgeElement::to() const
{
	return mLine.last() + pos();
}

