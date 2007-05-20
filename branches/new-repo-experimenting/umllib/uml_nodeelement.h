#ifndef UML_NODEELEMENT_H
#define UML_NODEELEMENT_H

#include "uml_element.h"
#include "uml_edgeelement.h"

#include <QtGui/QWidget>

const int kvadratik = 5;

namespace UML {
	class NodeElement : public Element
	{
		public:
			NodeElement();
			~NodeElement();

			virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
			QRectF boundingRect() const;
			QRectF contentsRect() const;

			virtual void updateData();

			const QPointF getPort(int i) const;
			int getNearestPort(const QPointF &location) const;
			bool isChildOf(int id){ return parentsList.contains(id); }

			void addEdge(EdgeElement *edge) { edgeList << edge; };
			void delEdge(EdgeElement *edge) { edgeList.removeAt(edgeList.indexOf(edge)); };

		protected:
//			void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
//			void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
//			void mousePressEvent( QGraphicsSceneMouseEvent * event );

			virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

			QList<int> parentsList;

			QList<QPointF> ports;
			
			QRectF m_contents;
		private:
			QList<EdgeElement *> edgeList;

			enum DragState { None, TopLeft, Top, TopRight, Left, Right, BottomLeft, Bottom, BottomRight };
			DragState dragState;
	};
};

#endif
