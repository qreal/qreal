#pragma once

#include <QtCore/QDebug>
#include <QtGui/QLayout>
#include <QtGui/QGraphicsColorizeEffect>

#include "tool.h"

namespace qReal
{
namespace widgetsEdit
{

int const gridSize = 10;

class LayoutHelperBase;
class LayoutHelperFactory;

class LayoutTool : public Tool
{
public:
	LayoutHelperFactory *layoutFactory() const;
	void setLayoutHelper(LayoutHelperBase *helper);

	virtual void removeChild(Tool *child);

	void startChildDrag(Tool *child);
	void finishChildDrag(bool success);

	virtual void generateXml(QDomElement &element, QDomDocument &document);
	virtual void deserializeWidget(QWidget *parent, const QDomElement &element);
	static void deserializeAttachedProperty(QWidget *parent, QWidget *widget,
											const QDomElement &element);

protected:
	LayoutTool(QWidget *widget, ToolController *controller
		, QGraphicsItem *parent = 0);

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option
		, QWidget *widget);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
	virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
	virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
	virtual void dropEvent(QGraphicsSceneDragDropEvent *event);


private:
	void highlightDrag();
	void dehighlightDrag();
	void makeChildrenResizable(bool const resizable = true);

	bool mDraggingOver;
	QGraphicsColorizeEffect *mEffect;
	LayoutHelperFactory *mFactory;
	LayoutHelperBase *mHelper;

};

}
}
