#pragma once

#include <interpreterBase/robotModel/robotParts/abstractSensor.h>

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

/// Sensor that returns single integer value.
class ROBOTS_INTERPRETER_BASE_EXPORT ScalarSensor : public AbstractSensor
{
	Q_OBJECT

	Q_PROPERTY(int value READ lastData)

public:
	/// Constructor, takes device type info and port on which this sensor is configured.
	ScalarSensor(const DeviceInfo &info, const PortInfo &port);

	/// Returns last value passed to 'newData' signal parameter, i.e. the last obtained value from the sensor.
	int lastData() const;

signals:
	/// Emitted when new data is ready. Note that concrete sensors may work in "pull" and in "push" mode. In "pull"
	/// mode this signal is emitted when sensor reads data after "read" call, in "push" mode this signal is emitted
	/// every time new data is ready, regardless of "read" calls.
	void newData(int reading);

private:
	int mLastValue;
};

}
}
}
