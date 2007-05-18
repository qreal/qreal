#include <QtGui>

#include "uml_nodeelement.h"
#include "realreporoles.h"

using namespace UML;

NodeElement::NodeElement()
{
}

NodeElement::~NodeElement()
{
	foreach (EdgeElement *edge, edgeList)
		edge->removeLink(this);
}

void NodeElement::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	if ( isSelected() ) {

		if ( QRectF(m_contents.topLeft(),QSizeF(4,4)).contains(event->pos()) ) {
			dragState = TopLeft;
		} else if ( QRectF(m_contents.topRight(),QSizeF(-4,4)).contains(event->pos()) ) {
			dragState = TopRight;
		} else if ( QRectF(m_contents.bottomRight(),QSizeF(-4,-4)).contains(event->pos()) ) {
			dragState = BottomRight;
		} else if ( QRectF(m_contents.bottomLeft(),QSizeF(4,-4)).contains(event->pos()) ) {
			dragState = BottomLeft;
		} else 
			Element::mousePressEvent(event);	

	} else
		Element::mousePressEvent(event);
}

void NodeElement::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
	foreach (EdgeElement *edge, edgeList)
		edge->adjustLink();

	if ( dragState == None ) {
		Element::mouseMoveEvent(event);
	} else {
        if ( ( m_contents.width() < 10 ) || ( m_contents.height() < 10 ) ) {
			;
        } else {
                prepareGeometryChange();
                switch ( dragState ) {
                    case TopLeft:       m_contents.setTopLeft(event->pos());        break;
                    case Top:           m_contents.setTop(event->pos().y());        break;
                    case TopRight:      m_contents.setTopRight(event->pos());       break;
                    case Left:          m_contents.setLeft(event->pos().x());       break;
                    case Right:         m_contents.setRight(event->pos().x());      break;
                    case BottomLeft:    m_contents.setBottomLeft(event->pos());     break;
                    case Bottom:        m_contents.setBottom(event->pos().y());     break;
                    case BottomRight:   m_contents.setBottomRight(event->pos());    break;
					case None:														break;
                }
            
                setPos(pos() + m_contents.topLeft());
                m_contents.translate(-m_contents.topLeft());
        }
    }
}

void NodeElement::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	moving = 1;
	QAbstractItemModel *im = const_cast<QAbstractItemModel *>(dataIndex.model());
	im->setData(dataIndex, pos(), Unreal::PositionRole);
	moving = 0;

	foreach (EdgeElement *edge, edgeList)
		edge->adjustLink();

	if ( dragState != None )
		dragState = None;
	else
		Element::mouseReleaseEvent(event);

	m_contents = m_contents.normalized();
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
		setPos(dataIndex.data(Unreal::PositionRole).toPointF());

		foreach (EdgeElement *edge, edgeList)
			edge->adjustLink();
	}
}

const QPointF NodeElement::getPort(int i) const
{
	return ports[i];
}

int NodeElement::getNearestPort(const QPointF location) const
{
	int closest = 0;
	qreal minLength = 1000000;
	for (int i = 0; i < ports.size(); ++i) {
		qreal curLength = QLineF(ports.at(i),location).length();
		if ( curLength < minLength ) {
			minLength = curLength;
			closest = i;
		}
	}
	return closest;
}

void NodeElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget*)
{
	if ( option->levelOfDetail >= 0.5 ) {
		if ( option->state & QStyle::State_Selected ) {
			painter->save();
			painter->drawRect(m_contents);

			painter->setBrush(Qt::SolidPattern);
			painter->drawRect(QRectF(m_contents.topLeft(),QSizeF(4,4)));
			painter->drawRect(QRectF(m_contents.topRight(),QSizeF(-4,4)));
			painter->drawRect(QRectF(m_contents.bottomRight(),QSizeF(-4,-4)));
			painter->drawRect(QRectF(m_contents.bottomLeft(),QSizeF(4,-4)));
			painter->restore();
		}

		foreach (QPointF port, ports) {
			painter->save();
//			painter->setOpacity(0.5);
			painter->translate(port);
			painter->setBrush(Qt::gray);
			painter->setPen(Qt::NoPen);
			painter->drawRect(QRectF(-5,-5,10,10));
			painter->setPen(Qt::darkGray);
			painter->drawLine(QLineF(-5,-5,5,5));
			painter->drawLine(QLineF(-5,5,5,-5));
			painter->restore();
		}
	}
}
