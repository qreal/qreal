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
	NullSensorImplementation(inputPort::InputPortEnum const &port);
	virtual void read();

private:
	QTimer mActiveWaitingTimer;
protected slots:
	virtual void timerTimeout() = 0;
};

}
}
}
}
}
}
