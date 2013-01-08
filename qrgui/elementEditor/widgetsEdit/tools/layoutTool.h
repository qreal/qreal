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
	Q_OBJECT

	Q_PROPERTY(int layoutLeftMargin READ layoutLeftMargin WRITE setLayoutLeftMargin USER true DESIGNABLE true)
	Q_PROPERTY(int layoutRightMargin READ layoutRightMargin WRITE setLayoutRightMargin USER true DESIGNABLE true)
	Q_PROPERTY(int layoutTopMargin READ layoutTopMargin WRITE setLayoutTopMargin USER true DESIGNABLE true)
	Q_PROPERTY(int layoutBottomMargin READ layoutBottomMargin WRITE setLayoutBottomMargin USER true DESIGNABLE true)

public:
	LayoutHelperFactory *layoutFactory() const;
	void setLayoutHelper(LayoutHelperBase *helper);

	virtual void removeChild(Tool *child);

	void startChildDrag(Tool *child);
	void finishChildDrag(bool success);

	virtual void generateXml(QDomElement &element, QDomDocument &document);
	virtual void deserializeWidget(QWidget *parent, const QDomElement &element);
	virtual void load(LayoutTool *parent, QDomElement const &element);
	static void deserializeAttachedProperty(QWidget *parent, QWidget *widget
		, QDomElement const &element);
	void loadAttachedProperty(Tool *child, QDomElement const &element);

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

	int layoutLeftMargin() const;
	int layoutRightMargin() const;
	int layoutTopMargin() const;
	int layoutBottomMargin() const;

	void setLayoutLeftMargin(int margin);
	void setLayoutRightMargin(int margin);
	void setLayoutTopMargin(int margin);
	void setLayoutBottomMargin(int margin);

	void invalidateLayoutMargin();

	bool mDraggingOver;
	QGraphicsColorizeEffect *mEffect;
	LayoutHelperFactory *mFactory;
	LayoutHelperBase *mHelper;

	int mLayoutLeftMargin;
	int mLayoutRightMargin;
	int mLayoutTopMargin;
	int mLayoutBottomMargin;
};

}
}
