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
	Sensor(robotImplementations::sensorImplementations::AbstractSensorImplementation *sensorImpl, inputPort::InputPortEnum const &port);
	virtual ~Sensor();
	virtual void read();
	robotImplementations::sensorImplementations::AbstractSensorImplementation *sensorImpl();
	void setImplementation(robotImplementations::sensorImplementations::AbstractSensorImplementation *sensorImpl);

protected:
	inputPort::InputPortEnum mPort;
	robotImplementations::sensorImplementations::AbstractSensorImplementation *mSensorImpl;  // Doesn't have ownership
};

}
}
}
}
}
