#pragma once

#include <QtCore/QObject>

#include <qrkernel/ids.h>
#include <interpreterBase/interpreterControlInterface.h>

namespace interpreterCore {
namespace interpreter {

class InterpreterInterface : public interpreterBase::InterpreterControlInterface
{
	Q_OBJECT

public:
	virtual ~InterpreterInterface() {}

	/// Returns time in ms that passed from the moment of last intepretation start.
	/// @warning: may return unreal time (for example when 2D model is beeng interpreted).
	virtual int timeElapsed() const = 0;

signals:
	void started();
	void stopped();

public slots:
	virtual void connectToRobot() = 0;
};

}
}
