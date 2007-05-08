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

        virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
//	virtual QRectF boundingRect() const;
	virtual QRectF contentsRect() const = 0;
	
        virtual void updateData();

	void addEdge(EdgeElement *edge) { edgeList << edge; };
	void delEdge(EdgeElement *edge) { edgeList.removeAt(edgeList.indexOf(edge)); };

	const QPointF getPort(int i) const;
	int getNearestPort(const QPointF location) const;
	bool isChildOf(int id){ return parentsList.contains(id); }

   protected:
        void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
	void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

    QList<int> parentsList;

	QList<QPointF> ports;

    private:
	QList<EdgeElement *> edgeList;
    };
};

#endif
