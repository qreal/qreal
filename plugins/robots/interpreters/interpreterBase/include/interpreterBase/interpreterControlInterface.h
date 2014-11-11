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

public slots:
	virtual void interpret() = 0;
	virtual void stopRobot() = 0;
};

}
