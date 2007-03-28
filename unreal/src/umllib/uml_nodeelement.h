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

        QPersistentModelIndex index() const { return dataIndex; };
	void setIndex(QPersistentModelIndex index) { dataIndex = index; updateData(); };

	void addEdge(EdgeElement *edge) { qDebug() << "addEdge" << edge->uuid(); edgeList << edge; };
	void delEdge(EdgeElement *edge) { qDebug() << "delEdge" << edge->uuid(); edgeList.removeAt(edgeList.indexOf(edge)); };

   protected:
        void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
	void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

    private:
	QList<EdgeElement *> edgeList;
    };
};

#endif
