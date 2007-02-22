#include "edge.h"
#include "element.h"

#include <QtGui>

#include <math.h>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;


Edge::Edge()
{
//    setFlags(ItemIsSelectable);
    source = dest = 0;
    text = "This is a Link";
}

void Edge::setSource(Element *source)
{
    this->source = source;
    source->addEdge(this);
}

void Edge::setDest(Element *dest)
{
    this->dest = dest;
    dest->addEdge(this);
}

void Edge::setText(const QString &text)
{
    this->text = text;
    update();
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

QRectF Edge::boundingRect() const
{
    if (!source || !dest)
	return QRectF();
	   
   qreal penWidth = 1;
   qreal arrowSize = 10;
   qreal extra = (penWidth + arrowSize) / 2.0;

   return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                     destPoint.y() - sourcePoint.y()))
             .normalized()
             .adjusted(-extra, -extra, extra, extra);
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    if (!source || !dest)
            return;

    double penWidth = 1;
    	    
    if (option->state & QStyle::State_Selected)
	penWidth += 2;

    // Draw the line itself
    QLineF line(sourcePoint, destPoint);
    painter->setPen(QPen(Qt::black, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);
    
    
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
    painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);


    int textLength = painter->fontMetrics().width(text);
    if (line.length() > textLength + 10) {
	painter->save();
	painter->translate(line.pointAt(1));
	painter->rotate(180-(angle*360)/TwoPi);
        painter->drawText((line.length()-textLength)/2,-3,text);
	painter->restore();
    }
    
//    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);

}
