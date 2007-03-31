#include <QtGui>

#include "uml_glamour_class.h"

using namespace UML;

GlamourClass::GlamourClass()
{
    ports << QPointF(70,0) << QPointF(70,170) << QPointF(0,85) << QPointF(140,85);

/*    QString text("<center><img src=\":/images/kdevclassview/CVclass.png\" /><b>class UML::Element</b><hr>&nbsp;\n"
	"<img src=\":/images/kdevclassview/CVpublic_meth.png\" />setIndex()<br>\n"
	"<img src=\":/images/kdevclassview/CVpublic_meth.png\" />updateData()<br>\n"
	"<img src=\":/images/kdevclassview/CVpublic_meth.png\" />uuid()<hr>&nbsp;\n"
	"<img src=\":/images/kdevclassview/CVpublic_var.png\" />int m_uuid<br>\n"
	"<img src=\":/images/kdevclassview/CVpublic_var.png\" />QString name<br>\n"
	"</center>"); */
}

GlamourClass::~GlamourClass()
{
}

void GlamourClass::updateData()
{
    QString name = dataIndex.sibling(dataIndex.row(),1).data().toString();
    text = "<center><img src=\":/images/kdevclassview/CVclass.png\" /><b>class ";
    text += name;
    text += "</b><hr><hr>&nbsp;\n";
    
    QString stuff = dataIndex.sibling(dataIndex.row(),7).data().toString();

    fields = stuff.split(";"); // boyan ;)
    
    foreach (QString str, fields) {
	text += "<img src=\":/images/kdevclassview/CVpublic_var.png\" />";
	text += str;
	text += "<br>\n";
    }
            
    text += "</center>";
    
    update();
    
    NodeElement::updateData();
}

//void GlamourClass::contextMenuEvent ( QGraphicsSceneContextMenuEvent * event )
//{
//  QMenu menu;
//  QAction *removeAction = menu.addAction("Remove");
//  QAction *markAction = menu.addAction("Mark");
//  QAction *selectedAction = menu.exec(event->screenPos());
//}

void GlamourClass::paint(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *widget)
{
//    QPainterPath path;
//    path.moveTo(0,0);
//    path.lineTo(100,100);
//    path.moveTo(100,0);
//    path.lineTo(0,100);

//    painter->drawPath(path);
//    painter->drawRect(boundingRect());

    painter->setBrush(QBrush(QColor(255,255,191,200)));
    painter->drawRect(contentsRect());
	
    QTextDocument d;
    d.setHtml(text);
    d.setTextWidth(contentsRect().width());
    d.drawContents(painter,contentsRect());

    NodeElement::paint(painter, style, widget);
}

QRectF GlamourClass::boundingRect() const
{
    return QRectF(-8,-8,156,186);
}

QRectF GlamourClass::contentsRect() const
{
    return QRectF(0,0,140,170);
}
