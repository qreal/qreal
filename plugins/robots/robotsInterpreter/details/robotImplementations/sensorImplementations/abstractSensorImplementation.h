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
	AbstractSensorImplementation(robots::enums::inputPort::InputPortEnum const port, robots::enums::sensorType::SensorTypeEnum const &sensorType);
	virtual ~AbstractSensorImplementation() {}
	virtual void read() = 0;
	void setPort(robots::enums::inputPort::InputPortEnum const port);
	robots::enums::inputPort::InputPortEnum port() const;
	robots::enums::sensorType::SensorTypeEnum type() const;
	virtual void configure() = 0;

	/// Performs dummy sensor reading with 0 result; used for sensor variables nullification
	virtual void nullify();

signals:
	void response(int reading);
	void failure();
	void configured();

protected:
	enum State {
		idle
		, pending
	};

	robots::enums::inputPort::InputPortEnum mPort;
	State mState;
	robots::enums::sensorType::SensorTypeEnum mType;
};

}
}
}
}
}
}
