#pragma once
#include <QtCore/QObject>
#include "../../../../../qrkernel/ids.h"
#include "../robotCommandConstants.h"
#include "../robotImplementations/motorImplementations/abstractMotorImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotParts {

class Motor : public QObject
{
	Q_OBJECT
public:
	Motor(int const port, robotImplementations::motorImplementations::AbstractMotorImplementation *motorImpl);
	~Motor();
	virtual void on(int speed);
	virtual void on(int speed, long unsigned int degrees);
	virtual void stop();
	virtual void off();
	virtual void resetMotorPosition(bool relative);
	robotImplementations::motorImplementations::AbstractMotorImplementation &motorImpl();
	void setImplementation(robotImplementations::motorImplementations::AbstractMotorImplementation *motorImpl);

signals:
	void motorTimeout();

private:
	outputPort::OutputPortEnum mPort;
	robotImplementations::motorImplementations::AbstractMotorImplementation *mMotorImpl;  // Doesn't have ownership.
};

}
}
}
}
}
