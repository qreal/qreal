#pragma once

#include "../edgeElement.h"
#include "reshapeEdgeCommand.h"

namespace qReal {

class LineHandler
{
public:
	LineHandler(EdgeElement *edge);

	virtual void startMovingEdge(int dragType, QPointF const &pos);
	void rejectMovingEdge();
	virtual void moveEdge(QPointF const &pos, bool needAlign);
	virtual void endMovingEdge();

	virtual void adjust();
	virtual void alignToGrid();
	void connectAndArrange(bool reconnectSrc, bool reconnectDst);
	virtual void reconnect(bool reconnectSrc, bool reconnectDst);
	virtual void layOut(bool needReconnect = true);

	virtual void drawLine(QPainter *painter, bool drawSavedLine);
	virtual void drawPorts(QPainter *painter);
	virtual QPainterPath shape() const;

	QPolygonF savedLine() const;
	bool isReshapeStarted() const;

	virtual QPointF portArrangePoint(bool startFromSrc) const;

protected:
	int addPoint(QPointF const &pos);
	int defineSegment(QPointF const &pos);
	virtual void improveAppearance();
	bool checkPort(QPointF const &pos, bool isStart) const;
	bool nodeChanged(bool isStart) const;
	int firstOutsidePoint(bool startFromSrc) const;
	void endReshape();

	virtual void drawPort(QPainter *painter, int portNumber);

	virtual QPointF alignedPoint(QPointF const &point) const;

	EdgeElement *mEdge;

	QPolygonF mSavedLine;
	int mDragType;
	QPointF mDragStartPoint;

	commands::ReshapeEdgeCommand *mReshapeCommand;
	bool mReshapeStarted;
};

}
