#pragma once

#include "edgeElement.h"

namespace qReal {

class LineHandler
{
public:
	LineHandler(EdgeElement *edge, LineType type);

	void startMovingEdge(QPointF const &pos);
	void rejectMovingEdge();
	void moveEdge(QPointF const &pos, bool needAlign);
	void endMovingEdge();

	void setType(LineType type);

protected:
	int addPoint(QPointF const &pos);
	void moveSegment(QPointF const &pos);
	int defineSegment(QPointF const &pos);
	void createSquarePoints(QPointF const &pos);

	EdgeElement *mEdge;
	LineType mType;

	QPolygonF mSavedLine;
	int mDragType;
	QPointF mDragStartPoint;
};

}
