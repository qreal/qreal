#ifndef UML_EDGEELEMENT_H
#define UML_EDGEELEMENT_H

#include "uml_element.h"

QPainterPath qt_graphicsItem_shapeFromPath(const QPainterPath &path, const QPen &pen);

namespace UML {

    enum ArrowType { FILLED_ARROW, EMPTY_ARROW, FILLED_RHOMB, EMPTY_RHOMB };
	
    class NodeElement;
	class EdgeElement : public Element
	{
		public:
			EdgeElement();
			~EdgeElement();

			void updateData();

			QRectF boundingRect() const;
			QPainterPath shape() const;
			void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

			void adjustLink();
			void removeLink(UML::NodeElement *from) { if (src == from) { src = 0; }; if (dst == from) { dst = 0; }; };

		private:
			int getPoint( const QPointF &location );
			NodeElement *getNodeAt( const QPointF &position );
//			void checkConnection();
			NodeElement *src, *dst;
			//	QPointF srcPoint, dstPoint;

			qreal portFrom, portTo;
			int dragState;

			int longPart;

			QPolygonF m_line;
			QColor m_color;

			void updateLongestPart();
		protected:
			void mousePressEvent ( QGraphicsSceneMouseEvent * event );
			void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
			void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
			void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );

			virtual void drawStartArrow ( QPainter * ) const = 0;
			virtual void drawEndArrow ( QPainter * ) const = 0;

			Qt::PenStyle m_penStyle;
			QString m_text;
            ArrowType m_startArrowStyle;
            ArrowType m_endArrowStyle;
	};
};

#endif
