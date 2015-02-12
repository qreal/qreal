#pragma once

#include <QtCore/QVector>

#include <interpreterBase/robotModel/robotParts/abstractSensor.h>

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

/// Sensor that returns a vector of integer values with cardinality depending on specific sensor (3 for accelerometer,
/// for example).
class ROBOTS_INTERPRETER_BASE_EXPORT VectorSensor : public AbstractSensor
{
	Q_OBJECT

public:
	/// Constructor, takes device type info and port on which this sensor is configured.
	VectorSensor(const DeviceInfo &info, const PortInfo &port);

signals:
	/// Emitted when new data is ready. Note that concrete sensors may work in "pull" and in "push" mode. In "pull"
	/// mode this signal is emitted when sensor reads data after "read" call, in "push" mode this signal is emitted
	/// every time new data is ready, regardless of "read" calls.
	void newData(QVector<int> reading);
};

}
}
}
