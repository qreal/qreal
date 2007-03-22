#include "element.h"
#include "edge.h"

#include "exampleeditor.h"

#include <QtGui>
#include <QtSvg>

Element::Element(ExampleEditor *parent)
    : mysize(0,0,75,75)
{
    setFlags(ItemIsSelectable | ItemIsMovable | ItemIsFocusable);
    setAcceptsHoverEvents(true);
    setCursor(Qt::SizeAllCursor);
    
    dragState = None;
    
    editor = parent;
    
    updateno = 0;
}

void Element::setIndex(QPersistentModelIndex idx)
{
    this->idx = idx;
    updateData();
}

void Element::updateData()
{
    // maybe move elsewhere?
    int myrow = idx.row();
    
    text = QString("el row # %1 upd %2<hr>").arg(idx.row()).arg(updateno++);
    
    for (int col = 0; col <= 11; col++) {
	text += idx.sibling(myrow,col).data().toString();
	text += "<br>";
    }
    
//    QString type = idx.sibling(myrow,1).data().toString();
//    svgr.load(QString("../shapes/%1.svg").arg(type));

    update();
}

void Element::addEdge(Edge *e)
{
    edgeList << e;
    e->adjust();
}

QRectF Element::boundingRect() const
{
    return mysize.adjusted(-10,-10,10,10);
}

void Element::keyPressEvent ( QKeyEvent * event )
{
    if ( event->key() == Qt::Key_Backspace )
	text.chop(1);
    else
        text += event->text();
    update();
}

void Element::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    if ( QRectF(mysize.topLeft(),QSizeF(4,4)).contains(event->pos()) ) {
	dragState = TopLeft;
    } else if ( QRectF(mysize.topRight(),QSizeF(-4,4)).contains(event->pos()) ) {
	dragState = TopRight;
    } else if ( QRectF(mysize.bottomRight(),QSizeF(-4,-4)).contains(event->pos()) ) {
	dragState = BottomRight;
    } else if ( QRectF(mysize.bottomLeft(),QSizeF(4,-4)).contains(event->pos()) ) {
	dragState = BottomLeft;
    } else 
	QGraphicsItem::mousePressEvent(event);
}

void Element::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    if ( dragState != None ) {
	dragState = None;
    } else 
	QGraphicsItem::mouseReleaseEvent(event);
	
    mysize = mysize.normalized();
}

void Element::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    if ( dragState == None ) {
        QGraphicsItem::mouseMoveEvent(event);
    } else {
	if ( ( mysize.width() < 10 ) || ( mysize.height() < 10 ) ) {
	} else {
		prepareGeometryChange();
		switch ( dragState ) {
		    case TopLeft:	mysize.setTopLeft(event->pos());	break;
		    case Top:		mysize.setTop(event->pos().y());	break;
		    case TopRight:	mysize.setTopRight(event->pos());	break;
		    case Left:		mysize.setLeft(event->pos().x());	break;
		    case Right:		mysize.setRight(event->pos().x());	break;
		    case BottomLeft:	mysize.setBottomLeft(event->pos());	break;
		    case Bottom:	mysize.setBottom(event->pos().y());	break;
		    case BottomRight:	mysize.setBottomRight(event->pos());	break;
		}
	    
		setPos(pos() + mysize.topLeft());
		mysize.translate(-mysize.topLeft());
	}
    }
    foreach (Edge *edge, edgeList)
        edge->adjust();
	
}

void Element::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    
//    painter->drawEllipse(-2,-2,4,4);
    
    QPen oldPen = painter->pen();
    QPen pen = oldPen;
    int width = 0;
//    if (option->state & QStyle::State_MouseOver)
//	    painter->drawRect(boundingRect());

    painter->setBrush(QBrush(Qt::white));
    painter->drawRect(mysize);
    
    QTextDocument d;
    d.setHtml(text);
    d.setTextWidth(mysize.width()*2);
    d.drawContents(painter,mysize);
    
    if (option->state & QStyle::State_Selected) {
	    painter->setBrush(Qt::SolidPattern);
	    painter->drawRect(QRectF(mysize.topLeft(),QSizeF(4,4)));
	    painter->drawRect(QRectF(mysize.topRight(),QSizeF(-4,4)));
	    painter->drawRect(QRectF(mysize.bottomRight(),QSizeF(-4,-4)));
	    painter->drawRect(QRectF(mysize.bottomLeft(),QSizeF(4,-4)));
    }
    
//    painter->drawEllipse(0,0,50,50);
    svgr.render(painter,mysize);

}
