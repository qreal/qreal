#pragma once

#include "plugins/editorPluginInterface/portHelpers.h"

/** @brief A factory creating concrete port types (StatLine and StatPoint) */
class PortFactory : public PortFactoryInterface
{
public:
	virtual PortInterface *createPort(const QPointF &point, bool propX, bool propY, int initWidth, int initHeight
			, PortImpl *impl) const;
	virtual PortInterface *createPort(QLineF const &line, bool propX1, bool propY1, bool propX2, bool propY2
			, int initWidth, int initHeight, PortImpl *impl) const;
};
