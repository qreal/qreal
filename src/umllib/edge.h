#ifndef EDGE_H
#define EDGE_H

#include <QtGui/QColor>
#include <QtGui/QGraphicsItem>

class Element;

class Edge : public QGraphicsItem
{
public:
    Edge();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
    
    void setSource(Element *source);
    void setDest(Element *dest);
    
    void adjust();

//protected:
//    void mousePressEvent ( QGraphicsSceneMouseEvent * event );
//    void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
//    void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    
private:
    Element *source, *dest;
    
    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize;
};

#endif
