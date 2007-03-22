#ifndef ELEMENT_H
#define ELEMENT_H

#include <QtGui/QColor>
#include <QtGui/QGraphicsItem>

#include <QtCore/QPersistentModelIndex>

#include <QtSvg/QSvgRenderer>

class Edge;
class ExampleEditor;

class Element : public QGraphicsItem
{
public:
    Element(ExampleEditor *parent);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
    
    void addEdge(Edge *e);
    
    void setIndex(QPersistentModelIndex idx);
    
    void updateData();

protected:
    void keyPressEvent ( QKeyEvent * event );

    void mousePressEvent ( QGraphicsSceneMouseEvent * event );
    void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
    void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    
private:
    QRectF mysize;    
    
    enum DragState { None, TopLeft, Top, TopRight, Left, Right, BottomLeft, Bottom, BottomRight };
    
    DragState dragState;
    
    QList<Edge *> edgeList;
    
    QString text;

    QSvgRenderer svgr;
    
    int updateno;
    
    ExampleEditor *editor;
    
    QPersistentModelIndex idx;
};

#endif
