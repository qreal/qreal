#pragma once

#include "umllib/private/lineHandler.h"

namespace qReal {

class BrokenLine : public LineHandler
{
	Q_OBJECT
public:
	BrokenLine(EdgeElement *edge);

	virtual void alignToGrid();

	virtual QList<ContextMenuAction *> extraActions(QPointF const &pos);

protected slots:
	void deletePoint(QPointF const &pos);
	void deleteSegment(QPointF const &pos);

protected:
	virtual void handleEdgeMove(QPointF const &pos);
	int addPoint(QPointF const &pos);
	QPointF alignedPoint(QPointF const &point) const;

	virtual void improveAppearance();
	void deleteUnneededPoints();
	void deleteClosePoints(QPolygonF &line) const;
	void removeSmallTriangles(QPolygonF &line) const;
	bool tooSmallTriangle(QPolygonF const &line, int i) const;

	bool delPointActionIsPossible(QPointF const &pos) const;
	bool delSegmentActionIsPossible(QPointF const &pos);
	bool minimizeActionIsPossible() const;

	ContextMenuAction mDeletePointAction;
	ContextMenuAction mDeleteSegmentAction;
	ContextMenuAction mMinimizeAction;
};

}
