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
	AbstractSensorImplementation(inputPort::InputPortEnum const &port, sensorType::SensorTypeEnum const &sensorType);
	virtual ~AbstractSensorImplementation() {}
	virtual void read() = 0;
	void setPort(inputPort::InputPortEnum const &port);
	inputPort::InputPortEnum port() const;
	sensorType::SensorTypeEnum type() const;
	virtual void configure() = 0;

signals:
	void response(int reading);
	void failure();
	void configured();

protected:
	enum State {
		idle
		, pending
	};

	inputPort::InputPortEnum mPort;
	State mState;
	sensorType::SensorTypeEnum mType;
};

}
}
}
}
}
}
