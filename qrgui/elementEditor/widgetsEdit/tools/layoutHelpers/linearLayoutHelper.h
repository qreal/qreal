#pragma once

#include <QtWidgets/QGraphicsLinearLayout>

#include "../layoutTool.h"
#include "layoutHelperBase.h"

namespace qReal
{
namespace widgetsEdit
{

class LayoutHelperBase;
class LayoutTool;

class LinearLayoutHelper : public LayoutHelperBase
{
public:
	LinearLayoutHelper(LayoutTool *tool, Qt::Orientation const orientation);

	virtual void drawCurrentDropPosition(QPainter *painter);
	virtual void dropItem(Tool *item);

	virtual void resetLayout(QGraphicsLayout *layout);

	virtual void startChildDrag(Tool *item);
	virtual void finishChildDrag(bool success);

	virtual void generateAttachedProperty(QDomElement &element, Tool *tool);

	void insertTool(int index, Tool *child);

protected:
	virtual void onToolResized(const QRectF &newGeometry);

private:
	void insertItem(int index, Tool *item);
	int findTool(Tool *tool) const;

	LayoutTool *mTool;
	QGraphicsLinearLayout *mLayout;
	Qt::Orientation mOrientation;
	int mCurrentSell;

	Tool *mDraggedChild;
	int mDraggedChildOldSell;

};

}
}
