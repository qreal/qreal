#pragma once
#include <QtCore/QObject>
#include "../../robotCommandConstants.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace motorImplementations {

class AbstractMotorImplementation : public QObject
{
	Q_OBJECT
public:
	AbstractMotorImplementation(int const port);
	virtual void on(int speed) = 0;
	virtual void on(int speed, long unsigned int degrees) = 0;
	virtual void stop() = 0;
	virtual void off() = 0;
	virtual void resetMotorPosition(bool relative) = 0;
	void setPort(int const port);
protected:
	outputPort::OutputPortEnum mPort;
};

}
}
}
}
}
}
