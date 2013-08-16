#pragma once

#include "edgeElement.h"
#include "private/reshapeEdgeCommand.h"

namespace qReal {

class LineHandler
{
public:
	LineHandler(EdgeElement *edge, LineType type);

	void startMovingEdge(int dragType, QPointF const &pos);
	void rejectMovingEdge();
	void moveEdge(QPointF const &pos, bool needAlign);
	void endMovingEdge();

	void adjust();
	void layOut();

	void setType(LineType type);

protected:
	int addPoint(QPointF const &pos);
	void moveSegment(QPointF const &pos);
	int defineSegment(QPointF const &pos);
	void improveAppearance();
	bool checkPort(QPointF const &pos, bool isStart) const;
	bool nodeChanged(bool isStart) const;

	EdgeElement *mEdge;
	LineType mType;

	QPolygonF mSavedLine;
	int mDragType;
	QPointF mDragStartPoint;

	commands::ReshapeEdgeCommand *mReshapeCommand;
};

}
