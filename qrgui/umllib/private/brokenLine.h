#pragma once

#include "umllib/private/lineHandler.h"

namespace qReal {

/// @brief A strategy class for handling custom broken link
/// User may move points of such link, create new points, delete points and segments
class BrokenLine : public LineHandler
{
	Q_OBJECT
public:
	BrokenLine(EdgeElement *edge);

	/// Align every non-end point of the link to grid
	virtual void alignToGrid();

	/// @return List of context menu actions that may be done on a link at the point pos
	virtual QList<ContextMenuAction *> extraActions(QPointF const &pos);

protected slots:
	/// Delete non-end point at position pos (given in link's coordinates), if any
	void deletePoint(QPointF const &pos);

	/// Delete non-end segment at position pos (given in link's coordinates), if any
	void deleteSegment(QPointF const &pos);

protected:
	/// Move point to position pos (given in link's coordinates)
	/// Point is determined earlier (when mouse button is pressed)
	/// If there is no point make the one then move it
	virtual void handleEdgeMove(QPointF const &pos);

	/// Add a new point at position pos, if there is a link's segment containing pos
	int addPoint(QPointF const &pos);

	/// Align point to the nearest grid corner
	QPointF alignedPoint(QPointF const &point) const;

	/// Remove unneeded points, lines and loops (if the link intersects itself)
	virtual void improveAppearance();

	/// Remove points and lines that are so close to each other that look like single point or line
	void deleteUnneededPoints();

	/// If two or more points are too close to each other replace them with just one point
	void deleteClosePoints(QPolygonF &line) const;

	/// Remove one of two lines if they are very close to each other
	void removeSmallTriangles(QPolygonF &line) const;

	/// Determine whether points of line at positions i - i+2 look like one line rather than two
	bool tooSmallTriangle(QPolygonF const &line, int i) const;

	/// @return true if pos is close enough to non-end point
	bool delPointActionIsPossible(QPointF const &pos) const;

	/// @return true if pos is close enough to non-end segment
	bool delSegmentActionIsPossible(QPointF const &pos);

	/// @return true if link consists of more than 2 points
	bool minimizeActionIsPossible() const;

	ContextMenuAction mDeletePointAction;
	ContextMenuAction mDeleteSegmentAction;
	ContextMenuAction mMinimizeAction;
};

}
