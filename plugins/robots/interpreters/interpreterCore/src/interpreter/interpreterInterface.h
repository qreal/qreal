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

	/// @todo What it is doing here?
	virtual qReal::IdList providedBlocks() const = 0;

signals:
	void started();
	void stopped();

public slots:
	virtual void connectToRobot() = 0;
};

}
}
