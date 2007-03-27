#include "edge.h"
#include "element.h"

#include "editorviewmvciface.h"

#include <QtGui>

#include <math.h>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;


Edge::Edge(EditorViewMViface *parent)
{
    setFlags(ItemIsSelectable | ItemIsFocusable);
    source = dest = 0;
    text = "";
    textLength = 0;
    
    editor = parent;
}

void Edge::adjust()
{
    if (!source || !dest)
	return;

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();
    QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);

    removeFromIndex();
    sourcePoint = line.p1() + edgeOffset;
    destPoint = line.p2() - edgeOffset;
    addToIndex();
}

void Edge::setIndex(QPersistentModelIndex idx)
{
    this->idx = idx;
    updateData();
}	

void Edge::updateData()
{
    // maybe move elsewhere?
    int myrow = idx.row();
	
    text = idx.sibling(myrow,1).data().toString();;
    
    int uuidFrom = idx.sibling(myrow,6).data().toInt();
    int uuidTo = idx.sibling(myrow,7).data().toInt();
    
    source = qgraphicsitem_cast<Element *>(editor->getItem(uuidFrom));
    if (source)
	source->addEdge(this);

    dest = qgraphicsitem_cast<Element *>(editor->getItem(uuidTo));
    if (dest)
	dest->addEdge(this);
				    
    update();
}

void Edge::keyPressEvent ( QKeyEvent * event )
{
    if ( event->key() == Qt::Key_Backspace )
        text.chop(1);
    else if ( ( event->key() == Qt::Key_Enter ) || ( event->key() == Qt::Key_Return ) ) {
	QAbstractItemModel *im = const_cast<QAbstractItemModel *>(idx.model());
	im->setData(idx.sibling(idx.row(),1), text );
    } else
        text += event->text();

    update();
}

QPainterPath Edge::shape() const
{
    QPainterPath path;
    
    path.moveTo(sourcePoint+QPointF(1,1));
    path.lineTo(sourcePoint+QPointF(-1,-1));

    path.lineTo(destPoint+QPointF(1,1));
    path.lineTo(destPoint+QPointF(-1,-1));
    path.closeSubpath();

    return path;	
}

QString Edge::toolTip() const
{
    return text;
}

QRectF Edge::boundingRect() const
{
    if (!source || !dest)
	return QRectF();
	   
   qreal penWidth = 1;
   qreal arrowSize = 10;
   qreal extra = (penWidth + arrowSize) / 2.0 + 20;

   return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                     destPoint.y() - sourcePoint.y()))
             .normalized()
             .adjusted(-extra, -extra, extra, extra);
}


void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    if (!source || !dest)
            updateData();
    if (!source || !dest)
            return;

    double penWidth = 1;
    	    
    if (option->state & QStyle::State_Selected)
	penWidth += 2;

    // Draw the line itself
    QLineF line(sourcePoint, destPoint);
    painter->setPen(QPen(Qt::black, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);
//    painter->drawPath(shape());
    
    
    double arrowSize = 10.0; 
    
    // Draw the arrows if there's enough room
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
            angle = TwoPi - angle;
		    
    QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + Pi / 3) * arrowSize,
			    cos(angle + Pi / 3) * arrowSize);
    QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                            cos(angle + Pi - Pi / 3) * arrowSize);
    QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
                            cos(angle - Pi / 3) * arrowSize);
    QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
                            cos(angle - Pi + Pi / 3) * arrowSize);
																												    
    painter->setBrush(Qt::black);
//    painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);


    textLength = painter->fontMetrics().width(text);
    if (line.length() > textLength + 10) {
	painter->save();
	painter->translate(line.pointAt(1));
	painter->rotate(180-(angle*360)/TwoPi);
        painter->drawText((line.length()-textLength)/2,-3,text);
	painter->restore();
    }
    
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);

}
