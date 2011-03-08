#pragma once
#include <QtCore/QObject>
#include "../../../../kernel/ids.h"
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
private:
	outputPort::OutputPortEnum mPort;
	robotImplementations::motorImplementations::AbstractMotorImplementation *mMotorImpl;
};

}
}
}
}
}
