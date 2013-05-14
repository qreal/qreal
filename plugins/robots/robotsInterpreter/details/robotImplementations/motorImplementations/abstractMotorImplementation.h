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

	/// Enables engine and applies specified parameters on it
	/// @param speed The power of engine`s rotation
	/// @param breakMode Specifies engine`s breaking behaviour:
	/// true = break, false = float
	virtual void on(int speed, bool breakMode = true) = 0;

	/// Enables engine and applies specified parameters on it
	/// @param speed The power of engine`s rotation
	/// @param degrees Tacho limit; 0 means no tacho limit
	/// @param breakMode Specifies engine`s breaking behaviour:
	/// true = break, false = float
	virtual void on(int speed, long unsigned int degrees, bool breakMode = true) = 0;

	/// Stops engine rotation but engine remains enabled. Equal to on(0, breakMode)
	virtual void stop(bool breakMode = true) = 0;

	/// Stops engine rotation and turns it off
	virtual void off() = 0;

	/// Resets encoder variables to zero
	virtual void resetMotorPosition(bool relative) = 0;

	void setPort(int const port);

signals:
	void motorImplTimeout();

protected:
	outputPort::OutputPortEnum mPort;
};

}
}
}
}
}
}
