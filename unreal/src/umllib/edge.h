#ifndef EDGE_H
#define EDGE_H

#include <QtGui/QColor>
#include <QtGui/QGraphicsItem>

#include <QtCore/QPersistentModelIndex>


class Element;
class ExampleEditor;

class Edge : public QGraphicsItem
{
public:
    Edge(ExampleEditor *parent);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
    
    void setIndex(QPersistentModelIndex idx);

    void adjust();
    void updateData();

//protected:
//    void mousePressEvent ( QGraphicsSceneMouseEvent * event );
//    void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
//    void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    
private:
    Element *source, *dest;
    
    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize;
    
    QString text;
    
    QPersistentModelIndex idx;

    ExampleEditor *editor;
};

#endif
