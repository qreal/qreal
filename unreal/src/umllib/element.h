#ifndef ELEMENT_H
#define ELEMENT_H

#include <QtGui/QColor>
#include <QtGui/QGraphicsItem>

class Edge;

class Element : public QGraphicsItem
{
public:
    Element();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
    
    void addEdge(Edge *e);
    
    void setInfo(QString type, QString name);

protected:
    void mousePressEvent ( QGraphicsSceneMouseEvent * event );
    void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
    void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    
private:
    QString type;
    QString name;
    
    QRectF mysize;    
    
    enum DragState { None, TopLeft, Top, TopRight, Left, Right, BottomLeft, Bottom, BottomRight };
    
    DragState dragState;
    
    QList<Edge *> edgeList;
    
    QString text;
};

#endif
