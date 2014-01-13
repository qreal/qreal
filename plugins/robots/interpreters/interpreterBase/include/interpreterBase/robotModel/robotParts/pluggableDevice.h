#pragma once

#include <QtCore/QObject>

#include "interpreterBase/robotModel/portInfo.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

class ROBOTS_INTERPRETER_BASE_EXPORT PluggableDevice : public QObject
{
	Q_OBJECT

public:
	PluggableDevice(PortInfo const &port);
	virtual ~PluggableDevice();

	PortInfo const &port() const;

private:
	PortInfo mPort;
};

}
}
}
