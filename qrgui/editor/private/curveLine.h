#pragma once

#include "editor/private/lineHandler.h"

namespace qReal {

/// @brief A strategy class for handling Bezier curve link
class CurveLine : public LineHandler
{
	Q_OBJECT
public:
	CurveLine(EdgeElement *edge);

	/// Draws Bezier curve
	virtual void drawLine(QPainter *painter, bool drawSavedLine);

	/// Draw ports and control lines for editing curve link
	virtual void drawPorts(QPainter *painter);

	/// @return Link's Bezier curve
	virtual QPainterPath shape() const;

	/// Adjust end points and update curve configuration
	virtual void adjust();

protected:
	/// Create correct curve configuration (two ports, two intermediate points)
	void setBezierPoints();

	/// Create Bezier curve based on the line
	QPainterPath bezierCurve(QPolygonF const &line) const;

	/// If user is dragging a point then move it. Otherwise, do nothing
	virtual void handleEdgeMove(const QPointF &pos);

	/// Draw ordinary port or special curve control point
	virtual void drawPort(QPainter *painter, int portNumber);

	/// Draw dash lines used for editing of Bezier curve
	void drawControlLines(QPainter *painter);
};

}
