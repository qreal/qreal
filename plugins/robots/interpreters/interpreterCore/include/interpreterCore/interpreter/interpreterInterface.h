#pragma once

#include <QtCore/QObject>

#include <qrkernel/ids.h>
#include <interpreterBase/interpreterControlInterface.h>

namespace interpreterCore {
namespace interpreter {

/// Interface that allows interpreter to communicate with other parts of a system.
class InterpreterInterface : public interpreterBase::InterpreterControlInterface
{
	Q_OBJECT

public:
	virtual ~InterpreterInterface() {}

	/// Returns time in ms that passed from the moment of last intepretation start.
	/// @warning: may return unreal time (for example when 2D model is being interpreted).
	virtual int timeElapsed() const = 0;

public slots:
	/// Called when establishing connection to a robot is requested. If connection is already established must
	/// disconnect from robot. May do nothing if interpretation is running, to avoid leaving robot in with motors
	/// turned on and without connection to stop it.
	virtual void connectToRobot() = 0;
};

}
}
