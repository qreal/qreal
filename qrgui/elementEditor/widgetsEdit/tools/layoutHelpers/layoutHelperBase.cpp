#include "layoutHelperBase.h"

using namespace qReal::widgetsEdit;

LayoutHelperBase::LayoutHelperBase(LayoutTool *tool)
	: mTool(tool)
{
}

void LayoutHelperBase::disconnectHelper()
{
	disconnect(mTool, SIGNAL(geometryChanged()), this, SLOT(onGeometryChanged()));
}

void LayoutHelperBase::setDragPos(const QPointF &pos)
{
	mPos = pos;
}

void LayoutHelperBase::onGeometryChanged()
{
	onToolResized(mTool->geometry());
}

void LayoutHelperBase::resetLayout(QGraphicsLayout *layout)
{
	Q_UNUSED(layout);
	connect(mTool, SIGNAL(geometryChanged()), this, SLOT(onGeometryChanged()));
}
