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
	virtual void improveAppearance();
	void moveSegment(QPointF const &pos);
};

}
