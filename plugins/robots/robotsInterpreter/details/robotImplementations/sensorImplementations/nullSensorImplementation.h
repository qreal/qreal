#pragma once

#include <QtCore/QTimer>

#include "abstractSensorImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace sensorImplementations {

class NullSensorImplementation : public AbstractSensorImplementation
{
	Q_OBJECT

public:
	NullSensorImplementation(
			robots::enums::inputPort::InputPortEnum const port
			, robots::enums::sensorType::SensorTypeEnum const &sensorType
			);

	virtual void read();
	virtual void configure();

protected slots:
	virtual void timerTimeout() = 0;

private:
	QTimer mActiveWaitingTimer;
};

}
}
}
}
}
}
