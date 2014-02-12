#pragma once

#include <QtCore/QDebug>
#include <QtXml/QDomElement>
#include <QtWidgets/QGraphicsProxyWidget>
#include <QtGui/QIcon>

#include "../private/toolController.h"
#include "../propertyBinding/propertyProxyBase.h"
#include "../../../editorPluginInterface/propertyEditorInterface.h"

namespace qReal
{
namespace widgetsEdit
{

int const resizeDrift = 5;

class ToolController;
class PropertyManager;

class ToolProxy : public PropertyProxyBase
{
	Q_OBJECT

	Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled USER true DESIGNABLE true)
	Q_PROPERTY(QRect geometry READ widgetGeometry WRITE setWidgetGeometry USER true DESIGNABLE true)
	Q_PROPERTY(QSize maximumSize READ widgetMaximumSize WRITE setWidgetMaximumSize USER true DESIGNABLE true)
	Q_PROPERTY(QSize minimumSize READ widgetMinimumSize WRITE setWidgetMinimumSize USER true DESIGNABLE true)
	Q_PROPERTY(QSize sizeIncrement READ sizeIncrement WRITE setSizeIncrement USER true DESIGNABLE true)
	Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip USER true DESIGNABLE true)
	Q_PROPERTY(bool transparent READ isTransparent WRITE setTransparent USER true DESIGNABLE true)
	Q_PROPERTY(QSizePolicy sizePolicy READ sizePolicy WRITE setSizePolicy USER true DESIGNABLE true)

public:
	explicit ToolProxy(QWidget *widget);

	bool isEnabled() const;
	QRect widgetGeometry() const;
	QSize widgetMaximumSize() const;
	QSize widgetMinimumSize() const;
	QSize sizeIncrement() const;
	QString toolTip() const;
	bool isTransparent() const;
	QSizePolicy sizePolicy() const;

	void setEnabled(bool const enabled);
	void setWidgetGeometry(const QRect &rect);
	void setWidgetMaximumSize(QSize const &size);
	void setWidgetMinimumSize(QSize const &size);
	void setSizeIncrement(QSize const &size);
	void setToolTip(QString const &toolTip);
	void setTransparent(bool const transparent);
	void setSizePolicy(QSizePolicy const &policy);

signals:
	void maximumSizeChanged(QSize const &size);
	void minimumSizeChanged(QSize const &size);

private:
	QWidget *mWidget;
	QRect mOldGeometry;
};

class Tool : public QGraphicsProxyWidget
{
	Q_OBJECT

public:
	virtual ~Tool() {}

	QIcon icon() const;
	QString title() const;
	QString tag() const;

	virtual void onLoaded();

	bool movable() const;
	void setMovable(bool movable = true);
	void setResizable(bool resizable = true);
	void select();
	void unselect();
	bool selected() const;

	PropertyManager *propertyManager() const;
	PropertyProxyBase *propertyProxy() const;

	virtual void generateXml(QDomElement &element, QDomDocument &document);
	virtual void deserializeWidget(QWidget *parent, QDomElement const &element
			, QList<PropertyEditorInterface *> &editors);
	virtual void load(LayoutTool *parent, QDomElement const &element);

	virtual void removeChild(Tool *child);

signals:
	void loaded();
	void propertyChanged(QString const &name, QVariant const &value);

protected:
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
	QString mTag;
	QIcon mIcon;
	bool mMovable;
	bool mResizable;
	ToolController *mController;
	ToolProxy *mProxy;

private slots:
	void syncMaximumSize(QSize const &size);
	void syncMinimumSize(QSize const &size);

private:
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
	PropertyManager *mPropertyManager;

};

}
}
