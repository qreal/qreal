/** @file uml_nodeelement.cpp
 * 	@brief Класс, представляющий объект на диаграмме
 * */
#include <QtGui/QStyle>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtGui/QMessageBox>
#include <QtGui/QTextCursor>
#include <QtGui/QToolTip>
#include <QtCore/QDebug>

#include "uml_nodeelement.h"
#include "../model/model.h"

using namespace UML;
using namespace qReal;

/* {{{ Element title */
void ElementTitle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsTextItem::mousePressEvent(event);
	if (!(event->modifiers() & Qt::ControlModifier))
		scene()->clearSelection();
	parentItem()->setSelected(true);
}

void ElementTitle::focusInEvent(QFocusEvent *event)
{
	mOldText = toHtml();
	QGraphicsTextItem::focusInEvent(event);
}

void ElementTitle::focusOutEvent(QFocusEvent *event)
{
	QString name = toPlainText();
	QString tmp = toHtml();
	QGraphicsTextItem::focusOutEvent(event);
	setTextInteractionFlags(Qt::NoTextInteraction);
	// FIXME: Reset selection
	setHtml("");
	setHtml(tmp);
	if (mOldText != toHtml())
		(static_cast<NodeElement*>(parentItem()))->setName(name);
}

void ElementTitle::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape)
	{
		// Restore previous text and loose focus
		setHtml(mOldText);
		clearFocus();
		return;
	}
	if (event->key() == Qt::Key_Enter ||
		event->key() == Qt::Key_Return)
	{
		// Loose focus: new name will be applied in focusOutEvent
		clearFocus();
		return;
	}
	QGraphicsTextItem::keyPressEvent(event);
}
/* }}} */

NodeElement::NodeElement()
: mPortsVisible(false), mDragState(None)
{
	setAcceptsHoverEvents(true);
}

NodeElement::~NodeElement()
{
	foreach (EdgeElement *edge, mEdgeList)
		edge->removeLink(this);
}

void NodeElement::setName(QString name)
{
	QAbstractItemModel *im = const_cast<QAbstractItemModel *>(mDataIndex.model());
	im->setData(mDataIndex, name, Qt::DisplayRole);
}

void NodeElement::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	if (isSelected())
	{
		if (QRectF(mContents.topLeft(), QSizeF(4, 4)).contains(event->pos()))
			mDragState = TopLeft;
		else if (QRectF(mContents.topRight(), QSizeF(-4, 4)).contains(event->pos()))
			mDragState = TopRight;
		else if (QRectF(mContents.bottomRight(), QSizeF(-12, -12)).contains(event->pos()))
			mDragState = BottomRight;
		else if (QRectF(mContents.bottomLeft(), QSizeF(4, -4)).contains(event->pos()))
			mDragState = BottomLeft;
		else
			Element::mousePressEvent(event);
	} else
		Element::mousePressEvent(event);

	if (event->button() == Qt::RightButton)
	{
		event->accept();
	}
}

void NodeElement::setGeometry(QRectF const &geom)
{
	prepareGeometryChange();
	setPos(geom.topLeft());
	if (geom.isValid())
		mContents = geom.translated(-geom.topLeft());
	mTransform.reset();
	mTransform.scale(mContents.width(), mContents.height());
	adjustLinks();
	mTitle.setTextWidth(mContents.width() - 15);
}

void NodeElement::adjustLinks() 
{
	foreach (EdgeElement *edge, mEdgeList)
		edge->adjustLink();

	foreach (QGraphicsItem *child, childItems()) {
		NodeElement *element = dynamic_cast<NodeElement*>(child);
		if (element)
			element->adjustLinks();
	}
}

void NodeElement::storeGeometry(void)
{
	QRectF tmp = mContents;
	model::Model *itemModel = const_cast<model::Model*>(static_cast<model::Model const *>(mDataIndex.model()));
	itemModel->setData(mDataIndex, pos(), roles::positionRole);
	itemModel->setData(mDataIndex, QPolygon(tmp.toAlignedRect()), roles::configurationRole);
}

void NodeElement::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (mDragState == None) {
		Element::mouseMoveEvent(event);
	} else {
		QRectF newcontents = mContents;

		switch (mDragState) {
		case TopLeft:
			newcontents.setTopLeft(event->pos());
			break;
		case Top:
			newcontents.setTop(event->pos().y());
			break;
		case TopRight:
			newcontents.setTopRight(event->pos());
			break;
		case Left:
			newcontents.setLeft(event->pos().x());
			break;
		case Right:
			newcontents.setRight(event->pos().x());
			break;
		case BottomLeft:
			newcontents.setBottomLeft(event->pos());
			break;
		case Bottom:
			newcontents.setBottom(event->pos().y());
			break;
		case BottomRight:
			newcontents.setBottomRight(event->pos());
			break;
		case None:
			break;
		}

		if (event->modifiers() & Qt::ShiftModifier)
		{
			qreal size = std::max(newcontents.width(), newcontents.height());
			newcontents.setWidth(size);
			newcontents.setHeight(size);
		}

		newcontents.translate(pos());
		if (!((newcontents.width() < 10) || (newcontents.height() < 10)))
			setGeometry(newcontents);
	}
}

void NodeElement::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	mContents = mContents.normalized();
	storeGeometry();

//	NodeElement *newParent = getNodeAt(event->scenePos());

	if (mDragState != None)
		mDragState = None;
	else
		Element::mouseReleaseEvent(event);

	/*
	EditorViewScene *evscene = dynamic_cast<EditorViewScene *>(scene());
	if (newParent) {
		itemModel->changeParent(dataIndex,newParent->dataIndex,
						 mapToItem(evscene->getElemByModelIndex(newParent->dataIndex),mapFromScene(scenePos())));
	} else {
		itemModel->changeParent(dataIndex,evscene->rootItem(),scenePos());
	}
	*/
}

QVariant NodeElement::itemChange(GraphicsItemChange change, const QVariant &value)
{
	switch (change) {
		case ItemPositionHasChanged:
			adjustLinks();
			return value;
		default:
			return QGraphicsItem::itemChange(change, value);
	}
}

QRectF NodeElement::contentsRect() const
{
	return mContents;
}

QRectF NodeElement::boundingRect() const
{
	return mContents.adjusted(-kvadratik, -kvadratik, kvadratik, kvadratik);
}

void NodeElement::updateData()
{
	Element::updateData();
	if (mMoving == 0) {
		QPointF newpos = mDataIndex.data(roles::positionRole).toPointF();
		QPolygon newpoly = mDataIndex.data(roles::configurationRole).value<QPolygon>();
		QRectF newRect; // Use default ((0,0)-(0,0))
		// QPolygon::boundingRect is buggy :-(
		if (!newpoly.isEmpty()) {
			int minx = newpoly[0].x();
			int miny = newpoly[0].y();
			int maxx = newpoly[0].x();
			int maxy = newpoly[0].y();;
			for (int i = 1; i < newpoly.size(); ++i) {
				if (minx > newpoly[i].x())
					minx = newpoly[i].x();
				if (maxx < newpoly[i].x())
					maxx = newpoly[i].x();
				if (miny > newpoly[i].y())
					miny = newpoly[i].y();
				if (maxy < newpoly[i].y())
					maxy = newpoly[i].y();
			}
			newRect = QRectF(QPoint(minx, miny), QSize(maxx - minx, maxy - miny));
		}
		setGeometry(newRect.translated(newpos));
	}
}

static int portId(qreal id)
{
	int iid = qRound(id);
	if ( id < ( 1.0 * iid ) )
		return iid - 1;
	else
		return iid;
}

const QPointF NodeElement::getPortPos(qreal id) const
{
	int iid = portId(id);

	if (id < 0.0)
		return QPointF(0, 0);
	if (id < mPointPorts.size())
		return mTransform.map(mPointPorts[iid]);
	if (id < mPointPorts.size() + mLinePorts.size())
		return newTransform(mLinePorts.at(iid - mPointPorts.size())).pointAt(id - 1.0 * iid);
	else
		return QPointF(0, 0);
}

QLineF NodeElement::newTransform(const StatLine& port) const
{
	float x1 = 0.0;
	float x2 = 0.0;
	float y1 = 0.0;
	float y2 = 0.0;

	if (port.prop_x1)
		x1 = port.line.x1() * 100;
	else
		x1 = port.line.x1() * contentsRect().width();

	if (port.prop_y1)
		y1 = port.line.y1() * 100;
	else
		y1 = port.line.y1() * contentsRect().height();

	if (port.prop_x2)
		x2 = port.line.x2() * 100;
	else
		x2 = port.line.x2() * contentsRect().width();

	if (port.prop_y2)
		y2 = port.line.y2() * 100;
	else
		y2 = port.line.y2() * contentsRect().height();

	return QLineF(x1, y1, x2, y2);
}

qreal NodeElement::getPortId(const QPointF &location) const
{
	for (int i = 0; i < mPointPorts.size(); ++i) {
		if (QRectF(mTransform.map(mPointPorts[i]) - QPointF(kvadratik, kvadratik),
				   QSizeF(kvadratik * 2, kvadratik * 2)).contains(location))
			return 1.0 * i;
	}

	for( int i = 0; i < mLinePorts.size(); i++ ) {
		QPainterPathStroker ps;
		ps.setWidth(kvadratik);

		QPainterPath path;
		path.moveTo(newTransform(mLinePorts[i]).p1());
		path.lineTo(newTransform(mLinePorts[i]).p2());

		path = ps.createStroke(path);
		if (path.contains(location))
			return (1.0 * (i + mPointPorts.size()) + qMin(0.9999,
				QLineF(QLineF(newTransform(mLinePorts[i])).p1(), location).length()
				/ newTransform(mLinePorts[i]).length() ) );
	}

	if (mPointPorts.size()!=0) {
		int numMinDistance = 0;
		qreal minDistance = QLineF( mPointPorts[0], mTransform.inverted().map(location) ).length();
		for( int i = 0; i < mPointPorts.size(); i++ ) {
			if (QLineF( mPointPorts[i], mTransform.inverted().map(location) ).length()<minDistance) {
				numMinDistance = i;
				minDistance = QLineF( mPointPorts[i], mTransform.inverted().map(location) ).length();
			}
		}
		return 1.0 * numMinDistance;
	} else if (mLinePorts.size()!=0) {
		int numMinDistance = 0;
		qreal minDistance = QLineF( QLineF(mLinePorts[0]).p1(), mTransform.inverted().map(location) ).length();
		for( int i = 0; i < mLinePorts.size(); i++ ) {
			if (QLineF( QLineF(mLinePorts[i]).p1(), mTransform.inverted().map(location) ).length()<minDistance) {
				numMinDistance = i;
				minDistance = QLineF( QLineF(mLinePorts[i]).p1(), mTransform.inverted().map(location) ).length();
			}
		}

		// the nearest point of line port
		QLineF nearestLinePort = mLinePorts[numMinDistance];
		qreal k = (nearestLinePort.y2() - nearestLinePort.y1()) / (nearestLinePort.x2() - nearestLinePort.x1());
		QPointF p0 = mTransform.inverted().map(location);
		qreal x = (-k*k*nearestLinePort.x1() + k*nearestLinePort.y1() - k*p0.y() - p0.x()) / (k*k + 1);
		qreal nearestPointOfLinePort = (x - nearestLinePort.x1()) / (nearestLinePort.x2() - nearestLinePort.x1());

		return 1.0 * (numMinDistance + nearestPointOfLinePort + mPointPorts.size());
	}
	return -1.0;
}

void NodeElement::setPortsVisible(bool value)
{
	prepareGeometryChange();
	mPortsVisible = value;
}

void NodeElement::complexInlineEditing()
{
	if ((mDocVis.toPlainText() == "") && (mDocType.toPlainText() == "")){
	mDocVis.setTextWidth(0);
	mDocType.setTextWidth(0);
	mTitle.setPos(15, mContents.height() - 15);
	mTitle.setTextWidth(mContents.width() - 25);
	} else
	if ((mDocVis.toPlainText() == "") && (mDocType.toPlainText() != "")){
	mDocVis.setTextWidth(0);
	mDocType.setPos(1, mContents.height() - 15);
	if (mTypeText.length() * 5 < 6*mContents.width() / 16)
		mDocType.setTextWidth(mTypeText.length() * 5);
	else
		mDocType.setTextWidth(6 * mContents.width() / 16);

	mTitle.setPos(mDocType.textWidth(), mContents.height() - 15);
	mTitle.setTextWidth(mContents.width() - mDocType.textWidth() - 30);
	} else
	if ((mDocVis.toPlainText() != "") && (mDocType.toPlainText() == "")){
	mDocType.setTextWidth(0);
	mDocVis.setPos(1, mContents.height() - 15);
	mDocVis.setTextWidth(11);
	mTitle.setPos(16, mContents.height() - 15);
	mTitle.setTextWidth(mContents.width() - 37);
	} else
	if ((mDocVis.toPlainText() != "") && (mDocType.toPlainText() != "")){
	mDocVis.setPos(1, mContents.height() - 15);
	mDocVis.setTextWidth(11);
	mDocType.setPos (16, mContents.height() - 15);
	if (mTypeText.length() * 5 < 6 * mContents.width() / 16)
		mDocType.setTextWidth(mTypeText.length() * 5);
	else
		mDocType.setTextWidth(6 * mContents.width() / 16);

	mTitle.setPos(mDocVis.textWidth() + mDocType.textWidth(),  mContents.height() - 15);
	mTitle.setTextWidth(mContents.width() - mDocType.textWidth() - 30);
	}
}

NodeElement *NodeElement::getNodeAt( const QPointF &position )
{
	foreach( QGraphicsItem *item, scene()->items(position) ) {
		NodeElement *e = dynamic_cast<NodeElement *>(item);
		if (( e )&&(item!=this))
			return e;
	}
	return 0;
}

void NodeElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget*, SdfRenderer* portrenderer)
{
	if (option->levelOfDetail >= 0.5)
	{
		if (option->state & QStyle::State_Selected)
		{
			painter->save();

			QBrush b;
			b.setColor(Qt::blue);
			b.setStyle(Qt::SolidPattern);
			painter->setBrush(b);
			painter->setPen(Qt::blue);

			painter->drawRect(QRectF(mContents.topLeft(),QSizeF(4,4)));
			painter->drawRect(QRectF(mContents.topRight(),QSizeF(-4,4)));
			painter->drawRect(QRectF(mContents.bottomLeft(),QSizeF(4,-4)));

			painter->translate(mContents.bottomRight());
			painter->drawLine(QLineF(-4,0,0,-4));
			painter->drawLine(QLineF(-8,0,0,-8));
			painter->drawLine(QLineF(-12,0,0,-12));

			painter->restore();
		}
		if (((option->state & QStyle::State_MouseOver) || mPortsVisible) && portrenderer)
		{
			painter->save();
			painter->setOpacity(0.7);
			portrenderer->render(painter,mContents);
			painter->restore();
		}
	}
}

void NodeElement::addEdge(EdgeElement *edge)
{
	mEdgeList << edge;
}

void NodeElement::delEdge(EdgeElement *edge)
{
	mEdgeList.removeAt(mEdgeList.indexOf(edge));
}
