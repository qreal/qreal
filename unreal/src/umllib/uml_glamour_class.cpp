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
    NodeElement::updateData();

    QString name = dataIndex.sibling(dataIndex.row(),1).data().toString();
    text = "<center><img src=\":/images/kdevclassview/CVclass.png\" /><b>class ";
    text += name;
    text += "</b><hr>&nbsp;";
    
    fields.clear();
    methods.clear();
    
    QString stuff = dataIndex.sibling(dataIndex.row(),7).data().toString();
    QStringList everything = stuff.split(";",QString::SkipEmptyParts);

    methods = everything.filter("()");
    fields = everything.filter("$");

    foreach (QString str, methods) {
	text += "<img src=\":/images/kdevclassview/CVpublic_meth.png\" />";
	text += str;
	text += "<br>\n";
    }
    
    text += "<hr>&nbsp;";

    foreach (QString str, fields) {
	text += "<img src=\":/images/kdevclassview/CVpublic_var.png\" />";
	text += str;
	text += "<br>\n";
    }
            
    text += "</center>";
    
    update();
    
}

void GlamourClass::contextMenuEvent ( QGraphicsSceneContextMenuEvent * event )
{
  QMenu menu;
  
  QAction *addMethAction = menu.addAction("Add Method");
  QAction *delMethAction = menu.addAction("Remove Method");
  
  QAction *addPropAction = menu.addAction("Add Property");
  QAction *delPropAction = menu.addAction("Remove Property");
  
  if ( QAction *selectedAction = menu.exec(event->screenPos()) ) {
        bool ok;

	if ( selectedAction == addMethAction ) {
	    QString text = QInputDialog::getText(0, QObject::tr("New Method"), QObject::tr("Method name:"), QLineEdit::Normal, "", &ok);
	    if (ok && !text.isEmpty())
	        methods << (text + "()");
	} else if ( selectedAction == delMethAction ) {
	    QString text = QInputDialog::getItem(0, QObject::tr("Remove Method"), QObject::tr("Method name:"), methods, 0, false, &ok);
	    if (ok && !text.isEmpty())
	        methods.removeAll(text);
        } else if ( selectedAction == addPropAction ) {
	    QString text = QInputDialog::getText(0, QObject::tr("New Field"), QObject::tr("Field name:"), QLineEdit::Normal, "", &ok);
	    if (ok && !text.isEmpty())
	        fields << ( QString("$") + text );
	} else if ( selectedAction == delPropAction ) {
	    QString text = QInputDialog::getItem(0, QObject::tr("Remove Field"), QObject::tr("Field name:"), fields, 0, false, &ok);
	    if (ok && !text.isEmpty())
	        fields.removeAll(text);
        }
  }
  
    QString result = (fields+methods).join(";");
    QAbstractItemModel *im = const_cast<QAbstractItemModel *>(dataIndex.model());
    im->setData(dataIndex.sibling(dataIndex.row(),7), result );
    
//    qDebug() << result;

  updateData();
}

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
