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

        virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
	virtual QRectF boundingRect() const;
	virtual QRectF contentsRect() const;
	
	virtual void updateData();
    protected:
	void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    private:
	QString text;
	
	QStringList methods, fields;
    };
};

#endif
