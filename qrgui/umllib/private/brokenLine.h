#pragma once

#include "lineHandler.h"

namespace qReal {

class BrokenLine : public LineHandler
{
public:
	BrokenLine(EdgeElement *edge);

protected:
	virtual void handleEdgeMove(QPointF const &pos, bool needAlign);
	virtual void improveAppearance();

	virtual QPointF alignedPoint(QPointF const &point) const;
};

}
