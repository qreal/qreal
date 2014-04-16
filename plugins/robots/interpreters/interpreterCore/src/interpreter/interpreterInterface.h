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

signals:
	void started();
	void stopped();

public slots:
	virtual void connectToRobot() = 0;
};

}
}
