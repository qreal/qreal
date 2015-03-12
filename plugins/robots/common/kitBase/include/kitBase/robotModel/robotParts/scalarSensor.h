#pragma once

#include <kitBase/robotModel/robotParts/abstractSensor.h>

#include "kitBase/kitBaseDeclSpec.h"

namespace kitBase {
namespace robotModel {
namespace robotParts {

/// Sensor that returns single integer value.
class ROBOTS_KIT_BASE_EXPORT ScalarSensor : public AbstractSensor
{
	Q_OBJECT

public:
	/// Constructor, takes device type info and port on which this sensor is configured.
	ScalarSensor(const DeviceInfo &info, const PortInfo &port);

signals:
	/// Emitted when new data is ready. Note that concrete sensors may work in "pull" and in "push" mode. In "pull"
	/// mode this signal is emitted when sensor reads data after "read" call, in "push" mode this signal is emitted
	/// every time new data is ready, regardless of "read" calls.
	void newData(int reading);
};

}
}
}
