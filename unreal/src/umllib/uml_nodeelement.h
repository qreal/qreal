#ifndef UML_NODEELEMENT_H
#define UML_NODEELEMENT_H

#include "uml_element.h"
#include "uml_edgeelement.h"

#include <QtGui/QWidget>

namespace UML {
    class NodeElement : public Element
    {
    public:
        NodeElement();
	~NodeElement();

        void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
	QRectF boundingRect() const;
	QRectF contentsRect() const;

	void addEdge(EdgeElement *edge) { edgeList << edge; };
	void delEdge(EdgeElement *edge) { edgeList.removeAt(edgeList.indexOf(edge)); };

	const QPointF getPort(int i) const;
	int getNearestPort(const QPointF location) const;

   protected:
        void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
	void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

    private:
	QList<EdgeElement *> edgeList;
	QList<QPointF> ports;
    };
};

#endif
