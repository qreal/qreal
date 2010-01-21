/** @file edgeelement.cpp
 * 	@brief Класс, представляющий связь на диаграмме
 * */
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtGui/QStyle>
#include <QtGui/QTextDocument>
#include <QtGui/QMenu>
#include <QtCore/QSettings>
#include <math.h>

#include "../view/editorviewscene.h"
#include "uml_edgeelement.h"
#include "uml_nodeelement.h"
#include "../model/model.h"

#include <QDebug>

using namespace UML;
using namespace qReal;

#ifndef M_PI
/** @brief Константа ПИ */
#define M_PI 3.14159265358979323846264338327950288419717
/** @brief Константа 1/ПИ */
#define M_1_PI 1/M_PI;
// Реквестирую ещё массу бозона Хиггса!
// Here you are: The God's particle energy (in GeV)
#define HIGGS_BOSON_MASS_APPROX 251
#endif //M_PI

/** @brief Индикатор перемещения связи */
// static bool moving = false;

EdgeElement::EdgeElement()
	: mPenStyle(Qt::SolidLine), mStartArrowStyle(NO_ARROW), mEndArrowStyle(NO_ARROW),
	mSrc(NULL), mDst(NULL), mPortFrom(0), mPortTo(0),
	mDragState(-1), mLongPart(0), mBeginning(NULL), mEnding(NULL)
{
	setZValue(100);
	setFlag(ItemIsMovable, true);
	// FIXME: draws strangely...
	setFlag(ItemClipsToShape, false);
	setFlag(ItemClipsChildrenToShape, false);

	mLine << QPointF(0, 0) << QPointF(200, 60);

	setAcceptHoverEvents(true);

	QSettings settings("SPbSU", "QReal");
	mChaoticEdition = settings.value("ChaoticEdition", false).toBool();
}

EdgeElement::~EdgeElement()
{
	if (mSrc)
		mSrc->delEdge(this);
	if (mDst)
		mDst->delEdge(this);
}

QRectF EdgeElement::boundingRect() const
{
	return mLine.boundingRect().adjusted(-20, -20, 20, 20);
}

static double lineAngle(const QLineF &line)
{
	double angle = ::acos(line.dx() / line.length());
	if (line.dy() >= 0)
		angle = 2 * M_PI - angle;

	return angle * 180 * M_1_PI;
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
	pen.setWidth(1);
	painter->setPen(pen);
	painter->drawPolyline(mLine);
	painter->restore();

	painter->save();
	painter->translate(mLine[0]);
	painter->drawText(QPointF(10, 20), mFromMult);
	painter->rotate(90 - lineAngle(QLineF(mLine[1], mLine[0])));
	drawStartArrow(painter);
	painter->restore();

	painter->save();
	painter->translate(mLine[mLine.size() - 1]);
	painter->drawText(QPointF(10, 20), mToMult);
	painter->rotate(90 - lineAngle(QLineF(mLine[mLine.size() - 2], mLine[mLine.size() - 1])));
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

bool canBeConnected(int linkID, int from, int to);

QPainterPath EdgeElement::shape() const
{
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);

	QPainterPathStroker ps;
	ps.setWidth(kvadratik);

	path.addPolygon(mLine);
	path = ps.createStroke(path);

	foreach (QPointF const point, mLine) {
		path.addRect(getPortRect(point));
	}

	return path;
}

QRectF EdgeElement::getPortRect(QPointF const &point)
{
	return QRectF(point - QPointF(kvadratik, kvadratik), QSizeF(kvadratik * 2, kvadratik * 2));
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

void EdgeElement::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	mDragState = -1;

	mDragState = getPoint(event->pos());

	if (mDragState == -1)
		Element::mousePressEvent(event);
}

void EdgeElement::connectToPort()
{
	model::Model *model = const_cast<model::Model *>(static_cast<model::Model const *>(mDataIndex.model()));  // TODO: OMG!

	setPos(pos() + mLine.first());
	mLine.translate(-mLine.first());

	mMoving = true;

	// Now we check whether start or end have been connected
	NodeElement *new_src = getNodeAt(mLine.first());
	mPortFrom = new_src ? new_src->getPortId(mapToItem(new_src, mLine.first())) : -1.0;

	if (mSrc) {
		mSrc->delEdge(this);
		mSrc = 0;
	}

	if (mPortFrom >= 0.0) {
		mSrc = new_src;
		mSrc->addEdge(this);
	}

	model->setData(mDataIndex, (mSrc ? mSrc->uuid() : ROOT_ID).toVariant(), roles::fromRole);
	model->setData(mDataIndex, mPortFrom, roles::fromPortRole);

	NodeElement *new_dst = getNodeAt(mLine.last());
	mPortTo = new_dst ? new_dst->getPortId(mapToItem(new_dst, mLine.last())) : -1.0;

	if (mDst) {
		mDst->delEdge(this);
		mDst = 0;
	}

	if (mPortTo >= 0.0) {
		mDst = new_dst;
		mDst->addEdge(this);
	}

	model->setData(mDataIndex, (mDst ? mDst->uuid() : ROOT_ID).toVariant(), roles::toRole);
	model->setData(mDataIndex, mPortTo, roles::toPortRole);

	setFlag(ItemIsMovable, !(mDst || mSrc));

	model->setData(mDataIndex, pos(), roles::positionRole);
	model->setData(mDataIndex, mLine.toPolygon(), roles::configurationRole);

	mMoving = false;

	adjustLink();
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

	if (mDragState == -1) {
		Element::mouseMoveEvent(event);
	} else {
		prepareGeometryChange();
		mLine[mDragState] = event->pos();
		updateLongestPart();
	}
}

void EdgeElement::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (mDragState == -1)
		Element::mouseReleaseEvent(event);
	else
		mDragState = -1;

	connectToPort();

	if (mBeginning)
		mBeginning->setPortsVisible(false);

	if (mEnding)
		mEnding->setPortsVisible(false);

	// cleanup after moving/resizing
	mBeginning = mEnding = NULL;
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

void EdgeElement::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	QMenu menu;

	QAction *addPointAction = menu.addAction("Add point");
	QAction *delPointAction = menu.addAction("Remove point");
	QAction *squarizeAction = menu.addAction("Squarize");

	if (QAction *selectedAction = menu.exec(event->screenPos())) {
		if (selectedAction == delPointAction) {
			int pointIndex = getPoint(event->pos());
			if (pointIndex != -1) {
				prepareGeometryChange();
				mLine.remove(pointIndex);
				mLongPart = 0;
				update();
			}
		} else if (selectedAction == addPointAction) {
			for (int i = 0; i < mLine.size() - 1; ++i) {
				QPainterPath path;
				QPainterPathStroker ps;
				ps.setWidth(kvadratik);

				path.moveTo(mLine[i]);
				path.lineTo(mLine[i+1]);
				if (ps.createStroke(path).contains(event->pos())) {
					mLine.insert(i + 1, event->pos());
					update();
					break;
				}
			}
		} else if (selectedAction == squarizeAction) {
			prepareGeometryChange();
			for (int i = 0; i < mLine.size() - 1; ++i) {
				QLineF line(mLine[i], mLine[i + 1]);
				if (qAbs(line.dx()) < qAbs(line.dy())) {
					mLine[i + 1].setX(mLine[i].x());
				} else {
					mLine[i + 1].setY(mLine[i].y());
				}
			}
			adjustLink();
			update();
		}
	}
}

void EdgeElement::adjustLink()
{
	prepareGeometryChange();
	if (mSrc)
		mLine.first() = mapFromItem(mSrc, mSrc->getPortPos(mPortFrom));
	if (mDst)
		mLine.last() = mapFromItem(mDst, mDst->getPortPos(mPortTo));
	updateLongestPart();
}

void EdgeElement::updateData()
{
	if (mMoving)
		return;

	Element::updateData();

	setPos(mDataIndex.data(roles::positionRole).toPointF());
	QPolygonF newLine = mDataIndex.data(roles::configurationRole).value<QPolygon>();
	if (!newLine.isEmpty())
		mLine = newLine;

	qReal::Id uuidFrom = mDataIndex.data(roles::fromRole).value<Id>();
	qReal::Id uuidTo = mDataIndex.data(roles::toRole).value<Id>();

	if (mSrc)
		mSrc->delEdge(this);
	if (mDst)
		mDst->delEdge(this);

	mSrc = dynamic_cast<NodeElement *>(static_cast<EditorViewScene *>(scene())->getElem(uuidFrom));
	mDst = dynamic_cast<NodeElement *>(static_cast<EditorViewScene *>(scene())->getElem(uuidTo));

	if (mSrc)
		mSrc->addEdge(this);
	if (mDst)
		mDst->addEdge(this);

	setFlag(ItemIsMovable, !(mDst || mSrc));

	mPortFrom = mDataIndex.data(roles::fromPortRole).toDouble();
	mPortTo = mDataIndex.data(roles::toPortRole).toDouble();

	adjustLink();
}

void EdgeElement::removeLink(UML::NodeElement const *from)
{
	if (mSrc == from)
		mSrc = NULL;

	if (mDst == from)
		mDst = NULL;
}
