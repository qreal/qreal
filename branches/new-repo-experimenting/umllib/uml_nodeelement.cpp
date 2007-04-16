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

void NodeElement::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    foreach (EdgeElement *edge, edgeList)
        edge->adjustLink();
    
    Element::mouseMoveEvent(event);
}

void NodeElement::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    moving = 1;
    
	QAbstractItemModel *im = const_cast<QAbstractItemModel *>(dataIndex.model());
    im->setData(dataIndex, pos(), Unreal::PositionRole);
    moving = 0;
    
    foreach (EdgeElement *edge, edgeList)
        edge->adjustLink();

    Element::mouseReleaseEvent(event);
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

void NodeElement::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    foreach (QPointF port, ports) {
        painter->save();
        painter->setOpacity(0.5);
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
