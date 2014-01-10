#pragma once

#include <QtCore/QObject.h>

#include "interpreterBase/robotModel/portInfo.h"
#include "interpreterBase/robotModel/robotParts/pluggableDevice.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {

class ROBOTS_INTERPRETER_BASE_EXPORT ConfigurationInterface : public QObject
{
	Q_OBJECT

public:
	enum PortDirection {
		defaultDirection
		, input
		, output
	};

	virtual void configureDevice(robotParts::PluggableDevice const &device, PortInfo const &port) = 0;

	virtual robotParts::PluggableDevice &pluggableDevice(
			PortInfo const &port
			, PortDirection direction = defaultDirection) = 0;

	// TODO: Implement some convenience methods that cast generic PluggableDevice to desired sensor/motor type.

signals:
	void sensorsConfigured();
};

}
}
