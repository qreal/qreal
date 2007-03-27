#ifndef UML_EDGEELEMENT_H
#define UML_EDGEELEMENT_H

#include "uml_element.h"

QPainterPath qt_graphicsItem_shapeFromPath(const QPainterPath &path, const QPen &pen);

namespace UML {
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
    protected:
        void mousePressEvent ( QGraphicsSceneMouseEvent * event );
        void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
        void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

    private:
	NodeElement *src, *dst;
	QPointF srcPoint, dstPoint;
    
	int dragState;
    };
};

#endif
