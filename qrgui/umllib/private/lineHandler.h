#pragma once

#include "umllib/edgeElement.h"
#include "umllib/private/reshapeEdgeCommand.h"

namespace qReal {

/// Base class for classes representing different link types (Strategy pattern).
/// Performs mouse events handling, link rendering and everything connected to laying the link out, reconnecting
/// and adjusting of the link to adjacent nodes and grid.
/// Apart of that, LineHandler's subclass may add extra actions to link's context menu.
/// To support undo-redo for custom context actions use connectAction() function instead of QObject::connect().
class LineHandler : public QObject
{
	Q_OBJECT
public:
	LineHandler(EdgeElement *edge);
	virtual ~LineHandler() {}

	/// Start reshape links, determine whether user tries to move point, segment or port of the link
	int startMovingEdge(QPointF const &pos);

	/// Cancel link move
	void rejectMovingEdge();

	/// For non-loop link delegate move processing to type-specific line handler, otherwise do nothing
	void moveEdge(QPointF const &pos);

	/// Reconnect, lay out and execute reshape command
	void endMovingEdge();

	/// Adjust link to make its' ends be placed exactly on corresponding ports
	virtual void adjust();

	/// Align link to grid in accordance with its' type
	virtual void alignToGrid();

	/// Connect link to port, arrange linear ports of adjacent nodes, make type-dependent appearance enhancements
	void layOut(bool needReconnect = true);

	/// Reconnect link in accordance with its' type. Default implementation excludes intersections by reconnecting
	/// link to points not contained in src/dst bounding rectangles
	virtual void reconnect(bool reconnectSrc, bool reconnectDst);

	/// @return criteria for sorting links on linear ports (depends on concrete link type)
	virtual EdgeArrangeCriteria arrangeCriteria(NodeElement const *node, QLineF const &portLine) const;

	/// Draw link line (depending on a link type, default implementation draw link's line() polygon)
	/// If drawSavedLine is true, method draw old (before reshape) link's configuration
	virtual void drawLine(QPainter *painter, bool drawSavedLine);

	/// Draw link points one by one with the given painter
	virtual void drawPorts(QPainter *painter);

	/// @return link shape (depending on a link type, default implementation returns link's line() polygon)
	virtual QPainterPath shape() const;

	/// @return configuration that link had before reshaping
	QPolygonF savedLine() const;

	/// @return true if the link is being reshaped
	bool isReshapeStarted() const;

	/// @return List of context menu actions available for a particular link type
	virtual QList<ContextMenuAction *> extraActions(QPointF const &pos);

	/// Provide undo-redo support for context actions
	void connectAction(ContextMenuAction *action, QObject *receiver, char const *slot) const;

protected slots:
	/// Remove all intermediate points
	void minimize();

	/// Start tracking edge changes
	void startReshape();

	/// Execute and reset reshape command
	void endReshape();

protected:
	/// Reimplement this method in subclass to make type-dependent actions when lay out
	/// Default implementation does nothing
	virtual void improveAppearance();

	/// Delete loops on a link (when link intersects itself)
	void deleteLoops();

	/// Find out whether there is loop containing QLineF(line[startPos], line[startPos + 1]).
	/// If there is such loop, delete it.
	void deleteLoop(QPolygonF &line, int startPos);

	/// @return true if a link can connect to some port at the position pos (in link's coordinates)
	bool checkPort(QPointF const &pos, bool isStart) const;

	/// @return true if a node at the current link end is not the same as the link's src/dst (depends on isStart value)
	bool nodeChanged(bool isStart) const;

	/// @return index of the point, that is not contained in link's src/dst, while the previous point is
	int firstOutsidePoint(bool startFromSrc) const;

	void connectAndArrange(bool reconnectSrc, bool reconnectDst);

	/// @return Number of point under position pos (if there is no such point, return -1)
	int definePoint(QPointF const &pos) const;

	/// @return Number of segment under position pos (if there is no such segment, return -1)
	int defineSegment(QPointF const &pos) const;

	/// Should be redefined in subclasses to handle mouse move events. Default implementation does nothing.
	virtual void handleEdgeMove(QPointF const &pos);

	/// Highlight ports of node under link's end
	void highlightPorts(bool isStart);

	/// Clear highlights
	void dehighlightPorts();

	/// Draw a port with a given portNumber
	virtual void drawPort(QPainter *painter, int portNumber);

	/// Helper function for sorting edges on linear ports
	QPointF portArrangePoint(NodeElement const *node) const;

	EdgeElement *mEdge; // Doesn't take ownership

	QPolygonF mSavedLine;
	int mDragType;
	QPointF mDragStartPoint;

	NodeElement *mNodeWithHighlightedPorts; // Doesn't take ownership

	commands::ReshapeEdgeCommand *mReshapeCommand;
	bool mReshapeStarted;
};

}
