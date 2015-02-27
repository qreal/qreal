#pragma once

#include <kitBase/robotModel/robotParts/device.h>
#include <kitBase/kitBaseDeclSpec.h>

namespace kitBase {
namespace robotModel {
namespace robotParts {

/// Abstract sensor device which can be asked to provide readings. Signals with readings are defined in subclasses
/// since readings can have different types, like scalars for touch or range sensor, or vector for accelerometer.
/// Sensors can also work in "push" mode, providing values as soon as they are ready, in that case "read()" call is
/// ignored.
/// General workflow is to subscribe to "newData" and "failure" signals and call read().
class ROBOTS_KIT_BASE_EXPORT AbstractSensor : public Device
{
	Q_OBJECT
	Q_CLASSINFO("direction", "input")

public:
	/// Constructor, takes device type info and port on which this sensor is configured.
	AbstractSensor(const DeviceInfo &info, const PortInfo &port);

	/// Ask sensor to provide new readings. Returns immediately. When reading is done, sensor emits "newData" signal
	/// with reading as a parameter.
	/// This call is ignored if sensor works in a "push" mode and always provides readings as soon as they are ready.
	/// If more than one call of "read" was made before sensor returned values, subsequent calls are ignored.
	virtual void read() = 0;

signals:
	/// Emitted when sensor reading is failed for some reason. Note that it is possible that sensor will not emit any
	/// signal in case of a failure, so if "failure()" is emitted, the sensor is failed, but if not, sensor may be still
	/// preparing readings and it can take some time, or simply failed silently.
	void failure();
};

}
}
}
