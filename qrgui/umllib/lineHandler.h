#pragma once

#include "edgeElement.h"

namespace qReal {

class LineHandler
{
public:
	LineHandler(EdgeElement *edge, LineType type);

	void startMovingEdge(int dragType, QPointF const &pos);
	void rejectMovingEdge();
	void moveEdge(QPointF const &pos, bool needAlign);
	void endMovingEdge();

protected:
	EdgeElement *mEdge;
	LineType mType;

	QPolygonF mSavedLine;
	int mDragType;
};

}
