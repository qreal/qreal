#pragma once

#include "../edgeElement.h"
#include "reshapeEdgeCommand.h"

namespace qReal {

class LineHandler : public QObject
{
	Q_OBJECT
public:
	LineHandler(EdgeElement *edge);
	virtual ~LineHandler() {}

	int startMovingEdge(QPointF const &pos);
	void rejectMovingEdge();
	void moveEdge(QPointF const &pos, bool needAlign);
	void endMovingEdge();

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

	virtual QPair<QPair<int, qreal>, qreal> arrangeCriteria(NodeElement const *node, QLineF const &portLine) const;

	virtual QList<ContextMenuAction *> extraActions(QPointF const &pos);

protected slots:
	void minimize();

protected:
	int addPoint(QPointF const &pos);

	virtual void improveAppearance();
	void deleteLoops();
	void deleteLoop(QPolygonF &line, int startPos);

	bool checkPort(QPointF const &pos, bool isStart) const;
	bool nodeChanged(bool isStart) const;
	int firstOutsidePoint(bool startFromSrc) const;
	void endReshape();

	int definePoint(QPointF const &pos) const;
	int defineSegment(QPointF const &pos) const;

	virtual void handleEdgeMove(QPointF const &pos, bool needAlign);
	virtual void drawPort(QPainter *painter, int portNumber);

	QPointF portArrangePoint(NodeElement const *node) const;

	virtual QPointF alignedPoint(QPointF const &point) const;

	EdgeElement *mEdge;

	QPolygonF mSavedLine;
	int mDragType;
	QPointF mDragStartPoint;

	commands::ReshapeEdgeCommand *mReshapeCommand;
	bool mReshapeStarted;
};

}
