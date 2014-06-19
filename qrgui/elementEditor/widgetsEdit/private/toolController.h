#pragma once

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QPointF>

#include <qrgui/elementEditor/widgetsEdit/tools/layoutHelpers/layoutTypes.h>
#include "propertyBrowserController.h"

namespace qReal
{
namespace widgetsEdit
{

class Tool;
class LayoutTool;
class PropertyBrowserController;

class ToolController : public QObject
{
	Q_OBJECT

public:
	ToolController();

	Tool *selectedItem() const;

	void addChild(Tool *child);
	void removeChild(Tool *child);
	void select(Tool *child);

	void markChildDragged(Tool *child);
	Tool *draggedChild() const;
	void unmarkChildDragged(bool success);

	void setLayout(Tool *tool, LayoutType type);

	void move(int dx, int dy);

	void setPropertyBrowserController(PropertyBrowserController *controller);

public slots:
	void processKeyEvent(QKeyEvent *event);

signals:
	void selectionChanged(Tool *newTool);
	void savePressed();
	void saveAsPressed();
	void openPressed();

private:
	void unselectAll();

	QList<Tool *> mItems;
	Tool *mSelectedChild;
	Tool *mDraggedChild;
	LayoutTool *mDraggedChildOldParent;
	PropertyBrowserController *mBrowserController;

};

}
}
