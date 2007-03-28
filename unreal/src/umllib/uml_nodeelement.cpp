#include <QtGui>

#include "uml_nodeelement.h"

using namespace UML;

NodeElement::NodeElement()
{
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

    Element::mouseMoveEvent(event);
}

