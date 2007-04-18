#ifndef EDGE_H
#define EDGE_H

#include <QtGui/QColor>
#include <QtGui/QGraphicsItem>

#include <QtCore/QPersistentModelIndex>


class Element;
class EditorViewMViface;

class Edge : public QGraphicsItem
{
public:
    Edge(EditorViewMViface *parent);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
    
    QPersistentModelIndex index() const { return idx; };
    void setIndex(QPersistentModelIndex idx);

    void adjust();
    void updateData();
    
    QString toolTip() const;

protected:
//    void mousePressEvent ( QGraphicsSceneMouseEvent * event );
//    void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
//    void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    
    void keyPressEvent ( QKeyEvent * event );
    

private:
    Element *source, *dest;
    
    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize;
    
    QString text;
    int textLength;
    
    QPersistentModelIndex idx;

    EditorViewMViface *editor;
};

#endif
