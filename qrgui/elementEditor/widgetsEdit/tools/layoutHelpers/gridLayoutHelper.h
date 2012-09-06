#pragma once

#include <QtCore/QVector>
#include <QtGui/QGraphicsGridLayout>

#include "../layoutTool.h"
#include "layoutHelperBase.h"

namespace qReal
{
namespace widgetsEdit
{

class LayoutHelperBase;
class LayoutTool;

class GridLayoutHelper : public LayoutHelperBase
{
public:
	GridLayoutHelper(LayoutTool *tool);

	virtual void drawCurrentDropPosition(QPainter *painter);
	virtual void dropItem(Tool *item);

	virtual void resetLayout(QGraphicsLayout *layout);

	virtual void startChildDrag(Tool *item);
	virtual void finishChildDrag(bool success);

	virtual void generateAttachedProperty(QDomElement &element, Tool *tool);

protected:
	virtual void onToolResized(const QRectF &newGeometry);

private:
	QRectF determineCell();
	void determineRow();
	void determineColumn();
	void insertItem(Tool *item, int row, int column);
	void updateGrid();
	void updateRows();
	void updateColumns();
	QPair<int, int> findTool(Tool *tool) const;

	LayoutTool *mTool;
	QGraphicsGridLayout *mLayout;
	int mCurrentRow;
	int mCurrentColumn;
	QVector<int> mRows;
	QVector<int> mColumns;

	Tool *mDraggedChild;
	int mDraggedChildOldRow;
	int mDraggedChildOldColumn;

};

}
}
