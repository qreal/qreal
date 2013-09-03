#pragma once

#include <QtCore/QDebug>
#include <QtCore/QObject>

#include "../robotCommandConstants.h"
#include "../../sensorConstants.h"
#include "../robotImplementations/sensorImplementations/abstractSensorImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotParts {

class Sensor : public QObject
{
	Q_OBJECT

public:
	Sensor(robotImplementations::sensorImplementations::AbstractSensorImplementation *sensorImpl, robots::enums::inputPort::InputPortEnum const port);
	virtual ~Sensor();
	virtual void read();
	robotImplementations::sensorImplementations::AbstractSensorImplementation *sensorImpl();
	void setImplementation(robotImplementations::sensorImplementations::AbstractSensorImplementation *sensorImpl);

	/// Performs dummy sensor reading with 0 result; used for sensor variables nullification
	void nullify();

protected:
	robots::enums::inputPort::InputPortEnum mPort;
	robotImplementations::sensorImplementations::AbstractSensorImplementation *mSensorImpl;  // Doesn't have ownership
};

}
}
}
}
}
