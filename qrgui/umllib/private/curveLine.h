#pragma once

#include "lineHandler.h"

namespace qReal {

class CurveLine : public LineHandler
{
public:
	CurveLine(EdgeElement *edge);

	virtual void moveEdge(QPointF const &pos, bool needAlign);
};

}
