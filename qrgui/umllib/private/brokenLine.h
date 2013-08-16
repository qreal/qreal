#pragma once

#include "lineHandler.h"

namespace qReal {

class BrokenLine : public LineHandler
{
public:
	BrokenLine(EdgeElement * edge);

	virtual void moveEdge(QPointF const &pos, bool needAlign);

protected:
	virtual void improveAppearance();
};

}
