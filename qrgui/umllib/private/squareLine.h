#pragma once

#include "lineHandler.h"

namespace qReal {

class SquareLine : public LineHandler
{
public:
	SquareLine(EdgeElement *edge);

	virtual void moveEdge(QPointF const &pos, bool needAlign);
	virtual void adjust();

protected:
	enum LineType {
		vertical,
		horizontal,
		verticalTurn,
		horizontalTurn
	};

	virtual void handleIntersections();
	virtual void improveAppearance();
	void moveSegment(QPointF const &oldPos, QPointF const &newPos);

	void squarize();
	int defineType();

	void verticalSquareLine(QPolygonF &line);
	void horizontalSquareLine(QPolygonF &line);
	void verticalTurningSquareLine(QPolygonF &line);
	void horizontalTurningSquareLine(QPolygonF &line);
};

}
