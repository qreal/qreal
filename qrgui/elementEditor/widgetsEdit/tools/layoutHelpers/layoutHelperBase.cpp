#include "layoutHelperBase.h"

using namespace Ui::WidgetsEdit;

LayoutHelperBase::LayoutHelperBase(LayoutTool *tool)
	: mTool(tool)
{
	connect(mTool, SIGNAL(geometryChanged()), this, SLOT(onGeometryChanged()));
}

void LayoutHelperBase::setDragPos(const QPointF &pos)
{
	mPos = pos;
}

void LayoutHelperBase::onGeometryChanged()
{
	onToolResized(mTool->geometry());
}
