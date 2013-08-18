#pragma once

#include "lineHandler.h"

namespace qReal {

class CurveLine : public LineHandler
{
public:
	CurveLine(EdgeElement *edge);

	virtual void moveEdge(QPointF const &pos, bool needAlign);

	virtual void drawLine(QPainter *painter, bool drawSavedLine);
	virtual void drawPorts(QPainter *painter);
	virtual QPainterPath shape() const;

protected:
	void setBezierPoints();
	QPainterPath bezierCurve(QPolygonF const &line) const;

	virtual void drawPort(QPainter *painter, int portNumber);
	void drawControlLines(QPainter *painter);
};

}
