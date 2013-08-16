#pragma once

#include "../edgeElement.h"
#include "reshapeEdgeCommand.h"

namespace qReal {

class LineHandler
{
public:
	LineHandler(EdgeElement *edge);

	void startMovingEdge(int dragType, QPointF const &pos);
	void rejectMovingEdge();
	virtual void moveEdge(QPointF const &pos, bool needAlign) = 0;
	void endMovingEdge();

	virtual void adjust();
	void layOut();

protected:
	int addPoint(QPointF const &pos);
	int defineSegment(QPointF const &pos);
	virtual void improveAppearance();
	bool checkPort(QPointF const &pos, bool isStart) const;
	bool nodeChanged(bool isStart) const;

	EdgeElement *mEdge;

	QPolygonF mSavedLine;
	int mDragType;
	QPointF mDragStartPoint;

	commands::ReshapeEdgeCommand *mReshapeCommand;
};

}
