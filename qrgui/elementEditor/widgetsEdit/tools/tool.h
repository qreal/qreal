#pragma once

#include <QtCore/QDebug>
#include <QtXml/QDomElement>
#include <QtGui/QGraphicsProxyWidget>
#include <QtGui/QIcon>

#include "../private/toolController.h"

namespace Ui
{
namespace WidgetsEdit
{

int const resizeDrift = 5;

class ToolController;
class ToolPropertyManager;

class Tool : public QGraphicsProxyWidget
{
	Q_OBJECT

public:
	//getters for showing in tool list
	QIcon icon() const;
	QString title() const;

	bool movable() const;
	void setMovable(bool movable = true);
	void setResizable(bool resizable = true);
	void select();
	void unselect();
	bool selected() const;

	ToolPropertyManager *propertyManager() const;
	void setPropertyManager(ToolPropertyManager *manager);

	virtual void generateXml(QDomElement &element, QDomDocument &document);
	virtual void deserializeWidget(QWidget *parent, QDomElement const &element);

signals:
	void propertyChanged(QString const &name, QVariant const &value);

protected:
	enum DragState {
		None = 0
		, Moving
		, TopLeft
		, Top
		, TopRight
		, Left
		, Right
		, BottomLeft
		, Bottom
		, BottomRight
	};

	Tool(QWidget *widget, ToolController *controller
		, QGraphicsItem *parent = 0);

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option
		, QWidget *widget);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);

	virtual void moveEvent(QGraphicsSceneMoveEvent *event);
	virtual void resizeEvent(QGraphicsSceneResizeEvent *event);

	QString mTitle;
	QIcon mIcon;
	bool mMovable;
	bool mResizable;
	ToolController *mController;

private:
	QRectF resizeRect(DragState state);
	void drawFieldForResizeItem(QPainter* painter);
	DragState getDragState(QPointF pos);
	bool moving() const;
	bool resizing() const;
	void moveTool(QGraphicsSceneMouseEvent *event);
	void resizeTool(QGraphicsSceneMouseEvent *event);

	bool mSelected;
	QPointF mClickPos;
	DragState mDragState;
	ToolPropertyManager *mPropertyManager;

};

}
}
