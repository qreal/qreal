#include "portFactory.h"

#include "umllib/ports/statLine.h"
#include "umllib/ports/statPoint.h"

PortInterface * PortFactory::createPort(QLineF const &line, bool propX1, bool propY1, bool propX2, bool propY2
		, int initWidth, int initHeight, PortImpl *impl) const
{
	return new StatLine(line, propX1, propY1, propX2, propY2, initWidth, initHeight, impl);
}

PortInterface * PortFactory::createPort(QPointF const &point, bool propX, bool propY
		, int initWidth, int initHeight, PortImpl *impl) const
{
	return new StatPoint(point, propX, propY, initWidth, initHeight, impl);
}
