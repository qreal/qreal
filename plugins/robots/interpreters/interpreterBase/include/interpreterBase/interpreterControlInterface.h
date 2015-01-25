#pragma once

#include <QtCore/QObject>

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {

/// Callback methods that enable plugin to communicate with interpreter, such as start or stop execution.
class ROBOTS_INTERPRETER_BASE_EXPORT InterpreterControlInterface : public QObject
{
	Q_OBJECT

public:
	virtual ~InterpreterControlInterface() {}

signals:
	/// Emitted when connection to robot is established, devices are configured and interpretation is started.
	void started();

	/// Emitted when program is finished or stopped by user.
	void stopped();

public slots:
	/// Starts interpteration process.
	virtual void interpret() = 0;

	/// Stops interpteration, reinitialises robot model.
	virtual void stopRobot() = 0;
};

}
