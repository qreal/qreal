#pragma once

#include "umllib/private/lineHandler.h"

namespace qReal {

/// @brief A strategy class for handling square link (consisting of strict vertical or horizontal lines)
/// User may move non-end segments of the link. Link is laid out in a way that is doesn't intersect
/// adjacent nodes.
class SquareLine : public LineHandler
{
	Q_OBJECT
public:
	SquareLine(EdgeElement *edge);

	/// Adjust link's ends, keep end segments strait
	virtual void adjust();

	/// @return criteria for sorting square links on a linear port
	virtual EdgeArrangeCriteria arrangeCriteria(NodeElement const *node, QLineF const &portLine) const;

	/// @return list of context menu actions available for square link at position pos
	virtual QList<ContextMenuAction *> extraActions(QPointF const &pos);

protected:
	enum LineType {
		vertical
		, horizontal
		, verticalTurn
		, horizontalTurn
	};

	/// If link reshaping is started on a segment, move that segment to position pos (in link's local coordinates)
	/// If user is trying to move port, do it. Otherwise do nothing
	virtual void handleEdgeMove(QPointF const &pos);

	/// Move segment at point oldPos to newPos, keeping the link square
	void moveSegment(QPointF const &oldPos, QPointF const &newPos);

	/// Remove loops, too short segments and coinciding points,
	/// ensure that link doesn't intersect adjacent nodes and stays square
	virtual void improveAppearance();

	/// Delete very short (non-end) segments
	void deleteShortSegments();

	/// Helper function for linear ports arranging
	QPointF portArrangePoint(NodeElement const *node) const;

	/// Ensure that both end segments are strict
	void adjustEndSegments();

	/// Ensure that first segment is strict
	void adjustStart();

	/// Ensure that last segment is strict
	void adjustEnd();

	/// Determine whether the link intersect src or dst
	bool needCorrect() const;
	bool needCorrectSrc() const;
	bool needCorrectDst() const;

	/// Determine whether the link is already square
	bool isSquareLine() const;

	/// Determine whether given line intersects given node
	bool intersects(QLineF const &line, NodeElement *node) const;

	/// Make the link square, ensure it doesn't intersect adjacent nodes
	void squarize();

	/// Define the type of square link configuration
	LineType defineType() const;

	void createHorizontalLine(QPolygonF &line) const;
	void createVerticalLine(QPolygonF &line) const;

	/// Correct link to make it not intersect src and dst
	void correctLine(QPolygonF &line, bool correctSrc, bool correctDst, bool isVertical) const;
	void correctVerticalLine(QPolygonF &line, bool correctSrc, bool correctDst) const;
	void correctHorizontalLine(QPolygonF &line, bool correctSrc, bool correctDst) const;

	void createHorizontalInnerLine(QPolygonF &line, bool correctSrc, bool correctDst) const;
	void createVerticalInnerLine(QPolygonF &line, bool correctSrc, bool correctDst) const;
	void verticalInnerLine(QPolygonF &line) const;
	void horizontalInnerLine(QPolygonF &line) const;

	void verticalSquareLine(QPolygonF &line, qreal middleLine) const;
	void horizontalSquareLine(QPolygonF &line, qreal middleLine) const;
	void verticalTurningSquareLine(QPolygonF &line) const;
	void horizontalTurningSquareLine(QPolygonF &line) const;

	void verticalSameSide(QPolygonF &line, bool topLoop) const;
	void horizontalSameSide(QPolygonF &line, bool leftLoop) const;

	/// @return Point that is outside of corresponding adjacent node and placed near port
	QPointF outgoingPoint(bool isStart) const;

	/// If there is some space (horizontal lane) between src and dst return bounds of this lane.
	/// Otherwise, the first element of return value would be greater than the second.
	QPair<qreal, qreal> verticalIntermediateSpace() const;

	/// If there is some space (vertical lane) between src and dst return bounds of this lane.
	/// Otherwise, the first element of return value would be greater than the second.
	QPair<qreal, qreal> horizontalIntermediateSpace() const;

	/// @return link src's contents rectangle in link's coordinates
	QRectF srcRect() const;

	/// @return link dst's contents rectangle in link's coordinates
	QRectF dstRect() const;

	/// Draw port with index portNumber is its non-end
	virtual void drawPort(QPainter *painter, int portNumber);

	ContextMenuAction mLayOutAction;
};

}
