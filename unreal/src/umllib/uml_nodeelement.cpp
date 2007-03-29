#include <QtGui>

#include "uml_nodeelement.h"

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
    foreach (EdgeElement *edge, edgeList)
        edge->adjustLink();

    Element::mouseReleaseEvent(event);
}

void NodeElement::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    QPainterPath path;
    path.moveTo(0,0);
    path.lineTo(100,100);
    path.moveTo(100,0);
    path.lineTo(0,100);

    painter->drawPath(path);
    painter->drawRect(boundingRect());
}

QRectF NodeElement::boundingRect() const
{
    return QRectF(0,0,100,100);
}
