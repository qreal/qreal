/** @file edgeelement.cpp
 * 	@brief Класс, представляющий связь на диаграмме
 * */
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtGui/QStyle>
#include <QtGui/QTextDocument>
#include <QtGui/QMenu>
#include <math.h>

#include "../view/editorviewscene.h"
#include "uml_edgeelement.h"
#include "uml_nodeelement.h"
#include "../model/model.h"

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

	mLine << QPointF(0, 0) << QPointF(200, 60);
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

	if (option->state & QStyle::State_Selected) {
		painter->setBrush(Qt::SolidPattern);
		foreach (QPointF const point, mLine) {
			QPen pen;
			QColor color;

			color.setNamedColor("#c3dcc4");
			pen.setWidth(11);
			pen.setColor(color);
			painter->setPen(pen);
			painter->drawPoint(point);

			color.setNamedColor("#465945");
			pen.setWidth(3);
			pen.setColor(color);
			painter->setPen(pen);
			painter->drawPoint(point);
		}
	}

	if (!mText.isEmpty()) {
		painter->save();
		QLineF longest(mLine[mLongPart], mLine[mLongPart + 1]);
		painter->translate(mLine[mLongPart]);
		painter->rotate(-lineAngle(longest));

		QTextDocument text;
		text.setHtml(mText);
		text.setTextWidth(longest.length());
		text.drawContents(painter);

		painter->restore();
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
}

void EdgeElement::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	mDragState = -1;

	if (isSelected())
		mDragState = getPoint(event->pos());

	if (mDragState == -1)
		Element::mousePressEvent(event);
}

void EdgeElement::connectToPort()
{
	model::Model *model = const_cast<model::Model *>(static_cast<model::Model const *>(dataIndex.model()));  // TODO: OMG!

	setPos(pos() + mLine.first());
	mLine.translate(-mLine.first());

	moving = true;

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

	model->setData(dataIndex, (mSrc ? mSrc->uuid() : ROOT_ID).toVariant(), roles::fromRole);
	model->setData(dataIndex, mPortFrom, roles::fromPortRole);

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

	model->setData(dataIndex, (mDst ? mDst->uuid() : ROOT_ID).toVariant(), roles::toRole);
	model->setData(dataIndex, mPortTo, roles::toPortRole);

	setFlag(ItemIsMovable, !(mDst || mSrc));

	model->setData(dataIndex, pos(), roles::positionRole);
	model->setData(dataIndex, mLine.toPolygon(), roles::configurationRole);

	moving = false;

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
	foreach (QGraphicsItem *item, scene()->items(mapToScene(position))) {
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
	if (moving)
		return;

	Element::updateData();

	setPos(dataIndex.data(roles::positionRole).toPointF());
	QPolygonF newLine = dataIndex.data(roles::configurationRole).value<QPolygon>();
	if (!newLine.isEmpty())
		mLine = newLine;

	qReal::IdType uuidFrom = dataIndex.data(roles::fromRole).value<Id>();
	qReal::IdType uuidTo = dataIndex.data(roles::toRole).value<Id>();

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

	mPortFrom = dataIndex.data(roles::fromPortRole).toDouble();
	mPortTo = dataIndex.data(roles::toPortRole).toDouble();

	adjustLink();
}

void EdgeElement::removeLink(UML::NodeElement const *from)
{
	if (mSrc == from)
		mSrc = NULL;

	if (mDst == from)
		mDst = NULL;
}
