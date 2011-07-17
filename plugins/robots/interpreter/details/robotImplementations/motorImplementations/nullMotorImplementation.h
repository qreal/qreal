#pragma once
#include "abstractMotorImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace motorImplementations {

class NullMotorImplementation : public AbstractMotorImplementation
{
	Q_OBJECT
public:
	NullMotorImplementation(int const port);
	virtual void on(int speed);
	virtual void on(int speed, long unsigned int degrees);
	virtual void stop() {}
	virtual void off() {}
	virtual void resetMotorPosition(bool relative);
};

}
}
}
}
}
}
