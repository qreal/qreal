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
#include "../view/editorViewScene.h"
#include "../editorPluginInterface/editorInterface.h"

using namespace qReal;

const double pi = 3.14159265358979;

const qreal epsilon = 0.00000000001;

/** @brief indicator of edges' movement */

EdgeElement::EdgeElement(ElementImpl *impl)
		: Element(impl)
		, mPenStyle(Qt::SolidLine), mPenWidth(1), mPenColor(Qt::black), mStartArrowStyle(NO_ARROW), mEndArrowStyle(NO_ARROW)
		, mSrc(NULL), mDst(NULL)
		, mPortFrom(0), mPortTo(0)
		, mDragPoint(-1), mLongPart(0)
		, mAddPointAction(tr("Add point"), this)
		, mDelPointAction(tr("Delete point"), this)
		, mSquarizeAction(tr("Squarize"), this)
		, mMinimizeAction(tr("Remove all points"), this)
		, mDelSegmentAction(tr("Remove segment"), this)
		, mReverseAction(tr("Reverse"), this)
		, mChangeSquarizeTypeAction(tr("Change drawing type"), this)
		, mModelUpdateIsCalled(false)
		, isCircle(false)
		, isVerticalChanging(false)
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
	mLastLineIsCircle = false;

	mLeftButtonIsPressed = false;
	mSavedLineForChanges = mLine;
	mSavesDragPointForSquarize = -1;

	setAcceptHoverEvents(true);

	connect(&mAddPointAction, SIGNAL(triggered(QPointF const &)), SLOT(addClosestPointHandler(QPointF const &)));
	connect(&mDelPointAction, SIGNAL(triggered(QPointF const &)), SLOT(delPointHandler(QPointF const &)));
	connect(&mDelPointAction, SIGNAL(triggered(QPointF const &)), SLOT(arrangeAndAdjustHandler(QPointF const &)));
	connect(&mSquarizeAction, SIGNAL(triggered(QPointF const &)), SLOT(squarizeAndAdjustHandler(QPointF const &)));
	// The following line is necessary because squarizeAndAdjustHandler() is used in adjustLink()
	connect(&mSquarizeAction, SIGNAL(triggered(QPointF const &)), SLOT(setGraphicApi(QPointF)));
	connect(&mChangeSquarizeTypeAction, SIGNAL(triggered(QPointF const &)), SLOT(changeSquarizeType(QPointF const &)));
	connect(&mReverseAction, SIGNAL(triggered(QPointF const)), SLOT(reverseHandler(QPointF const &)));
	connect(&mMinimizeAction, SIGNAL(triggered(QPointF const &)), SLOT(minimizeHandler(QPointF const &)));
	connect(&mDelSegmentAction, SIGNAL(triggered(QPointF const &)), SLOT(deleteSegmentHandler(QPointF const &)));
	connect(&mDelSegmentAction, SIGNAL(triggered(QPointF const &)), SLOT(arrangeAndAdjustHandler(QPointF const &)));

	mChaoticEdition = SettingsManager::value("ChaoticEdition").toBool();

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

void EdgeElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget*)
{
	if (!(mSavedLineForSquarize.size() < 2 || !SettingsManager::value("SquareLine").toBool() || mSavesDragPointForSquarize == -1)) {
		painter->save();
		QPen pen1 = painter->pen();
		pen1.setColor(QColor(Qt::magenta));
		pen1.setBrush(QColor(Qt::magenta));
		pen1.setStyle(Qt::DashDotLine);
		pen1.setWidth(mPenWidth);
		painter->setPen(pen1);
		painter->setOpacity(0.5);
		painter->setRenderHint(QPainter::Antialiasing);
		painter->drawPolyline(mSavedLineForSquarize);
		painter->restore();

		painter->save();
		painter->translate(mSavedLineForSquarize[0]);
		painter->rotate(90 - lineAngle(QLineF(mSavedLineForSquarize[1], mSavedLineForSquarize[0])));
		pen1 = painter->pen();
		pen1.setColor(QColor(Qt::magenta));
		pen1.setBrush(QColor(Qt::magenta));
		pen1.setStyle(mPenStyle);
		pen1.setWidth(3);
		painter->setPen(pen1);
		painter->setOpacity(0.5);
		painter->setRenderHint(QPainter::Antialiasing);
		drawStartArrow(painter);
		painter->restore();

		painter->save();
		pen1 = painter->pen();
		pen1.setColor(QColor(Qt::magenta));
		pen1.setBrush(QColor(Qt::magenta));
		pen1.setStyle(mPenStyle);
		pen1.setWidth(3);
		painter->setPen(pen1);
		painter->setOpacity(0.5);
		painter->setRenderHint(QPainter::Antialiasing);
		painter->translate(mSavedLineForSquarize[mSavedLineForSquarize.size() - 1]);
		painter->rotate(90 - lineAngle(QLineF(mSavedLineForSquarize[mSavedLineForSquarize.size() - 2], mSavedLineForSquarize[mSavedLineForSquarize.size() - 1])));
		painter->setPen(pen1);
		drawEndArrow(painter);
		painter->restore();
	}

	// painting mLine itself
	painter->save();
	QPen pen = painter->pen();
	pen.setColor(mColor);
	pen.setBrush(mColor);
	pen.setStyle(mPenStyle);
	pen.setWidth(mPenWidth);
	painter->setPen(pen);
	painter->drawPolyline(mLine);
	painter->restore();

	painter->save();
	painter->translate(mLine[0]);
	painter->drawText(QPointF(10, 20), mFromMult);
	painter->rotate(90 - lineAngle(QLineF(mLine[1], mLine[0])));
	pen = painter->pen();
	pen.setColor(mColor);
	pen.setBrush(mColor);
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
	pen.setBrush(mColor);
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
	ps.setWidth(kvadratik - 2.5);

	path.addPolygon(mLine);
	path = ps.createStroke(path);

	foreach (QPointF const point, mLine) {
		path.addRect(getPortRect(point).adjusted(1, 1, -1, -1)); // (see #569 and #165)
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
	mMoving = true;

	NodeElement *newSrc = getNodeAt(mLine.first(), true);
	NodeElement *newDst = getNodeAt(mLine.last(), false);

	if ((newSrc == newDst) && newSrc) {
		isCircle = true;
	} else {
		isCircle = false;
	}

	if (newSrc == newDst && newSrc && mLine.size() <= 3) {
		if (!mLastLine.isEmpty()) {
			if (mLastLine.size() > 2 && !mLastLineIsCircle) {
				isCircle = false;
				//highlight(Qt::gray);
			} else {
				//highlight(Qt::blue);
			}
			mLine = mLastLine;
			mMoving = false;
			return;
		} else {
			// FIXME: need createCircleLink()'s implementation. linker's edges, for example, transforms to point now (see #602)
			isCircle = true;
			connectCircleEdge(newSrc);
			createCircleLink();
			return;
			//highlight(Qt::yellow);
		}
	}

	if (isCircle) {
		connectCircleEdge(newSrc);
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
	setGraphicApi(QPointF());

	mMoving = false;

	if (newSrc && newDst) {
		highlight(mPenColor);
	} else {
		highlight(Qt::red);
	}
}

void EdgeElement::connectCircleEdge(NodeElement *newMaster)
{
	setPos(pos() + mLine.first());
	mLine.translate(-mLine.first());

	mPortFrom = newMaster ? newMaster->portId(mapToItem(newMaster, mLine.first())) : -1.0;
	mPortTo = mPortFrom;//1.999;

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
	setGraphicApi(QPointF());

	mMoving = false;

	highlight(mPenColor);
	//highlight(Qt::green);
}

void EdgeElement::createCircleLink() // nice implementation makes sense after #602 fixed!
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
	isCircle = true;
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
	if (mSavedLineForSquarize.size() >= 2 && SettingsManager::value("SquareLine").toBool()) {
		scene()->update();
	}
	if (event->button() == Qt::RightButton) {
		event->accept();
		if (mDragPoint == -3) {
			return;
		}
	}
	if (!SettingsManager::value("SquareLine").toBool() && (event->modifiers() & Qt::AltModifier)
		&& (getPoint(event->pos()) != -1) && (event->button() == Qt::LeftButton) && delPointActionIsPossible(event->pos()))
	{
		delPointHandler(event->pos());
		return;
	}
	if (event->button() == Qt::RightButton && mLeftButtonIsPressed) {
		prepareGeometryChange();
		mLine = mSavedLineForChanges;
		mDragPoint = -3;
		mLeftButtonIsPressed = false;
		return;
	}
	if (event->button() == Qt::LeftButton) {
		mLeftButtonIsPressed = true;
	}

	mDragPoint = getPoint(event->pos());

	if ((mSrc && mDst && mSrc->isSelected() && mDst->isSelected() && isSelected())
		|| (mSrc && !mDst && mSrc->isSelected() && isSelected())
		|| (mDst && !mSrc && mDst->isSelected() && isSelected()))
	{
		mLeftButtonIsPressed = false;
		mDragPoint = -1;
		Element::mousePressEvent(event);
	}
	else if (mDragPoint == -1) {
			Element::mousePressEvent(event);
			if ((mSrc) || (mDst)) {
				if ((event->button() != Qt::RightButton) && !event->modifiers()) {
					mSavedLineForChanges = mLine;
					addPointHandler(event->pos());
					mSavedLineForSquarize = mLine;
					mSavesDragPointForSquarize = mDragPoint;
				}
			}
	} else if (mDragPoint != -1 && (event->button() != Qt::RightButton)) {
		mSavesDragPointForSquarize = mDragPoint;
		mLastLineIsCircle = isCircle;
		mLastLine = mLine;	// saving info in case we need to rollback (see #4)
		mSavedLineForChanges = mLine;
		mSavedLineForSquarize = mLine;
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
	if (!mLeftButtonIsPressed && mDragPoint == -3) {
		return;
	}

	if (mDragPoint == -1) {
		Element::mouseMoveEvent(event);
	} else {
		//qDebug() << "DragPoint: " << mDragPoint << "and MaxPoint: "<< mLine.size() - 1;
		mDragPoint = mSavesDragPointForSquarize;
		if (mDragPoint > mLine.size() - 1) {
			mDragPoint = -2;
			prepareGeometryChange();
			mLine = mSavedLineForChanges;
			return;
		}
		if (mDragPoint < 0) {
			return;
		}
		mLine = mSavedLineForSquarize;
		prepareGeometryChange();
		mLine[mDragPoint] = event->pos();
		if (SettingsManager::value("SquareLine").toBool()) {
			squarizeAndAdjustHandler(QPointF());
		}
		updateLongestPart();
	}
}

void EdgeElement::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	mDragPoint = -1;
	mSavesDragPointForSquarize = -1;
	scene()->update();
	if (event->button() == Qt::RightButton) {
		event->accept();
		return;
	}
	if (mLeftButtonIsPressed && event->button() == Qt::LeftButton) {
		mLeftButtonIsPressed = false;
	}

	Element::mouseReleaseEvent(event);

	connectToPort();

	if (mSrc) {
		mSrc->setPortsVisible(false);
	}

	if (mDst) {
		mDst->setPortsVisible(false);
	}

	delCloseLinePoints();

	adjustNeighborLinks();
	arrangeSrcAndDst();

	correctArrow();
	prepareGeometryChange();
	correctInception();
	adjustNeighborLinks();

	setGraphicApiPos();
	setGraphicApi(QPointF());
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
			} else if (i != 0){
				mLine.remove(i);
				i = i - 2;
			}
		}
	}
}

void EdgeElement::delCloseLinePoints()
{
	if (isCircle) { // rough prevention of transforming in the point (because #602)
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
NodeElement *EdgeElement::getNodeAt(QPointF const &position, bool isStart)
{
	QPainterPath circlePath;
	circlePath.addEllipse(mapToScene(position), 12, 12);
	QList <QGraphicsItem*> items = scene()->items(circlePath);
	if (isStart && items.contains(mSrc)) {
		return mSrc;
	}
	if (!isStart && items.contains(mDst)) {
		return mDst;
	}
	foreach (QGraphicsItem *item, items) {
		NodeElement *e = dynamic_cast<NodeElement *>(item);
		if (e) {
			return e;
		}
	}
	return NULL;
}

QList<ContextMenuAction*> EdgeElement::contextMenuActions(const QPointF &pos)
{
	QList<ContextMenuAction*> result;
	if (delPointActionIsPossible(pos)) {
		result.push_back(&mDelPointAction);
	}
	if (squarizeActionIsPossible()) {
		result.push_back(&mSquarizeAction);
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
	if (changeSquarizeTypeActionIsPossible()) {
		result.push_back(&mChangeSquarizeTypeAction);
	}
	return result;
}

bool EdgeElement::delPointActionIsPossible(const QPointF &pos)
{
	if (isCircle && mLine.size() <= 4) {
		return false;
	}
	if (!SettingsManager::value("SquareLine").toBool()) {
		int pointIndex = getPoint(pos);
		// it is understood that there is a point and its index is equal to the index of the first and last (end) points
		return (pointIndex != -1 && pointIndex != mLine.count() - 1 && pointIndex);
	} else {
		return false;
	}
}

bool EdgeElement::squarizeActionIsPossible()
{
	//return (!SettingsManager::value("SquareLine").toBool());
	return false; // squarize action is unneeded because it's poorly now (need change conception of this action)
}

bool EdgeElement::addPointActionIsPossible(const QPointF &pos)
{
	int pointIndex = getPoint(pos);
	return !(mSrc || mDst) && (pointIndex == -1); // point's adding is unneeded now besides alone link's event
}

bool EdgeElement::delSegmentActionIsPossible(const QPointF &pos)
{
	if (isCircle) {
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
	if (isCircle) {
		return false;
	}
	return !(mLine.size() == 2) && !(mSrc == mDst) && !(SettingsManager::value("SquareLine").toBool() && mLine.size() == 3);
}

bool EdgeElement::reverseActionIsPossible()
{
	return true;
}

bool EdgeElement::changeSquarizeTypeActionIsPossible()
{
	return !isCircle && SettingsManager::value("SquareLine").toBool();
}

void EdgeElement::changeSquarizeType(QPointF const &pos)
{
	Q_UNUSED(pos);
	isVerticalChanging = !isVerticalChanging;
	arrangeSrcAndDst();
	adjustNeighborLinks();
	//correctArrow();
	//prepareGeometryChange();
	//correctInception();
	setGraphicApi(QPointF());
}

QList<PossibleEdge> EdgeElement::getPossibleEdges()
{
	return possibleEdges;
}

void EdgeElement::delPointHandler(QPointF const &pos)
{
	int pointIndex = getPoint(pos);
	// it is understood that there is a point and its index is equal to the index of the first and last (end) points
	if (pointIndex != -1 && pointIndex != mLine.count() - 1 && pointIndex) {
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
	setGraphicApi(QPointF());
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

void EdgeElement::squarizeHandler(QPointF const &pos)
{
	Q_UNUSED(pos);
	prepareGeometryChange();

	int i = 0;
	while (i + 1 < mLine.size()) {

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
				break;
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
				i++; // there is a possibility of several points in a row, so remove "i++" is possible, but they are deleted by adjustLink()
				continue;
			}
			if (mLine[i + 1].y() == mLine[i].y() && mLine[i + 2].y() == mLine[i].y()) {
				mLine.remove(i + 1);
				i++;
				continue;
			}
		}

		QPointF insPoint = mLine[i]; //point to insert between 2 others to make right angle

		//dont make new points on line
		if (insPoint.x() == mLine[i + 1].x() || insPoint.y() == mLine[i + 1].y()) {
			i++;
			continue;
		}
		if (!isVerticalChanging) {
			insPoint.setX(mLine[i + 1].x()); // maybe shold be added potential of choose of x or y?
		} else {
			insPoint.setY(mLine[i + 1].y());
		}

		mLine.insert(i + 1, insPoint); //insert new point to make right angle
		i += 2;
	}

	//correctArrow();

	update();
}

void EdgeElement::squarizeAndAdjustHandler(QPointF const &pos)
{
	squarizeHandler(pos);
	deleteLoops(); // this
	delCloseLinePoints(); // this
	squarizeHandler(pos); // and this need for correct drawing links when many edgeElements exist on the side (line)
	for (int i = 0; i < mLine.size() - 2; ++i) {
		if (removeOneLinePoints(i))
			--i;
	}
	updateLongestPart();
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
	setGraphicApi(QPointF());
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
		if (SettingsManager::value("SquareLine").toBool()) {
			squarizeAndAdjustHandler(QPointF());
		} else {
			updateLongestPart();
		}
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
		if (((mSrc && mDst && !(mDst->isSelected() && mSrc->isSelected()))
				|| ((!isSelected() && (mSrc ? mSrc->isSelected() : true) && (mDst ? mDst->isSelected() : true))))
//				|| (isSelected() && !(mSrc && mDst) && (mSrc || mDst) && (mSrc ? !mSrc->isSelected() : true) && (mDst ? !mDst->isSelected() : true)))
				&& SettingsManager::value("SquareLine").toBool()) {
			squarizeAndAdjustHandler(QPointF());
		}
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
		if (!jumpsOnly || reconnectedDst) { //&& !isCircle) {
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
	if (mDst && mDst != mSrc) {
		mDst->delEdge(this);
	}

	mSrc = dynamic_cast<NodeElement *>(static_cast<EditorViewScene *>(scene())->getElem(idFrom));
	mDst = dynamic_cast<NodeElement *>(static_cast<EditorViewScene *>(scene())->getElem(idTo));

	if (mSrc) {
		mSrc->addEdge(this);
	}
	if (mDst && mDst != mSrc) {
		mDst->addEdge(this);
	}

	mPortFrom = mGraphicalAssistApi->fromPort(id());
	mPortTo = mGraphicalAssistApi->toPort(id());

	adjustLink();
	if (mSrc != mDst) {
		correctArrow();
		prepareGeometryChange();
		correctInception();
	}
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

	isCircle = false;

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
		QPolygonF newMLine;
		newMLine << mLine.first() << mLine.last();
		mLine = newMLine;
		squarizeHandler(QPointF());
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
	if (node == mDst) {//&& !isCircle) {
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
		direct = -1;
		if ((!from && (mLine[mLine.size() - 2].y() > mLine.last().y()))
				|| (from && (mLine[1].y() < mLine[0].y()))) {
			direct = -3;
		}
	}
	if (port < 1 && port > 0) {
		direct = -2;
		if ((!from && (mLine[mLine.size() - 2].x() > mLine.last().x()))
				|| (from && (mLine[1].x() < mLine[0].x()))) {
			direct = -4;
		}
	}
	if (port < 3 && port > 2) {
		direct = 2;
		if ((!from && (mLine[mLine.size() - 2].x() < mLine.last().x()))
				|| (from && (mLine[1].x() > mLine[0].x()))) {
			direct = 4;
		}
	}
	if (port < 4 && port > 3) {
		direct = 1;
		if ((!from && (mLine[mLine.size() - 2].y() < mLine.last().y()))
				|| (from && (mLine[1].y() > mLine[0].y()))) {
			direct = 3;
		}
	}
	return direct;
}

void EdgeElement::correctInception()
{
	if (!SettingsManager::value("SquareLine").toBool() || mLine.size() < 3 || isCircle || !mSrc || !mDst
			|| mSrc->isContainer() || mDst->isContainer() || ((lengthOfSegment(mLine.first(), mLine.last()) < 2.5 * kvadratik) && (mLine.size() <= 3))) {
		return;
	}

	qreal const rad = kvadratik * 2.5;

	int direct = defineDirection(true);

	if ((lengthOfSegment(mLine[0], mLine[1]) < rad) && (direct & 1) && isVerticalChanging) {
		if (lengthOfSegment(mLine[0], mLine[1]) > 5 && abs(direct) == 3) {
			return;
		}

		QPointF middle = (mLine[1] + mLine[2]) / 2;
		int dirLocation = 1;
		if (middle.x() < mLine.first().x()) {
			dirLocation = -1;
		}
		if (abs(middle.x() - mLine.first().x()) > 2 * rad) {
			middle.setX(dirLocation * 2 * rad + mLine.first().x());
		}

		qreal shiftY = (abs(direct) == 3) ? rad + abs(mLine[1].y() - mLine.first().y()) : rad;
		if (direct > 0) {
			direct = 1;
		} else {
			direct = -1;
		}
		qreal shiftX = (mPortFrom < 2) ? (-mPortFrom + 1.5) * mSrc->contentsRect().width() : (-mPortFrom + 3.5) * mSrc->contentsRect().width();
		mLine[1].setY(mLine.first().y() + direct * shiftY);
		middle.setX(middle.x() + shiftX);
		QPointF tmp = middle;
		tmp.setY(mLine.first().y() + direct * shiftY);
		mLine.insert(2, middle);
		mLine.insert(2, tmp);
	} else if ((lengthOfSegment(mLine[0], mLine[1]) < rad) && !(direct & 1) && !isVerticalChanging) {
		if (lengthOfSegment(mLine[0], mLine[1]) > 5 && abs(direct) == 4) {
			return;
		}

		QPointF middle = (mLine[1] + mLine[2]) / 2;
		int dirLocation = 1;
		if (middle.y() < mLine.first().y()) {
			dirLocation = -1;
		}
		if (abs(middle.y() - mLine.first().y()) > 2 * rad) {
			middle.setY(dirLocation * 2 * rad + mLine.first().y());
		}

		qreal shiftX = (abs(direct) == 4) ? rad + abs(mLine[1].y() - mLine.first().y()) : rad;
		if (direct > 0) {
			direct = 1;
		} else {
			direct = -1;
		}
		qreal shiftY = (mPortFrom < 1) ? (-mPortFrom + 0.5) * mSrc->contentsRect().width() : (-mPortFrom + 2.5) * mSrc->contentsRect().width();

		mLine[1].setX(mLine.first().x() + direct * shiftX);
		middle.setY(middle.y() + shiftY);
		QPointF tmp = middle;
		tmp.setX(mLine.first().x() + direct * shiftX);
		mLine.insert(2, middle);
		mLine.insert(2, tmp);
	}
	edgeInceptionOverlapsNode(direct);
	updateLongestPart();
}

void EdgeElement::edgeInceptionOverlapsNode(int direct)
{
	if (isVerticalChanging && !(direct & 1) && ((direct < 0 && mLine.first().x() < mLine[2].x())
			|| ((direct > 0 && mLine.first().x() > mLine[2].x())))) {
		edgeInceptionOverlapsNodeHorizontally(direct);
	} else if (!isVerticalChanging && (direct & 1) && ((direct < 0 && mLine.first().y() < mLine[2].y())
			|| ((direct > 0 && mLine.first().y() > mLine[2].y())))) {
		edgeInceptionOverlapsNodeUpright(direct);
	}
}

void EdgeElement::edgeInceptionOverlapsNodeUpright(int direct)
{
	if (direct > 0) {
		direct = -1;
	} else {
		direct = 1;
	}
	mLine.first().setY(mLine[0].y() + direct * mSrc->contentsRect().height());
	mLine.first().setX(mLine[1].x());
	mLine.remove(1);

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

void EdgeElement::edgeInceptionOverlapsNodeHorizontally(int direct)
{
	if (direct > 0) {
		direct = -1;
	} else {
		direct = 1;
	}
	mLine.first().setX(mLine[0].x() + direct * mSrc->contentsRect().width());
	mLine.first().setY(mLine[1].y());
	mLine.remove(1);

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


void EdgeElement::correctArrow()
{
	if (!SettingsManager::value("SquareLine").toBool() || mLine.size() < 3 || isCircle || !mSrc || !mDst
			|| mSrc->isContainer() || mDst->isContainer() || ((lengthOfSegment(mLine.last(), mLine.first()) < 2.5 * kvadratik) && (mLine.size() <= 3))) {
		return;
	}

	qreal const rad = kvadratik * 2.5;

	int direct = defineDirection(false);

	if (!(direct & 1) && !isVerticalChanging) {
		mLine[mLine.size() - 2].setY(mLine.last().y());
		mLine[mLine.size() - 2].setX(mLine[mLine.size() - 3].x());
		isVerticalChanging = true;
		edgeArrowOverlapsNodeHorizontally(direct);
		return;
	} else if ((direct & 1) && isVerticalChanging) {
		mLine[mLine.size() - 2].setX(mLine.last().x());
		mLine[mLine.size() - 2].setY(mLine[mLine.size() - 3].y());
		isVerticalChanging = false;
		edgeArrowOverlapsNodeUpright(direct);
		return;
	}

	if ((lengthOfSegment(mLine[mLine.size() - 1], mLine[mLine.size() - 2]) < rad) && (direct & 1)) {
		QPointF middle = (mLine[mLine.size() - 3] + mLine[mLine.size() - 2]) / 2;
		int dirLocation = 1;
		if (middle.x() < mLine.last().x()) {
			dirLocation = -1;
		}
		if (abs(middle.x() - mLine.last().x()) > 4 * rad) {
			middle.setX(dirLocation * 2 * rad + mLine.last().x());
		}
		if ((abs(direct) == 1) && lengthOfSegment(mLine[mLine.size() - 1], mLine[mLine.size() - 2]) > kvadratik * 1.5) {
			return;
		}
		qreal shiftY = (abs(direct) == 1) ? rad + abs(mLine[mLine.size() - 2].y() - mLine.last().y()) : rad;
		if (direct > 0) {
			direct = 1;
		} else {
			direct = -1;
		}
		qreal shiftX = (mPortTo < 2) ? (-mPortTo + 1.5) * mDst->contentsRect().width() : (-mPortTo + 3.5) * mDst->contentsRect().width();
		mLine[mLine.size() - 2].setY(mLine.last().y() + direct * shiftY);
		middle.setX(middle.x() + shiftX);
		QPointF tmp = middle;
		tmp.setY(mLine.last().y() + direct * shiftY);
		mLine.insert(mLine.size() - 2, middle);
		mLine.insert(mLine.size() - 2, tmp);
		isVerticalChanging = false;
	} else if ((lengthOfSegment(mLine[mLine.size() - 1], mLine[mLine.size() - 2]) < rad) && !(direct & 1)) {
		QPointF middle = (mLine[mLine.size() - 3] + mLine[mLine.size() - 2]) / 2;
		int dirLocation = 1;
		if (middle.y() < mLine.last().y()) {
			dirLocation = -1;
		}
		if (abs(middle.y() - mLine.last().y()) > 4 * rad) {
			middle.setY(dirLocation * 2 * rad + mLine.last().y());
		}
		if ((abs(direct) == 2) && lengthOfSegment(mLine[mLine.size() - 1], mLine[mLine.size() - 2]) > kvadratik * 1.5) {
			return;
		}
		qreal shiftX = (abs(direct) == 2) ? rad + abs(mLine[mLine.size() - 2].x() - mLine.last().x()) : rad;
		if (direct > 0) {
			direct = 1;
		} else {
			direct = -1;
		}
		qreal shiftY = (mPortTo < 1) ? (-mPortTo + 0.5) * mDst->contentsRect().width() : (-mPortTo + 2.5) * mDst->contentsRect().width();
		mLine[mLine.size() - 2].setX(mLine.last().x() + direct * shiftX);
		middle.setY(middle.y() + shiftY);
		QPointF tmp = middle;
		tmp.setX(mLine.last().x() + direct * shiftX);
		mLine.insert(mLine.size() - 2, middle);
		mLine.insert(mLine.size() - 2, tmp);
		isVerticalChanging = true;
	}
	edgeArrowOverlapsNode(direct);
}

void EdgeElement::edgeArrowOverlapsNode(int direct)
{
	if (!isVerticalChanging && (direct & 1) && ((direct < 0 && mLine.last().y() < mLine[mLine.size() - 2].y())
			|| ((direct > 0 && mLine.last().y() > mLine[mLine.size() - 2].y())))) {
		edgeArrowOverlapsNodeUpright(direct);
	} else if (isVerticalChanging && !(direct & 1) && ((direct < 0 && mLine.last().x() < mLine[mLine.size() - 2].x())
				|| ((direct > 0 && mLine.last().x() > mLine[mLine.size() - 2].x())))) {
		edgeArrowOverlapsNodeHorizontally(direct);
	}
}

void EdgeElement::edgeArrowOverlapsNodeUpright(int direct)
{
	if (direct > 0) {
		direct = -1;
	} else {
		direct = 1;
	}
	mLine.last().setY(mLine.last().y() + direct * mDst->contentsRect().height());

	mMoving = true;
	setPos(pos() + mLine.first());
	mLine.translate(-mLine.first());
	mPortTo = mDst ? mDst->portId(mapToItem(mDst, mLine.last())) : -1.0;
	mGraphicalAssistApi->setToPort(id(), mPortFrom);
	adjustNeighborLinks();
	arrangeSrcAndDst();
	mGraphicalAssistApi->setPosition(id(), pos());
	mMoving = false;
}

void EdgeElement::edgeArrowOverlapsNodeHorizontally(int direct)
{
	if (direct > 0) {
		direct = -1;
	} else {
		direct = 1;
	}
	mLine.last().setX(mLine.last().x() + direct * mDst->contentsRect().width());

	mMoving = true;
	setPos(pos() + mLine.first());
	mLine.translate(-mLine.first());
	mPortTo = mDst ? mDst->portId(mapToItem(mDst, mLine.last())) : -1.0;
	mGraphicalAssistApi->setToPort(id(), mPortFrom);
	adjustNeighborLinks();
	arrangeSrcAndDst();
	mGraphicalAssistApi->setPosition(id(), pos());
	mMoving = false;
}

void EdgeElement::redrawing(QPointF const &pos)
{
	Q_UNUSED(pos);
	if (isCircle) { // it's row prevention of transform to the point before fix #602
		createCircleLink();
	}
	adjustNeighborLinks();
	arrangeSrcAndDst();
	correctArrow();
	prepareGeometryChange();
	correctInception();
	mMoving = true;
	mGraphicalAssistApi->setPosition(id(), this->pos());
	mMoving = false;
	setGraphicApi(QPointF());
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
	mData.srcId = src()->id();
	mData.dstId = dst()->id();

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
	// should be adjusted deleteLoop and delCloseLinePoints to a good drawing considering isCircle (after fix #602)
	// It's need to drawing without point's links and "QTransform::rotate with NaN called" accordingly
	if (isCircle && ((SettingsManager::value("SquareLine").toBool() && mLine.size() <= 6) || mLine.size() <= 5)) {
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
			if (SettingsManager::value("SquareLine").toBool()) {
				squarizeHandler(QPointF());
			}
			for (int i = 0; i < mLine.size() - 2; i++) {
				if (removeOneLinePoints(i)) {
					i--;
				}
			}
			updateLongestPart();
			break;
		}
	}
	setGraphicApi(QPointF());
}

void EdgeElement::setGraphicApi(QPointF const &pos)
{
	Q_UNUSED(pos);
	mModelUpdateIsCalled = true;
	mGraphicalAssistApi->setConfiguration(id(), mLine.toPolygon());
}

void EdgeElement::reverseHandler(QPointF const &pos1)
{
	Q_UNUSED(pos1);

	int direct = defineDirection(true);

	bool isCrooked = !isCircle && SettingsManager::value("SquareLine").toBool() && lengthOfSegment(mLine[1], mLine[0]) < 3 * kvadratik
			&& (((direct & 1) && isVerticalChanging) || (!(direct & 1) && !isVerticalChanging));

	int length = mLine.size();
	for (int i = 0; i < (length >> 1); ++i) {
		QPointF tmp(mLine[i]);
		mLine[i] = mLine[length - 1 - i];
		mLine[length - 1 - i] = tmp;
	}

	reversingReconnectToPorts(mDst, mSrc);

	arrangeAndAdjustHandler(QPointF());

	if (isCrooked) {
		arrangeSrcAndDst(); // this
		adjustNeighborLinks(); // this
		arrangeSrcAndDst(); // this
		adjustNeighborLinks(); // and this - all are needed!

		correctArrow();
		prepareGeometryChange();
		correctInception();

		setGraphicApiPos();
		setGraphicApi(QPointF());
	}
	updateLongestPart();
}

void EdgeElement::reversingReconnectToPorts(NodeElement *newSrc, NodeElement *newDst)
{
	mMoving = true;

	if (isCircle) {
		connectCircleEdge(newSrc);
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
			if (!(mSrc && mDst) && (mSrc || mDst) && (mSrc ? !mSrc->isSelected() : true) && (mDst ? !mDst->isSelected() : true)
					&& SettingsManager::value("SquareLine").toBool()) {
				squarizeAndAdjustHandler(QPointF());
			}
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
	return isCircle;
}

/* TODO: It's alternative squarize methos
void EdgeElement::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	if (SettingsManager::value("SquareLine").toBool() && (event->modifiers() & Qt::AltModifier)
		&& (getPoint(event->pos()) == -1) && !isCircle)
	{
		int start;
		QPainterPath path;
		QPainterPathStroker ps;
		ps.setWidth(kvadratik >> 1);
		for (int i = 0; i < mLine.size() - 1; ++i) {
			path.moveTo(mLine[i]);
			path.lineTo(mLine[i + 1]);
			if (ps.createStroke(path).contains(event->pos())) {
				start = i;
				break;
			}
		}

		if ((start == 0 || start == 1) && mLine.size() == 3) {
			mLine.remove(1);
			QPolygonF newMLine;
			newMLine << mLine.first() << mLine.last();
			mLine = newMLine;
			if (mSrc) {
				prepareGeometryChange();
				mLine.first() = mapFromItem(mSrc, mSrc->portPos(mPortFrom));
			}
			if (mDst) {
				prepareGeometryChange();
				mLine.last() = mapFromItem(mDst, mDst->portPos(mPortTo));
			}
			arrangeSrcAndDst();
			prepareGeometryChange();
			specialSquarizeSegment(0);
		} else if ((start == 0 || start == 1) && mLine.size() > 3) {
			mLine.remove(1);
			if (mSrc) {
				mLine.first() = mapFromItem(mSrc, mSrc->portPos(mPortFrom));
			}
			prepareGeometryChange();
			specialSquarizeSegment(0);
		} else if ((start == mLine.size() - 3 || start == mLine.size() - 2) && mLine.size() > 3) {
			mLine.remove(mLine.size() - 2);
			if (mDst) {
				mLine.last() = mapFromItem(mDst, mDst->portPos(mPortTo));
			}
			prepareGeometryChange();
			specialSquarizeSegment(mLine.size() - 3);
		}
	}
	updateLongestPart();
	update();
}

void EdgeElement::specialSquarizeLink()
{
	prepareGeometryChange();
	for (int i = 0; i < mLine.size(); ++i) {
		specialSquarizeSegment(i);
	}
	delClosePoints();
	updateLongestPart();
	update();
}

void EdgeElement::specialSquarizeSegment(int start, bool xYx)
{
	QPointF middle = (mLine[start] + mLine[start + 1]) / 2;
	if (xYx) {
		mLine.insert(start + 1, QPointF(middle.x(), mLine[start].y()));
		mLine.insert(start + 2, QPointF(middle.x(), mLine[start + 2].y()));
	} else { // yXy
		mLine.insert(start + 1, QPointF(mLine[start].x(), middle.y()));
		mLine.insert(start + 2, QPointF(mLine[start + 2].x(), middle.y()));
	}
}
*/
