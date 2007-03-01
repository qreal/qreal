#ifndef ELEMENT_H
#define ELEMENT_H

#include <QtGui/QColor>
#include <QtGui/QGraphicsItem>

#include <QtSvg/QSvgRenderer>

class Edge;

class Element : public QGraphicsItem
{
public:
    Element(const QString &type);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
    
    void addEdge(Edge *e);
    
    void setInfo(QString type, QString name);

protected:
    void keyPressEvent ( QKeyEvent * event );

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

    QSvgRenderer svgr;
};

#endif
