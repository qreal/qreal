#pragma once

#include <QtCore/QObject>

#include <qrkernel/ids.h>

namespace interpreterCore {
namespace interpreter {

class InterpreterInterface : public QObject
{
	Q_OBJECT

public:
	virtual ~InterpreterInterface() {}

	/// @todo What it is doing here?
	virtual qReal::IdList providedBlocks() const = 0;

public slots:
	virtual void interpret() = 0;
	virtual void stopRobot() = 0;
	virtual void connectToRobot() = 0;
};

}
}
