#include <QtGui>

#include "uml_nodeelement.h"

using namespace UML;

NodeElement::NodeElement()
{
    ports << QPointF(70,0) << QPointF(70,170) << QPointF(0,85) << QPointF(140,85);
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
	qDebug() << curLength;
	if ( curLength < minLength ) {
	    minLength = curLength;
	    closest = i;
	}
    }
    qDebug() << "GetNearestPort" << closest;
    return closest;
}


void NodeElement::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
//    QPainterPath path;
//    path.moveTo(0,0);
//    path.lineTo(100,100);
//    path.moveTo(100,0);
//    path.lineTo(0,100);

//    painter->drawPath(path);
//    painter->drawRect(boundingRect());

    QString text("<center><img src=\":/images/kdevclassview/CVclass.png\" /><b>class UML::Element</b><hr>&nbsp;"
	"<img src=\":/images/kdevclassview/CVpublic_meth.png\" />setIndex()<br>"
	"<img src=\":/images/kdevclassview/CVpublic_meth.png\" />updateData()<br>"
	"<img src=\":/images/kdevclassview/CVpublic_meth.png\" />uuid()<hr>&nbsp;"
	"<img src=\":/images/kdevclassview/CVpublic_var.png\" />int m_uuid<br>"
	"<img src=\":/images/kdevclassview/CVpublic_var.png\" />QString name<br>"
	"</center>");
	
    painter->setBrush(QBrush(QColor(255,255,191,200)));
    painter->drawRect(contentsRect());
	
    QTextDocument d;
    d.setHtml(text);
    d.setTextWidth(contentsRect().width());
    d.drawContents(painter,contentsRect());

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

QRectF NodeElement::boundingRect() const
{
    return QRectF(-8,-8,156,186);
}

QRectF NodeElement::contentsRect() const
{
    return QRectF(0,0,140,170);
}
