#pragma once

#include <QtCore/QDebug>
#include <QtXml/QDomElement>
#include <QtGui/QGraphicsProxyWidget>
#include <QtGui/QIcon>

#include "../private/toolController.h"

namespace qReal
{
namespace widgetsEdit
{

int const resizeDrift = 5;

class ToolController;
class PropertyManager;

class Tool : public QGraphicsProxyWidget
{
	Q_OBJECT

	Q_PROPERTY(QSize baseSize READ baseSize WRITE setBaseSize USER true)
	Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled USER true)
	Q_PROPERTY(QRect geometry READ widgetGeometry WRITE setWidgetGeometry USER true)
	Q_PROPERTY(int maximumHeight READ widgetMaximumHeight WRITE setWidgetMaximumHeight USER true)
	Q_PROPERTY(QSize maximumSize READ widgetMaximumSize WRITE setWidgetMaximumSize USER true)
	Q_PROPERTY(int maximumWidth READ widgetMaximumWidth WRITE setWidgetMaximumWidth USER true)
	Q_PROPERTY(int minimumHeight READ widgetMinimumHeight WRITE setWidgetMinimumHeight USER true)
	Q_PROPERTY(QSize minimumSize READ widgetMinimumSize WRITE setWidgetMinimumSize USER true)
	Q_PROPERTY(int minimumWidth READ widgetMinimumWidth WRITE setWidgetMinimumWidth USER true)
	Q_PROPERTY(QSize sizeIncrement READ sizeIncrement WRITE setSizeIncrement USER true)
	Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip USER true)

public:
	virtual ~Tool() {}

	//getters for showing in tool list
	QIcon icon() const;
	QString title() const;

	void raiseLoaded();

	bool movable() const;
	void setMovable(bool movable = true);
	void setResizable(bool resizable = true);
	void select();
	void unselect();
	bool selected() const;

	PropertyManager *propertyManager() const;

	virtual void generateXml(QDomElement &element, QDomDocument &document);
	virtual void deserializeWidget(QWidget *parent, QDomElement const &element);

signals:
	void loaded();
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

	// getters and setters redefinition for property editor
	QSize baseSize() const;
	QRect widgetGeometry() const;
	int widgetMaximumHeight() const;
	QSize widgetMaximumSize() const;
	int widgetMaximumWidth() const;
	int widgetMinimumHeight() const;
	QSize widgetMinimumSize() const;
	int widgetMinimumWidth() const;
	QSize sizeIncrement() const;
	QString toolTip() const;

	void setBaseSize(QSize const &size);
	void setWidgetGeometry(const QRect &rect);
	void setWidgetMaximumHeight(int height);
	void setWidgetMaximumSize(QSize const &size);
	void setWidgetMaximumWidth(int width);
	void setWidgetMinimumHeight(int height);
	void setWidgetMinimumSize(QSize const &size);
	void setWidgetMinimumWidth(int width);
	void setSizeIncrement(QSize const &size);
	void setToolTip(QString const &toolTip);

	bool mSelected;
	QPointF mClickPos;
	DragState mDragState;
	PropertyManager *mPropertyManager;

};

}
}
