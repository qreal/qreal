/** @file uml_nodeelement.cpp
 * 	@brief Класс, представляющий объект на диаграмме
 * */
#include <QtGui>

#include "uml_nodeelement.h"
#include <QMessageBox>
#include <QTextCursor>
#include <QToolTip>

using namespace UML;

NodeElement::NodeElement()
: portsVisible(false)
{
	oldName="";
	setAcceptsHoverEvents(true);
	dragState = None;
	mLockChangeName = false;
	mLockUpdateText = false;
}

NodeElement::~NodeElement()
{
	foreach (EdgeElement *edge, edgeList)
		edge->removeLink(this);
}

void NodeElement::changeName()
{
/*	if(d.toPlainText() != oldName)
		{*/
	if (!mLockChangeName) {
		mLockUpdateText = true;
		QAbstractItemModel *im = const_cast<QAbstractItemModel *>(dataIndex.model());
			im->setData(dataIndex, d.toPlainText(), Qt::DisplayRole);
		mLockUpdateText = false;
	}
/*	}
	oldName=d.toPlainText();*/
}

void NodeElement::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	//d.setTextWidth(boundingRect().width()-25);

	//complexInlineEditing();
   // changeName();
	if (isSelected())
	{
		if (QRectF(m_contents.topLeft(), QSizeF(4, 4)).contains(event->pos()))
			dragState = TopLeft;
		else if (QRectF(m_contents.topRight(), QSizeF(-4, 4)).contains(event->pos()))
			dragState = TopRight;
		else if (QRectF(m_contents.bottomRight(), QSizeF(-12, -12)).contains(event->pos()))
			dragState = BottomRight;
		else if (QRectF(m_contents.bottomLeft(), QSizeF(4, -4)).contains(event->pos()))
			dragState = BottomLeft;
		else
			Element::mousePressEvent(event);
	} else
		Element::mousePressEvent(event);

	if (event->button() == Qt::RightButton)
	{
		event->accept();
	}
}

void NodeElement::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
	if ( dragState == None ) {
		Element::mouseMoveEvent(event);
	} else {
		QRectF newcontents = m_contents;

		switch ( dragState ) {
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

		if ( ! ( ( newcontents.width() < 10 ) || ( newcontents.height() < 10 ) ) ) {
			prepareGeometryChange();

			m_contents = newcontents;

			setPos(pos() + m_contents.topLeft());
			m_contents.translate(-m_contents.topLeft());

			transform.reset();
			transform.scale(m_contents.width(), m_contents.height());

			foreach (EdgeElement *edge, edgeList)
				edge->adjustLink();
		}
	}
}

void NodeElement::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	m_contents = m_contents.normalized();

	moving = 1;
	Q_ASSERT(dataIndex.isValid());
	moving = 0;
	if ( dragState != None )
		dragState = None;
	else
		Element::mouseReleaseEvent(event);
}

QVariant NodeElement::itemChange(GraphicsItemChange change, const QVariant &value)
{
	switch (change) {
		case ItemPositionHasChanged:
			foreach (EdgeElement *edge, edgeList)
				edge->adjustLink();
			return value;
		default:
			return QGraphicsItem::itemChange(change, value);
	}
}

QRectF NodeElement::contentsRect() const
{
	return m_contents;
}

QRectF NodeElement::boundingRect() const
{
	return m_contents.adjusted(-kvadratik,-kvadratik,kvadratik,kvadratik);
}

void NodeElement::updateData()
{
	Element::updateData();
	if (moving == 0) {
		transform.reset();
		transform.scale(m_contents.width(), m_contents.height());
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
		return QPointF(0,0);
	if (id < pointPorts.size())
		return transform.map(pointPorts[iid]);
	if (id < pointPorts.size() + linePorts.size())
		return newTransform(linePorts.at(iid - pointPorts.size())).pointAt(id - 1.0 * iid);
	else
		return QPointF(0,0);
}

QLineF NodeElement::newTransform(const statLine& port) const
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
	for( int i = 0; i < pointPorts.size(); i++ ) {
		if ( QRectF(transform.map(pointPorts[i])-QPointF(kvadratik,kvadratik),QSizeF(kvadratik*2,kvadratik*2)).contains( location ) )
			return 1.0 * i;
	}

	for( int i = 0; i < linePorts.size(); i++ ) {
		QPainterPathStroker ps;
		ps.setWidth(kvadratik);

		QPainterPath path;
		path.moveTo(newTransform(linePorts[i]).p1());
		path.lineTo(newTransform(linePorts[i]).p2());

		path = ps.createStroke(path);
		if ( path.contains(location) )
			return ( 1.0 * ( i + pointPorts.size() ) + qMin( 0.9999,
				QLineF( QLineF(newTransform(linePorts[i])).p1(),location).length()
				/ newTransform(linePorts[i]).length() ) );
	}

	if (pointPorts.size()!=0) {
		int numMinDistance = 0;
		qreal minDistance = QLineF( pointPorts[0], transform.inverted().map(location) ).length();
		for( int i = 0; i < pointPorts.size(); i++ ) {
			if (QLineF( pointPorts[i], transform.inverted().map(location) ).length()<minDistance) {
				numMinDistance = i;
				minDistance = QLineF( pointPorts[i], transform.inverted().map(location) ).length();
			}
		}
		return 1.0 * numMinDistance;
	} else if (linePorts.size()!=0) {
		int numMinDistance = 0;
		qreal minDistance = QLineF( QLineF(linePorts[0]).p1(), transform.inverted().map(location) ).length();
		for( int i = 0; i < linePorts.size(); i++ ) {
			if (QLineF( QLineF(linePorts[i]).p1(), transform.inverted().map(location) ).length()<minDistance) {
				numMinDistance = i;
				minDistance = QLineF( QLineF(linePorts[i]).p1(), transform.inverted().map(location) ).length();
			}
		}
		return 1.0 * (numMinDistance + pointPorts.size());
	}

	return -1.0;
}

void NodeElement::setPortsVisible(bool value) {
	prepareGeometryChange();
	portsVisible = value;
}

void NodeElement::complexInlineEditing() {
	if ((docvis.toPlainText() == "") && (doctype.toPlainText() == "")){
	docvis.setTextWidth(0);
	doctype.setTextWidth(0);
	d.setPos(15, m_contents.height() - 15);
	d.setTextWidth(m_contents.width() - 25);
	} else
	if ((docvis.toPlainText() == "") && (doctype.toPlainText() != "")){
	docvis.setTextWidth(0);
	doctype.setPos(1, m_contents.height() - 15);
	if (typetext.length() * 5 < 6*m_contents.width() / 16)
		doctype.setTextWidth(typetext.length() * 5);
	else
		doctype.setTextWidth(6 * m_contents.width() / 16);

	d.setPos(doctype.textWidth(), m_contents.height() - 15);
	d.setTextWidth(m_contents.width() - doctype.textWidth() - 30);
	} else
	if ((docvis.toPlainText() != "") && (doctype.toPlainText() == "")){
	doctype.setTextWidth(0);
	docvis.setPos(1, m_contents.height() - 15);
	docvis.setTextWidth(11);
	d.setPos(16, m_contents.height() - 15);
	d.setTextWidth(m_contents.width() - 37);
	} else
	if ((docvis.toPlainText() != "") && (doctype.toPlainText() != "")){
	docvis.setPos(1, m_contents.height() - 15);
	docvis.setTextWidth(11);
	doctype.setPos (16, m_contents.height() - 15);
	if (typetext.length() * 5 < 6 * m_contents.width() / 16)
		doctype.setTextWidth(typetext.length() * 5);
	else
		doctype.setTextWidth(6 * m_contents.width() / 16);

	d.setPos(docvis.textWidth() + doctype.textWidth(),  m_contents.height() - 15);
	d.setTextWidth(m_contents.width() - doctype.textWidth() - 30);
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

			painter->drawRect(QRectF(m_contents.topLeft(),QSizeF(4,4)));
			painter->drawRect(QRectF(m_contents.topRight(),QSizeF(-4,4)));
			painter->drawRect(QRectF(m_contents.bottomLeft(),QSizeF(4,-4)));

			painter->translate(m_contents.bottomRight());
			painter->drawLine(QLineF(-4,0,0,-4));
			painter->drawLine(QLineF(-8,0,0,-8));
			painter->drawLine(QLineF(-12,0,0,-12));

			painter->restore();
		}
		if ((option->state & QStyle::State_MouseOver) || portsVisible)
		{
			painter->save();
			painter->setOpacity(0.7);
			portrenderer->render(painter,m_contents);
			painter->restore();
		}
	}
}
