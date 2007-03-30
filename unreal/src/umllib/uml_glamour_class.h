#ifndef UML_GLAMOUR_CLASS_H
#define UML_GLAMOUR_CLASS_H

#include "uml_nodeelement.h"

#include <QtGui/QWidget>

namespace UML {
    class GlamourClass : public NodeElement
    {
    public:
        GlamourClass();
	~GlamourClass();

        void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
	QRectF boundingRect() const;
	QRectF contentsRect() const;
    };
};

#endif
