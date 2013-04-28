#pragma once

#include <QtCore/QDebug>
#include <QtWidgets/QLayout>
#include <QtWidgets/QGraphicsColorizeEffect>

#include "tool.h"

namespace qReal
{
namespace widgetsEdit
{

int const gridSize = 10;

class LayoutHelperBase;
class LayoutHelperFactory;

class LayoutToolProxy : public ToolProxy
{
	Q_OBJECT

	Q_PROPERTY(int layoutLeftMargin READ layoutLeftMargin WRITE setLayoutLeftMargin
			NOTIFY leftMarginChanged USER true DESIGNABLE true)
	Q_PROPERTY(int layoutRightMargin READ layoutRightMargin WRITE setLayoutRightMargin
			NOTIFY rightMarginChanged USER true DESIGNABLE true)
	Q_PROPERTY(int layoutTopMargin READ layoutTopMargin WRITE setLayoutTopMargin
			NOTIFY topMarginChanged USER true DESIGNABLE true)
	Q_PROPERTY(int layoutBottomMargin READ layoutBottomMargin WRITE setLayoutBottomMargin
			NOTIFY bottomMarginChanged USER true DESIGNABLE true)

public:
	int layoutLeftMargin() const;
	int layoutRightMargin() const;
	int layoutTopMargin() const;
	int layoutBottomMargin() const;

	void invalidateLayoutMargin();

protected:
	explicit LayoutToolProxy(QWidget *widget);

signals:
	void leftMarginChanged(int newMargin);
	void rightMarginChanged(int newMargin);
	void topMarginChanged(int newMargin);
	void bottomMarginChanged(int newMargin);

private:
	void setLayoutLeftMargin(int margin);
	void setLayoutRightMargin(int margin);
	void setLayoutTopMargin(int margin);
	void setLayoutBottomMargin(int margin);

	int mLayoutLeftMargin;
	int mLayoutRightMargin;
	int mLayoutTopMargin;
	int mLayoutBottomMargin;

	QWidget *mWidget;
};

class LayoutTool : public Tool
{
	Q_OBJECT

public:
	LayoutHelperFactory *layoutFactory() const;
	void setLayoutHelper(LayoutHelperBase *helper);

	virtual void removeChild(Tool *child);
	virtual void onLoaded();

	void startChildDrag(Tool *child);
	void finishChildDrag(bool success);

	virtual void generateXml(QDomElement &element, QDomDocument &document);
	virtual void deserializeWidget(QWidget *parent, QDomElement const &element
			, QList<PropertyEditorInterface *> &editors);
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

private slots:
	void invalidateLayoutMargin();

private:
	void highlightDrag();
	void dehighlightDrag();
	void makeChildrenResizable(bool const resizable = true);

	bool mDraggingOver;
	QGraphicsColorizeEffect *mEffect;
	LayoutHelperFactory *mFactory;
	LayoutHelperBase *mHelper;
	LayoutToolProxy *mLayoutToolProxy;
};

}
}
