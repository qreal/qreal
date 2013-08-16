#include "curveLine.h"

namespace qReal {

CurveLine::CurveLine(EdgeElement *edge)
	: LineHandler(edge)
{
}

void CurveLine::moveEdge(QPointF const &pos, bool needAlign)
{
	Q_UNUSED(needAlign)
	QPolygonF line = mEdge->line();

	if (mDragType >= 0) {
		line[mDragType] = pos;
		mEdge->setLine(line);
	}

	mEdge->update();
}

}
