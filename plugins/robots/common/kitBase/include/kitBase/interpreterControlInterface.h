/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtCore/QObject>

#include <qrutils/interpreter/stopReason.h>
#include "kitBase/kitBaseDeclSpec.h"

namespace kitBase {

/// Callback methods that enable plugin to communicate with interpreter, such as start or stop execution.
class ROBOTS_KIT_BASE_EXPORT InterpreterControlInterface : public QObject
{
	Q_OBJECT

public:
	InterpreterControlInterface() {
		connect(this
				, &InterpreterControlInterface::stopAllInterpretation
				, this
				, &InterpreterControlInterface::stopRobot);
	}

	virtual ~InterpreterControlInterface() {}

signals:
	/// Emitted when connection to robot is established, devices are configured and interpretation is started.
	void started();

	/// Emitted when program is finished or stopped by user.
	void stopped(qReal::interpretation::StopReason reason);

	/// @todo: temporary hack while js interpreter is separated from regular interpreter logic
	void stopAllInterpretation(qReal::interpretation::StopReason reason = qReal::interpretation::StopReason::userStop);

	/// same
	void startJsInterpretation();

public slots:
	/// Starts interpteration process.
	virtual void interpret() = 0;

	/// Stops interpretation, reinitialises robot model.
	/// @param reason The reason why program execution stopped.
	virtual void stopRobot(qReal::interpretation::StopReason reason) = 0;

	/// An alias for stopRobot(StopReason::userStop) for convenient connection with buttons.
	void userStopRobot()
	{
		stopRobot(qReal::interpretation::StopReason::userStop);
	}
};

}
