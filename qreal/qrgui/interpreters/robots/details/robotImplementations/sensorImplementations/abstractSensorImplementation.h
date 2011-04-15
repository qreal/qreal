#pragma once
#include <QtCore/QObject>
#include "../../../sensorConstants.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace sensorImplementations {

class AbstractSensorImplementation : public QObject
{
	Q_OBJECT
public:
	AbstractSensorImplementation(inputPort::InputPortEnum const &port);
	virtual ~AbstractSensorImplementation() {};
	virtual void read() = 0;
	void setPort(inputPort::InputPortEnum const &port);

signals:
	void response(int reading);
	void failure();

protected:
	enum State {
		idle
		, pending
	};

	inputPort::InputPortEnum mPort;
	State mState;
};

}
}
}
}
}
}
